/**
 @file MessageRender.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "MessageRender.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	MessageRender::MessageRender()
	:
		Core::Meta::RenderScheme()
	{

	}

	MessageRender::MessageRender(const QString& extname, int version_major, int version_minor, const QString& filename, const QFlags<RenderLayout>& layout_set, const QString& artefactname)
	:
		Core::Meta::RenderScheme("View/Message", version_major, version_minor, extname, filename, filename),

		_layout_set(layout_set),
		_artefactname(artefactname.isEmpty() ? filename.trimmed() : artefactname)
	{

	}

	MessageRender::MessageRender(const MessageRender& origin)
	:
		Core::Meta::RenderScheme(origin),

		_layout_set(origin._layout_set),
		_artefactname(origin._artefactname)
	{

	}

	MessageRender::MessageRender(MessageRender&& origin)
	:
		Core::Meta::RenderScheme(origin),

		_layout_set(std::move(origin._layout_set)),
		_artefactname(std::move(origin._artefactname))
	{

	}

	bool MessageRender::load(const Cadabra::Queen::Core::Meta::Qd& qd)
	{
		Core::Meta::Scheme<MessageRender>::Iterator i(Core::Meta::Scheme<MessageRender>::Iterate(qd));
		if (!Core::Meta::Scheme<MessageRender>::Exists(i)) {
			return false;
		}
		operator=(i.value());
		return true;
	}

	QUrl MessageRender::makeURL(Cadabra::Queen::Conversation::RenderLayout layout) const
	{
		if (RenderLayout::FeedLayout == layout) {
			return RenderScheme::makeURL();
		} else if (!_layout_set.testFlag(layout)) {
			return QUrl();
		}
		return RenderScheme::makeURL(QString(".%1").arg(static_cast<int>(layout)));
	}

	MessageRender& MessageRender::operator=(const MessageRender& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	MessageRender& MessageRender::operator=(MessageRender&& origin)
	{
		RenderScheme::operator=(origin);
		return *this;
	}

	QHash<int, QByteArray> MessageRender::roleNames() const
	{
		QHash<int, QByteArray> retval(RenderScheme::roleNames());
		retval[Role::LayoutSet] = "layout_set";
		retval[Role::ArtefactName] = "artefactname";

		return retval;
	}

	QVariant MessageRender::data(int role) const
	{
		switch (role) {
			case Role::LayoutSet: return static_cast<int>(_layout_set);
			case Role::ArtefactName: return _artefactname;

			default: return RenderScheme::data(role);
		}
	}

	const QFlags<RenderLayout>& MessageRender::getLayoutSet() const
	{
		return _layout_set;
	}

	const QString& MessageRender::getArtefactName() const
	{
		return _artefactname;
	}
} } } }
