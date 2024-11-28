/**
 @file ApplicationRender.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "ApplicationRender.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	ApplicationRender::ApplicationRender()
	:
		Core::Meta::RenderScheme()
	{

	}

	ApplicationRender::ApplicationRender(const QString& extname, const QString& modelname, int version_major, int version_minor, const QString& filename)
	:
		Core::Meta::RenderScheme("View/Application", version_major, version_minor, extname, modelname, filename)
	{

	}

	ApplicationRender::ApplicationRender(const ApplicationRender& origin)
	:
		Core::Meta::RenderScheme(origin)
	{

	}

	ApplicationRender::ApplicationRender(ApplicationRender&& origin)
	:
		Core::Meta::RenderScheme(origin)
	{

	}

	QUrl ApplicationRender::makeIconURL() const
	{
		return QUrl(QString("qrc:/%1/Asset/Icon/%2.svg").arg(getModuleName(), getModelName()));
	}

	ApplicationRender& ApplicationRender::operator=(const ApplicationRender& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	ApplicationRender& ApplicationRender::operator=(ApplicationRender&& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	bool ApplicationRender::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<ApplicationRender>::Iterator i(Core::Meta::Scheme<ApplicationRender>::Iterate(qd));
		if (!Core::Meta::Scheme<ApplicationRender>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}

	QHash<int, QByteArray> ApplicationRender::roleNames() const
	{
		QHash<int, QByteArray> retval(RenderScheme::roleNames());
		retval[Role::IconURL] = "icon_url";

		return retval;
	}

	QVariant ApplicationRender::data(int role) const
	{
		switch (role) {
			case Role::IconURL: return makeIconURL();
			default: return RenderScheme::data(role);
		}
	}
} } } }
