#include "mg_RiddleRender.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Meta {
	RiddleRender::RiddleRender()
	:
		Core::Meta::RenderScheme()
	{

	}

	RiddleRender::RiddleRender(const QString& extname, int version_major, int version_minor, const QString& filename)
	:
		Core::Meta::RenderScheme("View/Riddle", version_major, version_minor, extname, filename, filename)
	{

	}

	RiddleRender::RiddleRender(const RiddleRender& origin)
	:
		Core::Meta::RenderScheme(origin)
	{

	}

	RiddleRender::RiddleRender(RiddleRender&& origin)
	:
		Core::Meta::RenderScheme(origin)
	{

	}

	bool RiddleRender::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<RiddleRender>::Iterator i(Core::Meta::Scheme<RiddleRender>::Iterate(qd));

		if (!Core::Meta::Scheme<RiddleRender>::Exists(i)) {
			return false;
		}

		operator=(i.value());
		return true;
	}

	RiddleRender& RiddleRender::operator=(const RiddleRender& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	RiddleRender& RiddleRender::operator=(RiddleRender&& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}
} } } }
