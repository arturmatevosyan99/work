/**
 @file Windows.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "../Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class Windows : public Native
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Windows(QObject* parent = nullptr);
		virtual ~Windows() override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool filterEvent(QObject* receiver, QEvent* event) override;

		virtual Q_INVOKABLE bool shareFile(QUrl url) override;
		virtual Q_INVOKABLE void openAlbum() override;
		virtual Q_INVOKABLE void openCamera() override;
		virtual Q_INVOKABLE bool openFile(QUrl url) override;
		virtual Q_INVOKABLE void checkApplicationUpdate() override;
		virtual Q_INVOKABLE void runApplicationUpdate() override;
		virtual Q_INVOKABLE void vibrate() override;
		virtual Q_INVOKABLE void alertInput(const QString& title, const QString& placeholder = "", AlertFlags flag_set = {}, const QString& ident ="") override;
		virtual Q_INVOKABLE void alertAction(const QString& title, const QString& message, const QStringList& action_list, AlertFlags flag_set = {}, const QString& ident = "") override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual qreal getScreenKeyboardScreenHeight() const override;
		virtual int getScreenDensity() const override;
		virtual qreal getDefaultFontScale() const override;
	/** @} */
	};
} } } }
