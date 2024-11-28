/**
 @file iOSAlert.mm
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "../Alert.hpp"

#include "InputAlert.hpp"
#include "ActionAlert.hpp"

#include "../Native/iOS.h"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void Alert::head(StateAlert* state)
	{

	}

	void Alert::head(InputAlert* input)
	{
		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc == nil) {
			qWarning("Could not obtain a Window");
			return;
		}

		UIAlertController* alert = [
									UIAlertController
									alertControllerWithTitle: @""
									message: input->getTitle().toNSString()
									preferredStyle: UIAlertControllerStyleAlert
		];

		UIAlertAction* action_ok = [
									UIAlertAction
									actionWithTitle: tr("Ok").toNSString()
									style: UIAlertActionStyleDefault
									handler:^(UIAlertAction * _Nonnull action)
									{
										input->complete(QString::fromNSString(alert.textFields[0].text));
									}
		];
		[alert addAction: action_ok];

		if (input->getFlagSet().testFlag(InputAlert::Flag::CancelOption)) {
			UIAlertAction* action_cancel = [
											UIAlertAction
											actionWithTitle: tr("Cancel").toNSString()
											style: UIAlertActionStyleCancel
											handler: ^(UIAlertAction * _Nonnull action)
											{
												input->cancel();
											}
			];
			[alert addAction: action_cancel];
		}

		[alert
				addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull text_field)
				{
					text_field.placeholder = input->getPlaceholder().toNSString();
				}
		];

		[rvc presentViewController: alert animated: YES completion: nil];
	}

	void Alert::head(ActionAlert* action)
	{
		if (_headless && !action->getFlagSet().testFlag(ActionAlert::Flag::HeadFull)) {
			return ;
		}

		UIViewController* rvc = [QueenKeyWindow() rootViewController];
		if (rvc == nil) {
			return ;
		}

		UIAlertController * alert = [
									UIAlertController
									alertControllerWithTitle: action->getTitle().toNSString()
									message: action->getText().toNSString()
									preferredStyle: action->getFlagSet().testFlag(ActionAlert::Flag::SheetMode)
													? UIAlertControllerStyleActionSheet
													: UIAlertControllerStyleAlert
		];

		for (AlertOption* option : action->getOptionPropertyList()) {
			if (option->getCaption().isEmpty()) {
				continue;
			}

			UIAlertActionStyle style;
			switch (option->getFlag()) {
				case AlertOption::Flag::Discard:
				case AlertOption::Flag::Reset:
				case AlertOption::Flag::Restore:
				case AlertOption::Flag::Abort:
					style = UIAlertActionStyleDestructive;
					break;

				case AlertOption::Flag::Cancel:
					style = UIAlertActionStyleCancel;
					break;

				default:
					style = UIAlertActionStyleDefault;
					break;
			}

			[
				alert
				addAction:
				[
					UIAlertAction
					actionWithTitle: option->getCaption().toNSString()
					style: style
					handler: ^(UIAlertAction*)
					{
						action->opt(option);
					}
				]
			];
		}

		[rvc presentViewController: alert animated: YES completion: nil];
	}
} } } }
