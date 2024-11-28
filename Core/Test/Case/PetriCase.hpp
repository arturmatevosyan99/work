#ifndef Cadabra_Queen_Core_Test_Case_PetriCase_hpp
#define Cadabra_Queen_Core_Test_Case_PetriCase_hpp

#include "../AbstractCase.hpp"

#include "../PetrI.hpp"

#include <QPointer>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class PetriCase : public AbstractCase
	{
	friend class Port;
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
		static PetrI* Origin(QObject* object);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PetriCase(QObject* parent = nullptr);
		virtual ~PetriCase() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QPointer<PetrI> _petr_i;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool run() override;

	protected:
		virtual bool dial(QQmlIncubator& incubator) override;
		virtual bool dial(const QQmlIncubator& incubator) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual const Context& getContext() const override;

	protected:
		virtual Context& getContext() override;

		const QPointer<PetrI>& getPetrI() const;
		/** @} */
	/** @} */

	/** @name Slots */
	/** @{ */
	signals:
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

	/** @name States */
	/** @{ */
	protected:
		bool hasInstance() const;
	/** @} */
	};
} } } }

#endif
