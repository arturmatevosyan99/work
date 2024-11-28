#ifndef Cadabra_Queen_Core_Data_Event_hpp
#define Cadabra_Queen_Core_Data_Event_hpp

#include "../../Core/Model/Event.hpp"
#include "AbstractCommand.hpp"

#include <QEvent>
#include <QMetaType>
#include <QSharedPointer>
#include <type_traits>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class Event : public Core::Model::Event<AbstractCommand>
	{
	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Event(const QString& name);
		using Core::Model::Event<AbstractCommand>::Event;
		Event(const Event& origin);
		virtual ~Event() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
