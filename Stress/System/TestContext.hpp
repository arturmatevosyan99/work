#ifndef Cadabra_Queen_Stress_TestContext_hpp
#define Cadabra_Queen_Stress_TestContext_hpp

#include "../../Environment.hpp"

#include "../../Core/Test/AbstractCase.hpp"

#include <QString>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlIncubationController>

namespace Cadabra { namespace Queen { namespace Stress { namespace System {
	class TestContext : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
        Q_PROPERTY(Cadabra::Queen::Core::Test::AbstractCase* currentCase READ getCurrentCase NOTIFY currentCaseChanged)
		Q_CLASSINFO("DefaultProperty", "currentCase")
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TestContext(QObject* parent = nullptr);
		virtual ~TestContext();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QQmlEngine _qml_engine;
		QQmlContext _qml_context;
		QQmlIncubationController _qml_incubation_controller;

		Core::Test::AbstractCase* _current_case;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void clear();
		bool interrupt(bool onoff);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE QVariant getPreferenceOption(const QString& key, const QVariant& default_value) const;
		Q_INVOKABLE QVariant getEnvironmentVariable(const QString& key, const QVariant& default_value = QVariant{}) const;

		Core::Test::AbstractCase* getCurrentCase() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void put(Core::Test::AbstractCase* value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void currentCaseChanged();
		void engineQuited();
		void engineExited(int retcode);
		void engineWarned(const QList<QQmlError>& warning_list);
	/** @} */

	/** @name Slots */
	/** @{ */
	public:
		virtual void timerEvent(QTimerEvent *) override;
	/** @} */

	/** @name States */
	/** @{ */
    public:
	/** @} */
	};
} } } }

#endif
