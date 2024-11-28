/**
 @file TrackRender.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "TrackRender.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Meta {
	TrackRender::TrackRender()
	:
		Core::Meta::RenderScheme()
	{

	}

	TrackRender::TrackRender(const QString& extname, int version_major, int version_minor, const QString& filename, const QFlags<TrackRenderLayout>& layout_set, const QString& artefactname)
	:
		Core::Meta::RenderScheme("View/Track", version_major, version_minor, extname, filename, filename),

		_layout_set(layout_set),
		_artefactname(artefactname.isEmpty() ? filename.trimmed() : artefactname)
	{

	}

	TrackRender::TrackRender(const TrackRender& origin)
	:
		Core::Meta::RenderScheme(origin),

		_layout_set(origin._layout_set),
		_artefactname(origin._artefactname)
	{

	}

	TrackRender::TrackRender(TrackRender&& origin)
	:
		Core::Meta::RenderScheme(origin),

		_layout_set(std::move(origin._layout_set)),
		_artefactname(std::move(origin._artefactname))
	{

	}

	bool TrackRender::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<TrackRender>::Iterator i(Core::Meta::Scheme<TrackRender>::Iterate(qd));
		if (!Core::Meta::Scheme<TrackRender>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}

	QUrl TrackRender::makeURL(Cadabra::Queen::Mentalgami::TrackRenderLayout layout) const
	{
		if (TrackRenderLayout::PlaylistLayout == layout) {
			return RenderScheme::makeURL();
		} else if (!_layout_set.testFlag(layout)) {
			return QUrl();
		}
		return RenderScheme::makeURL(QString(".%1").arg(static_cast<int>(layout)));
	}

	TrackRender& TrackRender::operator=(const TrackRender& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	TrackRender& TrackRender::operator=(TrackRender&& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	QHash<int, QByteArray> TrackRender::roleNames() const
	{
		QHash<int, QByteArray> retval(RenderScheme::roleNames());
		retval[Role::LayoutSet] = "layout_set";
		retval[Role::ArtefactName] = "artefactname";

		return retval;
	}

	QVariant TrackRender::data(int role) const
	{
		switch (role) {
			case Role::LayoutSet: return static_cast<int>(_layout_set);
			case Role::ArtefactName: return _artefactname;

			default: return RenderScheme::data(role);
		}
	}

	const QFlags<TrackRenderLayout>& TrackRender::getLayoutSet() const
	{
		return _layout_set;
	}

	const QString& TrackRender::getArtefactName() const
	{
		return _artefactname;
	}
} } } }
