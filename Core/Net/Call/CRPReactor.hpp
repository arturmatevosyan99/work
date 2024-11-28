/**
 @file CRPReactor.hpp
 @date 2014-01-01
 @copyright Cadabra Systems
 @author Daniil A Megrabyan <daniil@megrabyan.pro>
*/

#ifndef Cadabra_Queen_Core_Net_Call_CRPReactor_hpp
#define Cadabra_Queen_Core_Net_Call_CRPReactor_hpp

#include <QObject>

#include "CRPReaction.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CRPReactor : public QObject
	{
	/** @name Constructors */
	/** @{ */
	public:
		CRPReactor();
		virtual ~CRPReactor();
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void callback(const CRPReaction&) const = 0;
	/** @} */
	};

	class CRPFunctor : public CRPReactor
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Type = std::function<void(const CRPReaction&)>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CRPFunctor(Type&& functor);
		virtual ~CRPFunctor();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const Type _functor;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void callback(const CRPReaction& reaction) const override;
	/** @} */
	};

	template <typename I>
	class CRPInteractor : public CRPReactor
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Interface = I;
		using Method = void (Interface::*)(const CRPReaction&);
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		CRPInteractor(Interface* object, Method method)
		:
			_object(object),
			_method(method)
		{

		}
		virtual ~CRPInteractor()
		{

		}
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Interface* const _object;
		const Method _method;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void callback(const CRPReaction& reaction) const override
		{
			if (_object) {
				((*_object).*(_method))(reaction);
			}
		}
	/** @} */
	};
} } } }

#endif
