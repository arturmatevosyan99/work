#include "AbstractLayout.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	Cadabra::Queen::Conversation::Meta::Assist* AbstractLayout::Item(QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist>* list, int index)
	{
		AbstractLayout* assist = qobject_cast<AbstractLayout*>(list->object);
		return &(assist->_item_vector[index]);
	}

	int AbstractLayout::ItemListSize(QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist>* list)
	{
		AbstractLayout* assist = qobject_cast<AbstractLayout*>(list->object);
		return assist ? assist->_item_vector.count() : 0;
	}

	AbstractLayout::AbstractLayout(Meta::Assist::Scheme scheme, QObject* parent)
	:
		AbstractAssist(scheme, parent)
	{
	}

	QJsonObject AbstractLayout::exportBody() const
	{
		QJsonArray assits;
		for (const Meta::Assist& meta : _item_vector) {
			assits.push_back(static_cast<QJsonObject>(meta));
		}

		return QJsonObject{
							{"assists", assits}
		};
	}

	bool AbstractLayout::importBody(const QJsonObject& body)
	{
		_item_vector.clear();

		QJsonObject::ConstIterator o(body.find("assists"));
		if (body.constEnd() == o || !o->isArray()) {
			return false;
		}

		QJsonArray assist(o->toArray());
		for (QJsonArray::ConstIterator a = assist.constBegin(); a != assist.constEnd(); ++a) {
			_item_vector.push_back(Meta::Assist(a->toObject()));
			if (!_item_vector.back().isValid() || (!_item_vector.back().getAffectedSubjectIdSet().isEmpty() && !_item_vector.back().getAffectedSubjectIdSet().contains(getChannelApplication()->getDescriptor()->getSubjectId()))) {
				_item_vector.pop_back();
			}
		}

		return true;
	}

	Meta::Assist::TargetLayout AbstractLayout::getDefaultTargetLayout() const
	{
		return Meta::Assist::TargetLayout::FeedFooter;
	}

	QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist> AbstractLayout::makeItemList()
	{
		return QQmlListProperty<Cadabra::Queen::Conversation::Meta::Assist>(this, this, &AbstractLayout::ItemListSize, &AbstractLayout::Item);
	}
} } } }
