#ifndef Cadabra_Queen_Core_System_Native_iOS_h
#define Cadabra_Queen_Core_System_Native_iOS_h

#import <UIKit/UIKit.h>
#import <UIKit/UIWindow.h>
#import <Foundation/Foundation.h>

UIWindow* QueenKeyWindow();
NSDictionary* QueenLaunchDictionary();

@interface QIOSApplicationDelegate : UIResponder<UIApplicationDelegate>
@end

@interface QIOSApplicationDelegate(Queen)
@end

#include "../Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class iOS : public Native
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit iOS(QObject* parent = nullptr);
		virtual ~iOS() override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool filterEvent(QObject* receiver, QEvent* event) override;

		virtual Q_INVOKABLE bool openFile(QUrl url) override;
		virtual Q_INVOKABLE bool shareFile(QUrl url) override;
		virtual Q_INVOKABLE void openAlbum() override;
		virtual Q_INVOKABLE void openCamera() override;
		virtual Q_INVOKABLE void requestApplicationRate() override;
		virtual Q_INVOKABLE void checkApplicationUpdate() override;
		virtual Q_INVOKABLE void runApplicationUpdate() override;
		virtual Q_INVOKABLE void vibrate() override;
		virtual Q_INVOKABLE void alertInput(const QString& title, const QString& placeholder = "", AlertFlags flag_set = {}, const QString& ident ="") override;
		virtual Q_INVOKABLE void alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set = {}, const QString& ident = "") override;

	protected:
		void updateUpplication(bool auto_offer);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual qreal getScreenKeyboardScreenHeight() const override;
		virtual qreal getDefaultFontScale() const override;
	/** @} */
	};
} } } }

#endif
