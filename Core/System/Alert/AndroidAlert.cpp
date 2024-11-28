/**
 @file AndroidAlert.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "../Alert.hpp"

#include "StateAlert.hpp"
#include "InputAlert.hpp"
#include "ActionAlert.hpp"

#include "../Native/Android.hpp"

#include <QScopedPointer>
#include <QtAndroid>
#include <QAndroidJniEnvironment>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void Alert::head(StateAlert* state)
	{

	}

	void Alert::head(InputAlert* input)
	{
		if (_headless && !input->getFlagSet().testFlag(InputAlert::Flag::HeadFull)) {
			return ;
		}

		QAndroidJniObject j_text = QAndroidJniObject::fromString(input->getTitle());
		QAndroidJniObject j_placeholder = QAndroidJniObject::fromString(input->getPlaceholder());
		QAndroidJniObject j_ok_label = QAndroidJniObject::fromString(tr("Ok"));
		QAndroidJniObject j_cancel_label = QAndroidJniObject::fromString(tr("Cancel"));

		QAndroidJniObject j_alert_dialog_builder
		(
			"android/app/AlertDialog$Builder", "(Landroid/content/Context;)V",
			QtAndroid::androidContext().object()
		);
		QAndroidJniObject j_edit_text_widget
		(
			"android/widget/EditText", "(Landroid/content/Context;)V",
			QtAndroid::androidContext().object()
		);
		if ( !j_alert_dialog_builder.isValid() || !j_edit_text_widget.isValid()) {
			return ;
		}

		QScopedPointer<DialogClickListener> click_listener(new DialogClickListener(input));
		QObject::connect
		(
			click_listener.data(), &DialogClickListener::done,
			input,
			[input, j_edit_text_widget](int index)
			{
				QAndroidJniObject text = j_edit_text_widget.callObjectMethod("getText", "()Landroid/text/Editable;");
				if (text.isValid()) {
					input->complete(text.callObjectMethod<jstring>("toString").toString());
				}
			}
		);

		QScopedPointer<DialogClickListener> cancel_listener(input->getFlagSet().testFlag(InputAlert::Flag::CancelOption) ? new DialogClickListener(input) : nullptr);
		QObject::connect
		(
			cancel_listener.data(), &DialogClickListener::done,
			input,
			[input](int index)
			{
				input->cancel();
			}
		);

		QtAndroid::runOnAndroidThreadSync
		(
			[&j_text, &j_placeholder, &j_ok_label, &j_cancel_label, &j_alert_dialog_builder, &j_edit_text_widget, &click_listener, &cancel_listener]()
			{
				j_edit_text_widget.callMethod<void>
				(
					"setInputType", "(I)V",
					QAndroidJniObject::getStaticField<jint>("android/text/InputType", "TYPE_CLASS_TEXT")
				);
				j_edit_text_widget.callMethod<void>
				(
					"setHint", "(Ljava/lang/CharSequence;)V",
					j_placeholder.object<jstring>()
				);

				j_alert_dialog_builder.callObjectMethod(
					"setCancelable", "(Z)Landroid/app/AlertDialog$Builder;",
					false
				);
				j_alert_dialog_builder.callObjectMethod
				(
					"setTitle", "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;",
					j_text.object<jstring>()
				);
				j_alert_dialog_builder.callObjectMethod
				(
					"setView", "(Landroid/view/View;)Landroid/app/AlertDialog$Builder;",
					j_edit_text_widget.object<jobject>()
				);
				j_alert_dialog_builder.callObjectMethod
				(
					"setPositiveButton", "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;",
					j_ok_label.object<jstring>(),
					static_cast<jobject>(*(click_listener.take()))
				);

				if (!cancel_listener.isNull()) {
					j_alert_dialog_builder.callObjectMethod
					(
						"setNegativeButton", "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;",
						j_cancel_label.object<jstring>(),
						static_cast<jobject>(*(cancel_listener.take()))
					);
				}

				j_alert_dialog_builder.callObjectMethod("show", "()Landroid/app/AlertDialog;");
			}
		);
	}

	void Alert::head(ActionAlert* action)
	{
		if (_headless && !action->getFlagSet().testFlag(ActionAlert::Flag::HeadFull)) {
			return ;
		}

		QAndroidJniEnvironment j_environment;

		QAndroidJniObject j_text(QAndroidJniObject::fromString("<h2>" + action->getTitle() + "</h2><br><p>" + action->getText() +"</p>"));
		QAndroidJniObject j_span;
		if (QtAndroid::androidSdkVersion() >= static_cast<quint8>(Android::Version::N)) {
			j_span = QAndroidJniObject::callStaticObjectMethod
			(
				"android/text/Html", "fromHtml", "(Ljava/lang/String;I)Landroid/text/Spanned;",
				j_text.object<jstring>(),
				QAndroidJniObject::getStaticField<jint>("android/text/Html", "FROM_HTML_MODE_COMPACT")
			);
		} else {
			j_span = QAndroidJniObject::callStaticObjectMethod
			(
				"android/text/Html", "fromHtml", "(Ljava/lang/String;)Landroid/text/Spanned;",
				j_text.object<jstring>()
			);
		}

		QAndroidJniObject j_alert_dialog_builder
		(
			"android/app/AlertDialog$Builder", "(Landroid/content/Context;)V",
			QtAndroid::androidContext().object()
		);

		QAndroidJniObject j_text_view
		(
			"android/widget/TextView", "(Landroid/content/Context;)V",
			QtAndroid::androidContext().object()
		);

		if (!j_text.isValid() || !j_text_view.isValid() || !j_alert_dialog_builder.isValid()) {
			return ;
		}

		jobjectArray action_array(
									j_environment->NewObjectArray
									(
										action->getOptionPropertyList().length(),
										j_environment->FindClass("java/lang/String"),
										j_environment->NewStringUTF("")
									)
		);
		for (int o = 0, oo = action->getOptionPropertyList().length(); o < oo; ++o) {
			j_environment->SetObjectArrayElement(
												action_array,
												o,
												j_environment->NewStringUTF(action->getOptionPropertyList().at(o)->getCaption().toLocal8Bit().data())
			);
		}
		QAndroidJniObject j_action_array = QAndroidJniObject::fromLocalRef(action_array);

		QScopedPointer<DialogClickListener> click_listener(new DialogClickListener(action));
		QObject::connect
		(
			click_listener.data(), &DialogClickListener::done,
			action,
			[action](int index)
			{
				action->getOptionPropertyList().at(index)->opt();
			}
		);

		QtAndroid::runOnAndroidThreadSync
		(
			[&j_span, &j_alert_dialog_builder, &j_text_view, &j_action_array, &click_listener]()
			{
				j_text_view.callMethod<void>
				(
					"setPadding", "(IIII)V",
					15, 15, 15, 0
				);
				j_text_view.callMethod<void>
				(
					"setText", "(Ljava/lang/CharSequence;)V",
					j_span.object<jstring>()
				);

				j_alert_dialog_builder.callObjectMethod
				(
					"setCancelable", "(Z)Landroid/app/AlertDialog$Builder;",
					false
				);
				j_alert_dialog_builder.callObjectMethod
				(
					"setCustomTitle", "(Landroid/view/View;)Landroid/app/AlertDialog$Builder;",
					j_text_view.object<jstring>()
				);
				j_alert_dialog_builder.callObjectMethod
				(
					"setItems", "([Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;",
					j_action_array.object<jobjectArray>(),
					static_cast<jobject>(*(click_listener.take()))
				);

				j_alert_dialog_builder.callObjectMethod("show", "()Landroid/app/AlertDialog;");
			}
		);
	}
} } } }
