#include "SmartBook.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {

SmartBook::SmartBook(QObject* parent)
	: Repository::Model::AbstractApplication(parent),
	  _index_model(new BookIndex(this))
{
}

SmartBook::~SmartBook()
{
}

void SmartBook::setSource(const QUrl& url)
{
	if (_index_model) {
		_index_model->open(url);
	}
}

QAbstractItemModel* SmartBook::getIndexModel() const
{
	return _index_model.data();
}

bool SmartBook::onOpen()
{
	return true;
}

bool SmartBook::onClose()
{
	return true;
}

}}}} // namespace
