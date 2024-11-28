/**
 @file Channel.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Channel_hpp
#define Cadabra_Queen_Repository_Meta_Channel_hpp

#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	class Channel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Role : qint16
		{
			NonGrata = 0,
			Listener = 1,
			Voice = 2,
			Moderator = 3,
			Operator = 4,
			Owner = 5,
		};
	/** @} */
		
	/** @name Constructors */
	/** @{ */
	public:
	/** @} */
		
	/** @name Factories */
	/** @{ */
	public:
	/** @} */
		
	/** @name Operators */
	/** @{ */
	public:
	/** @} */
		
	/** @name Properties */
	/** @{ */
	public:		
	/** @} */
		
	/** @name Procedures  */
	/** @{ */
	protected:
	/** @} */
		
	/** @name Converters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Getters */
	/** @{ */
	public:
	/** @} */
		
	/** @name Formatters */
	/** @{ */
	public:
	/** @} */
		
	/** @name States */
	/** @{ */
	public:
	/** @} */
		
	/** @name Hooks */
	/** @{ */
	public:
	/** @} */
		
	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
