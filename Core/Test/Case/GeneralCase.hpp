#ifndef Cadabra_Queen_Core_Test_Case_GeneralCase_hpp
#define Cadabra_Queen_Core_Test_Case_GeneralCase_hpp

#include "../AbstractCase.hpp"

#include <QPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class GeneralCase : public AbstractCase
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		GeneralCase(QObject* parent = nullptr);
		virtual ~GeneralCase() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QPointer<QObject> _unit_o;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool run() override;

	protected:
		virtual bool dial(const QQmlIncubator& incubator) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	protected:
		const QPointer<QObject>& getObject() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		virtual void initTestCase() override;
		virtual void initTestCase_data() override;
		virtual void cleanupTestCase() override;
		virtual void init() override;
		virtual void cleanup() override;
	/** @} */
	};
} } } }

#endif
