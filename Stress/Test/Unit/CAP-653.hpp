#ifndef Cadabra_Queen_Stress_Test_Case_CAP_653_hpp
#define Cadabra_Queen_Stress_Test_Case_CAP_653_hpp

#include "../../../Core/Test/AbstractCase.hpp"

#include "../../../Core/Data/AbstractEntity.hpp"

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	/**
	 * @details
	 * Does ЁЖ look alike ёж?
	 */
	class CAP653 : public Core::Test::AbstractCase
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CAP653(QObject* parent = nullptr);
		virtual ~CAP653() override = default;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool run() override;
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

	class Animal : public Core::Data::AbstractEntity
	{
	/** @name Constructors */
	/** @{ */
	public:
		Animal();
		virtual ~Animal() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	public:
		Field name;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual void onSelect() override;
		virtual void onCreate() override;
		virtual void onForeign() override;
	/** @} */
	};
} } } }

#endif
