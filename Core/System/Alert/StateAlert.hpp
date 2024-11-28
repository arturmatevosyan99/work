#ifndef Cadabra_Queen_Core_System_Alert_StateAlert_hpp
#define Cadabra_Queen_Core_System_Alert_StateAlert_hpp

#include "../Alert.hpp"

#include <QQuickPaintedItem>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class StateAlert : public AbstractAlert
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(QObject* source READ getSource CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit StateAlert(QObject* parent = nullptr);
		explicit StateAlert(QObject* parent, const QString& name, const QString& title, const QString& description, QObject* source = nullptr, AbstractAlert::Level level = AbstractAlert::Level::Info);
		virtual ~StateAlert() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _description;
		QObject* _source;
		int _timer_id;
		int _timer_interval;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool warn() override;
		Q_INVOKABLE bool discard();
		void stopTimer();
		void startTimer(int value);
		void progressTimer(int value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getDescription() const;
		QObject* getSource() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setDescription(const QString& value);
		void setTimerInterval(int value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void timerExpired();
		void descriptionChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
		virtual void timerEvent(QTimerEvent* event) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
