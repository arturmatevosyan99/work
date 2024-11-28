#include "Directory.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Directory::Directory(QObject* parent)
	:
		AbstractApplication(parent)
	{
	}

	Directory::~Directory()
	{
	}

	Core::Net::CRPPromise Directory::index()
	{
		return FolderInterface::index();
	}

	QAbstractListModel* Directory::getIndexModel() const
	{
		return _index_model.get();
	}

	void Directory::onIndexed(const FolderInterface::Map& map)
	{
		_index_model->remap(map);
		emit indexed();
	}

	bool Directory::onHandle()
	{
		return true;
	}

	bool Directory::onOpen()
	{
		_index_model.reset(new Meta::FolderIndex());
		return true;
	}

	bool Directory::onSetup()
	{
		return true;
	}

	void Directory::customEvent(QEvent* event)
	{

	}
} } } }
