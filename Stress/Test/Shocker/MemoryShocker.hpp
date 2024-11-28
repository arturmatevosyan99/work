#ifndef Cadabra_Queen_Stress_Test_Shocker_MemoryShocker_hpp
#define Cadabra_Queen_Stress_Test_Shocker_MemoryShocker_hpp

#include "../../../Core/Test/AbstractShocker.hpp"

#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace Stress { namespace Test {
	class MemoryShocker : public Core::Test::AbstractShocker
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(int span READ getSpan WRITE setSpan NOTIFY spanChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		MemoryShocker(QObject* parent = nullptr);
		virtual ~MemoryShocker() override;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		int _span;
		char* _pointer;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		virtual bool turnOn() override;
		virtual bool turnOff() override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		int getSpan() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setSpan(int span);
	/** @} */

	/** @name Slots */
	/** @{ */
	signals:
		void spanChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
