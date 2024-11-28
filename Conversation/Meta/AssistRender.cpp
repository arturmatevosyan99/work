/**
 @file AssistRender.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "AssistRender.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	AssistRender::AssistRender()
	:
		Core::Meta::RenderScheme()
	{

	}

	AssistRender::AssistRender(const QString& extname, int version_major, int version_minor, const QString& filename)
	:
		Core::Meta::RenderScheme("View/Assist", version_major, version_minor, extname, filename, filename)
	{

	}

	AssistRender::AssistRender(const AssistRender& origin)
	:
		Core::Meta::RenderScheme(origin)
	{

	}

	AssistRender::AssistRender(AssistRender&& origin)
	:
		Core::Meta::RenderScheme(origin)
	{

	}

	bool AssistRender::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<AssistRender>::Iterator i(Core::Meta::Scheme<AssistRender>::Iterate(qd));
		if (!Core::Meta::Scheme<AssistRender>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}

	AssistRender& AssistRender::operator=(const AssistRender& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	AssistRender& AssistRender::operator=(AssistRender&& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}
} } } }
