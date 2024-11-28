#ifndef Cadabra_Queen_Core_Test_Node_Shock_hpp
#define Cadabra_Queen_Core_Test_Node_Shock_hpp

#include "Action.hpp"

#include "../AbstractShocker.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Shock : public Action
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Test::AbstractShocker* shocker READ getShocker WRITE setShocker NOTIFY shockerChanged)
		Q_PROPERTY(bool onoff MEMBER _onoff)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Shock(const QString& description = "Empty shock description");
		virtual ~Shock() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		AbstractShocker* _shocker;
		bool _onoff;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		AbstractShocker* getShocker() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setShocker(AbstractShocker* value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void shockerChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onActed();
	/** @} */
	};
} } } }

#endif
