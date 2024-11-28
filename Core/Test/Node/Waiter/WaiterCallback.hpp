#ifndef Cadabra_Queen_Core_Test_Node_Waiter_WaiterCallback_hpp
#define Cadabra_Queen_Core_Test_Node_Waiter_WaiterCallback_hpp

#include <QObject>

#include <QString>
#include <QJSValue>

#if defined(QT_TESTLIB_LIB)
	#include <QtTest/QSignalSpy>
#endif

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class WaiterCallback : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QJSValue callee READ getCallee WRITE setCallee NOTIFY calleeChanged)

		Q_CLASSINFO("DefaultProperty", "callee")
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		#if defined(QT_TESTLIB_LIB)
			using SignalSpy = QSignalSpy;
		#else
			class SignalSpy
			{
			public:
				SignalSpy(const QObject*, const QMetaMethod&) {};
				bool isEmpty() { return true; }
				QList<QVariant> takeFirst() { return {}; };
			};
		#endif
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		WaiterCallback(QObject* parent = nullptr);
		virtual ~WaiterCallback() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _name;
		QJSValue _callee;

		QScopedPointer<WaiterCallback::SignalSpy> _spy;
		QList<QByteArray> _signal_parameter_names_list;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getName() const;
		const QJSValue& getCallee() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setName(const QString& value);
		void setCallee(const QJSValue& callee);

		void setSignalSpy(const QPointer<QObject>& target, const QMetaMethod& signal);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void nameChanged();
		void calleeChanged();
		void done();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onDone();
	/** @} */
	};
} } } }

#endif
