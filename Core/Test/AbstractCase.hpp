#ifndef Cadabra_Queen_Core_Test_AbstractCase_hpp
#define Cadabra_Queen_Core_Test_AbstractCase_hpp

#include "../Core.hpp"

#include "Context.hpp"

#include <QObject>
#include <QString>
#include <QWaitCondition>
#include <QQmlComponent>
#include <QtTest/QtTest>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class AbstractCase : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractCase(QObject* parent = nullptr);
		virtual ~AbstractCase() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Context _context;

		QWaitCondition _loader;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool run();
		virtual bool run(const QString& filename, TestDialect dialect) final;

	protected:
		virtual bool dial(QQmlIncubator& incubator);
		virtual bool dial(const QQmlIncubator& incubator);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const Context& getContext() const;

	protected:
		virtual Context& getContext();
	/** @} */

	/** @name Setters */
	/** @{ */
	protected:
	/** @} */

	/** @name signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		/**
		 * @brief Will be called before the first test function is executed.
		 * Use for preparing the test. Every test should leave the system in a usable state, so it can be run repeatedly.
		 * If fails, no test function will be executed.
		 */
		virtual void initTestCase() = 0;

		/**
		 * @brief Will be called to create a global test data table.
		 */
		virtual void initTestCase_data() = 0;

		/**
		 * @brief Will be called after the last test function was executed.
		 * Cleanup operations should be handled here, so they get run even if the test fails.
		 */
		virtual void cleanupTestCase() = 0;

		/**
		 * @brief Will be called before each test function is executed.
		 * Use for preparing a test function. Every test function should leave the system in a usable state, so it can be run repeatedly.
		 * If fails, the following test function will not be executed, the test will proceed to the next test function.
		 */
		virtual void init() = 0;

		/**
		 * @brief Will be called after every test function.
		 * Cleanup operations should be handled here, so they get run even if the test function fails and exits early.
		 */
		virtual void cleanup() = 0;

	protected slots:
		void onStatusChanged(const QQmlComponent& component);
	/** @} */
	};
} } } }

#endif
