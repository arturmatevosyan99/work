/**
 @file AbstractPatch.cpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "AbstractPatch.hpp"

#include "AbstractMap.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	AbstractPatch::AbstractPatch()
	:
		QObject(nullptr),

		_target_map(nullptr),

		_log_stamp_range({{QString(""), QString("")}, {QString(""), QString("")}}),
		_tag_id_range({0, 0})
	{

	}

	AbstractPatch::~AbstractPatch()
	{

	}

	bool AbstractPatch::apply()
	{
		if (!_target_map || _stash_map.isEmpty()) {
			return false;
		}

		/// @todo Разумно в рамках MapObserver открывать транзакцию и фиксировать изменения вместе с Data::Sync
		const QScopedPointer<MapObserver> observer(new MapObserver(_target_map));
		StashMap::Iterator i(_stash_map.upperBound(0));
		if (_stash_map.end() == i) {
			_stash_map.clear();
			return true;
		} do {
			const QDateTime modification_timestamp
			(
				QDateTime::fromString
				(
					i.value().log_stamp.second.isEmpty()
					? "0000-00-00 00:00:00.000 UTC"
					: i.value().log_stamp.second.chopped(3).append(" UTC"),
					"yyyy-MM-dd hh:mm:ss.zzz t"
				)
			);
			if (i.value().property_map.value("_").toBool(false)) {
				if (!_target_map->remove(i.key())) {
					_error_list.push_back(tr("Could not remove target from hierarchy, realm is probably broken"));
					return !observer->getMap().isEmpty();
				}
			} else if (!_target_map->apply(i.key(), modification_timestamp, i.value().property_map)) {
				_error_list.push_back(tr("Could not apply log to hierarchy, realm is probably broken"));
				return !observer->getMap().isEmpty();
			}

			for (QMap<Meta::Essence::Id, bool>::Iterator t = i.value().tag_map.begin(); t != i.value().tag_map.end(); t = i.value().tag_map.erase(t)) {
				if (!_target_map->tag(i.key(), t.key(), t.value())) {
					_error_list.push_back(tr("Could not tag target in hierarchy during log processing, roaster is probably broken"));
					return !observer->getMap().isEmpty();
				}
			}
		} while (_stash_map.end() != (i = _stash_map.erase(i)));
		_stash_map.remove(0); /// < Не определились, нужны ли субъект dummy и объект dummy
		return !observer->getMap().isEmpty();
	}

	bool AbstractPatch::unmap()
	{
		if (_target_map)  {
			_target_map = nullptr;
			return true;
		}
		return false;
	}

	void AbstractPatch::clear()
	{
		_error_list.clear();
		_stash_map.clear();
		_log_stamp_range = {{QString(""), QString("")}, {QString(""), QString("")}};
		_tag_id_range = {0, 0};
	}

	AbstractPatch::Resolution AbstractPatch::stash(const QJsonArray& array)
	{
		using ResolutionType = std::underlying_type<Resolution>::type;
		ResolutionType retval(static_cast<ResolutionType>(Resolution::Unknown));
		foreach (const QJsonValue& v, array) {
			if (v.isObject()) {
				retval |= static_cast<ResolutionType>(stash(v.toObject()));
			}
		}
		return static_cast<Resolution>(retval);
	}

	bool AbstractPatch::flush(Meta::Essence::Id id, LogStamp log_stamp)
	{
		if (!_stash_map.isEmpty()) {
			return false;
		}
		_stash_map[id] = {log_stamp, {}, {}, true};
		return true;
	}

	bool AbstractPatch::map(AbstractMap* target_map)
	{
		if (!target_map) {
			return false;
		}
		_target_map = target_map;
		return true;
	}

	AbstractPatch::Diff& AbstractPatch::target(Meta::Essence::Id id, const LogStamp& log_stamp)
	{
		Diff& retval(_stash_map[id]);
		if (retval.log_stamp.second > log_stamp.second || id < 0) {
			retval.ignore = true;
		} else {
			retval.ignore = false;
			retval.log_stamp = log_stamp;
			if (log_stamp.second.compare(_log_stamp_range.second.second) > 0) {
				_log_stamp_range.second = log_stamp;
			}
		}
		return retval;
	}

	AbstractPatch::Resolution AbstractPatch::stash(Meta::Essence::Id id, const LogStamp& logstamp, const QJsonArray& tag_array)
	{
		Diff& diff(target(id, logstamp));
		if (diff.ignore) {
			return Resolution::Ignore;
		}
		foreach (const QJsonValue& tag, tag_array) {
			Meta::Essence::Id tag_id(Meta::Essence::ToId(tag));
			if (tag_id > 0) {
				diff.tag_map[tag_id] = true;
			} else {
				tag_id = qAbs(tag_id);
				diff.tag_map[tag_id] = false;
			}
		}
		_tag_id_range.first = (0 == _tag_id_range.first) ? diff.tag_map.firstKey() : qMin(_tag_id_range.first, diff.tag_map.firstKey());
		_tag_id_range.second = qMax(_tag_id_range.second, diff.tag_map.lastKey());
		return Resolution::Accept;
	}

	AbstractPatch::Resolution AbstractPatch::stash(const LogStamp& logstamp, const QJsonObject& object, const QJsonValue& meta)
	{
		return Resolution::Ignore;
	}

	AbstractPatch::Resolution AbstractPatch::stash(const QJsonObject& object)
	{
		const Meta::Essence::Impact impact(static_cast<Meta::Essence::Impact>(object.value("impact").toInt(static_cast<int>(Meta::Essence::Impact::Error))));
		if (Meta::Essence::Impact::Success != impact) {
			return Resolution::Ignore;
		}

		const LogStamp log_stamp(object.value("acestamp").toString("0000x0000000000000000"), object.value("timestamp").toString("0000-00-00 00:00:00.000000"));
		const int log_stamp_compare(log_stamp.second.compare(_log_stamp_range.second.second));
		if (log_stamp_compare < 0) { /// < Меньше отсечки
			return Resolution::Obsolete;
		} else if (log_stamp_compare == 0 && log_stamp.first.compare(_log_stamp_range.second.first) == 0) { /// < Такая же отсчечка и такой же туз
			return Resolution::Duplicate;
		}
		return stash(log_stamp, object, object.value("meta"));
	}

	Meta::Essence::Id AbstractPatch::getLastId() const
	{
		return _stash_map.isEmpty() ? 0 : _stash_map.lastKey();
	}

	const AbstractPatch::LogStamp& AbstractPatch::getLogStampOffset() const
	{
		return _log_stamp_range.second;
	}

	const QPair<AbstractPatch::LogStamp, AbstractPatch::LogStamp>& AbstractPatch::getLogStampRange() const
	{
		return _log_stamp_range;
	}

	const QPair<Meta::Essence::Id, Meta::Essence::Id>& AbstractPatch::getTagIdRange() const
	{
		return _tag_id_range;
	}

	const QStringList& AbstractPatch::getErrorList() const
	{
		return _error_list;
	}

	void AbstractPatch::setLogStampOffset(const LogStamp& log_stamp)
	{
		_log_stamp_range.first = log_stamp;
		_log_stamp_range.second = log_stamp;
	}

	bool AbstractPatch::hasMap() const
	{
		return nullptr != _target_map;
	}

	bool AbstractPatch::hasErrors() const
	{
		return !_error_list.isEmpty();
	}

	bool AbstractPatch::hasStash() const
	{
		return !_stash_map.isEmpty();
	}

	bool AbstractPatch::hasLogStampOffset() const
	{
		return !_log_stamp_range.second.first.isEmpty() && !_log_stamp_range.second.first.isEmpty();
	}
} } } }
