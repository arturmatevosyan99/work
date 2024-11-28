#ifndef Cadabra_Queen_Core_View_StateIsland_hpp
#define Cadabra_Queen_Core_View_StateIsland_hpp

#include <QQuickPaintedItem>

#include "../System/Alert.hpp"
#include "../System/Alert/StateAlert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace View {
	/**
	 * @brief The StateIsland class
	 *
	 * @todo Реализовать минимальный режим отрисовки (только точки)
	 */
	class StateIsland : public QQuickPaintedItem
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::System::Alert* alert READ getAlert WRITE setAlert NOTIFY alertChanged)
		Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		StateIsland(QQuickItem* parent = nullptr);
		virtual ~StateIsland() = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		System::Alert* _alert;
		int _interval;

		int _timer_id;
		int _queue_index;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void paint(QPainter* painter) override;

	private:
		void restartTimer();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		System::Alert* getAlert() const;
		int getInterval() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setAlert(System::Alert* value);
		void setInterval(int value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void alertChanged();
		void intervalChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onAlerted(System::StateAlert* state, bool onoff);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void timerEvent(QTimerEvent *event) override;
	/** @} */
	};
} } } }

#endif
