#ifndef Cadabra_Queen_Core_Net_Proto_Scheme_hpp
#define Cadabra_Queen_Core_Net_Proto_Scheme_hpp

#include <cstdint>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	enum class Scheme : char
	{
		HTTP = 'h',
		HTTPS = 'H',
		WS = 'w',
		WSS = 'W'
	};
} } } }

#endif
