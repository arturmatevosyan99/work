#include "ITunesItem.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	ITunesItem::ITunesItem()
	:
		_id(0)
	{

	}

	ITunesItem::ITunesItem(int id, const QString& ident, const QString& censored_name, const QString& track_name, const QString& description, const QString& version)
	:
		_id(id),
		_ident(ident),
		_censored_name(censored_name),
		_track_name(track_name),
		_description(description),
		_version(version)
	{

	}

	ITunesItem::ITunesItem(const ITunesItem& origin)
	:
		_id(origin._id),
		_ident(origin._ident),
		_censored_name(origin._censored_name),
		_track_name(origin._track_name),
		_description(origin._description),
		_version(origin._version)
	{

	}

	ITunesItem::ITunesItem(ITunesItem&& origin)
	:
		_id(std::move(origin._id)),
		_ident(std::move(origin._ident)),
		_censored_name(std::move(origin._censored_name)),
		_track_name(std::move(origin._track_name)),
		_description(std::move(origin._description)),
		_version(std::move(origin._version))
	{

	}

	ITunesItem::~ITunesItem()
	{

	}

	ITunesItem& ITunesItem::operator=(const ITunesItem& rhs)
	{
		_id = rhs._id;
		_ident = rhs._ident;
		_censored_name = rhs._censored_name;
		_track_name = rhs._track_name;
		_description = rhs._description;
		_version = rhs._version;

		return *this;
	}

	ITunesItem& ITunesItem::operator=(ITunesItem&& rhs)
	{
		_id = std::move(rhs._id);
		_ident = std::move(rhs._ident);
		_censored_name = std::move(rhs._censored_name);
		_track_name = std::move(rhs._track_name);
		_description = std::move(rhs._description);
		_version = std::move(rhs._version);

		return *this;
	}

	const int& ITunesItem::getId() const
	{
		return _id;
	}

	const QString& ITunesItem::getIdent() const
	{
		return _ident;
	}

	const QString& ITunesItem::getCensoredName() const
	{
		return _censored_name;
	}

	const QString& ITunesItem::getTrackName() const
	{
		return _track_name;
	}

	const QString& ITunesItem::getDescription() const
	{
		return _description;
	}

	const QString& ITunesItem::getVersion() const
	{
		return _version;
	}

	bool ITunesItem::isZeroVersion() const
	{
		return _version.isEmpty() || _version.compare("0.0.0") == 0;
	}

	bool ITunesItem::isHigher(const ITunesItem& rhs) const
	{
		return (_id == rhs.getId() || _ident == rhs.getIdent()) && isHigher(rhs.getVersion());
	}

	bool ITunesItem::isHigher(const QString& rhs) const
	{
		if (rhs.isEmpty() || isZeroVersion()) {
			return false;
		}

		const QStringList lhs_version_list(_version.split(".")), rhs_version_list(rhs.split("."));
		if (lhs_version_list.count() < 3 || rhs_version_list.count() < 3) {
			return false;
		}
		const int rhs_offset(rhs_version_list.count() - 3);
		for (int v = 0; v < 3; ++v) {
			bool ok(true);

			const uint l(lhs_version_list.at(v).toUInt(&ok));
			if (!ok) return false;
			const uint r(rhs_version_list.at(v + rhs_offset).toUInt(&ok));
			if (!ok) return false;

			if (l > r) {
				return true;
			} else if (l < r) {
				return false;
			}
		}
		return false;
	}
} } } }
