#include "HyperLink.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	HyperLink::HyperLink(QObject* parent)
	:
		AbstractAssist(-8034428181974876140, parent)
	{
	}

	QJsonObject HyperLink::exportBody() const
	{
		return QJsonObject{
							{"title", _title},
							{"target", _target.toString()}
		};
	}

	bool HyperLink::importBody(const QJsonObject& body)
	{
		setTitle(body.value("title").toString(tr("Follow the link")));
		setTarget(body.value("target").toString(""));
		return true;
	}

	const QString& HyperLink::getTitle() const
	{
		return _title;
	}

	const QUrl& HyperLink::getTarget() const
	{
		return _target;
	}

	Meta::Assist::TargetLayout HyperLink::getDefaultTargetLayout() const
	{
		return Meta::Assist::TargetLayout::FeedFooter;
	}

	QString HyperLink::getPreviewText() const
	{
		return _title;
	}

	void HyperLink::setTitle(const QString& title)
	{
		if (_title != title) {
			_title = title;
			emit titleChanged();
		}
	}

	void HyperLink::setTarget(const QUrl& target)
	{
		if (_target != target) {
			_target = target;
			emit targetChanged();
		}
	}
} } } }
