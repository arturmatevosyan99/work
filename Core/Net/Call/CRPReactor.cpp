#include "CRPReactor.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	CRPReactor::CRPReactor()
	{

	}

	CRPReactor::~CRPReactor()
	{

	}

	CRPFunctor::CRPFunctor(Type&& functor)
	:
		_functor(functor)
	{

	}
	CRPFunctor::~CRPFunctor()
	{

	}

	void CRPFunctor::callback(const CRPReaction& reaction) const
	{
		if (_functor) {
			_functor(reaction);
		}
	}
} } } }
