/**
 @file Session.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_Session_hpp
#define Cadabra_Queen_Repository_Meta_Session_hpp

#include "Essence.hpp"

#include <QtGlobal>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Session
	{
	/** @name Classes */
	/** @{ */
	public:
		using Id = SubjectId;
		using Handle = Essence::Handle;
		using Pool = SessionPool;

		enum class SubscriptionKey : std::uint8_t {
			Global = '0',
			ObjectId = 'o',
			ObjectHandle = 'O'
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
