#include "PlaylistInterface.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	Playmeta::operator QJsonObject() const
	{
		return {
				{"title", title},
				{"description", description},
				{"author", author},
				{"copyright", copyright},
				{"artwork", artwork},
				{"revision_code", revision_code},
				{"language_code", language_code},
				{"release_datetime", release_datetime.toSecsSinceEpoch()},
				{"count", count},
				{"key", key},
				{"map", QJsonObject::fromVariantMap(map)}
		};
	}

	Track::operator QJsonObject() const
	{
		QJsonObject js_meta_map(QJsonObject::fromVariantMap(meta_map));
		js_meta_map["title"] = title;
		js_meta_map["description"] = description;
		js_meta_map["weight"] = weight;
		js_meta_map["content_type"] = content_type;
		js_meta_map["content_length"] = content_length;
		js_meta_map["content_size"] = content_size;

		return {
				{"key", key},
//				{"flagset", flagset},
				{"description", description},

				{"file_timestamp", file_datetime.toSecsSinceEpoch()},

				{"progress_datetime", progress_datetime.toSecsSinceEpoch()},
				{"progress", progress},

				{"meta_datetime", file_datetime.toSecsSinceEpoch()},
				{"meta", js_meta_map}
		};
	}

	Playmeta PlaylistInterface::OverviewSummary(const QJsonObject& input)
	{
		Playmeta retval;

		retval.title = input.value("title").toString("");
		retval.description = input.value("description").toString("");
		retval.author = input.value("author").toString("");
		retval.copyright = input.value("copyright").toString("");
		retval.artwork = input.value("artwork").toString("");
		retval.revision_code = input.value("revision_code").toInt(0);
		retval.language_code = input.value("language_code").toInt(0);
		retval.release_datetime = QDateTime::fromSecsSinceEpoch(input.value("release_timestamp").toInt(0), Qt::UTC);
		retval.count = input.value("count").toInt(0);
		retval.key = input.value("key").toString("");
		retval.map = input.value("map").toObject({}).toVariantMap();
		retval.map["over_view_time"] = QDateTime::currentSecsSinceEpoch();

		return retval;
	}

	QVector<Track> PlaylistInterface::IndexSequence(const QJsonArray& input)
	{
		QVector<Track> retval;
		for (const QJsonValue& value : input) {
			if (!value.isObject()) {
				continue;
			}

			retval.push_back({});
			Track& track(retval.back());

			const QJsonObject object(value.toObject());
			track.key = object.value("key").toString();

			const QJsonArray flagset(object.value("flagset").toArray({}));
			for (QJsonArray::ConstIterator f = flagset.constBegin(); f != flagset.constEnd(); ++f) {
				track.flagset.insert(static_cast<Track::Flag>(static_cast<char>(f->toInt('d'))));
			}

			track.file_datetime = QDateTime::fromSecsSinceEpoch(object.value("file_timestamp").toInt(0), Qt::UTC);

			track.progress_datetime = QDateTime::fromSecsSinceEpoch(object.value("progress_timestamp").toInt(0), Qt::UTC);
			track.progress = object.value("progress").toDouble(0.0);

			QJsonObject meta_object(object.value("meta").toObject());
			track.title = meta_object.take("title").toString("");
			track.description = meta_object.take("description").toString("");
			track.weight = meta_object.take("weight").toInt(0);
			track.artwork = meta_object.take("artwork").toString("");
			track.content_type = meta_object.take("content_type").toString(0);
			track.content_length = meta_object.take("content_length").toInt(0);
			track.content_size = meta_object.take("content_size").toInt(0);

			track.meta_datetime = QDateTime::fromSecsSinceEpoch(object.value("meta_timestamp").toInt(0), Qt::UTC);
			track.meta_map = meta_object.toVariantMap();
		}
		return retval;
	}

	Core::Net::CRPDelay PlaylistInterface::overviewSummary()
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::PlaylistInterface",
			"Cadabra::Ace::Repository::Model::SummaryOverview",
			{
			}
		).detach(this, &PlaylistInterface::onSummaryOverview);
	}

	Core::Net::CRPDelay PlaylistInterface::indexSequence(const QString& key, qint32 limit)
	{
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::PlaylistInterface",
			"Cadabra::Ace::Repository::Model::SequenceIndex",
			{
				{"key", key},
				{"limit", 10}
			}
		).detach(this, &PlaylistInterface::onSequenceIndex);
	}

	Core::Net::CRPDelay PlaylistInterface::pausePlayback(const QMap<QString, double>& map, const QDateTime& datetime)
	{
		QJsonObject js_map;
		for (QMap<QString, double>::ConstIterator m = map.constBegin(); m != map.constEnd(); ++m) {
			js_map.insert(m.key(), m.value());
		}
		return Core::Net::CRPDelay
		(
			makeDeferredCall(),
			"Cadabra::Ace::Repository::Model::PlaylistInterface",
			"Cadabra::Ace::Repository::Model::PlaybackPause",
			{
				{"map", js_map},
				{"timestamp", datetime.isValid() ? datetime.toSecsSinceEpoch() : 0} /// @??? MSecsSinceEpoch
			}
		).detach(this, &PlaylistInterface::onPlaybackPause);
	}

	Core::Net::CRPDelay PlaylistInterface::pausePlayback(const QString& key, double progress, const QDateTime& datetime)
	{
		return pausePlayback({{key, progress}}, datetime);
	}

	void PlaylistInterface::onSummaryOverview(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return onSummaryOverviewed(reaction.getCode(), reaction.getDomain(), reaction.getMessage());
		} else if (!(*reaction).isObject()) {
			return onSummaryOverviewed(0, 5, "Payload is not an object");
		}
		onSummaryOverviewed(OverviewSummary((*reaction).toObject()));
	}
	void PlaylistInterface::onSummaryOverviewed(quint8 domain, quint32 code, const QString& message)
	{

	}
	void PlaylistInterface::onSummaryOverviewed(const Playmeta& data)
	{

	}

	void PlaylistInterface::onSequenceIndex(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return onSequenceIndexed(reaction.getCode(), reaction.getDomain(), reaction.getMessage());
		} else if (!(*reaction).isArray()) {
			return onSequenceIndexed(0, 5, "Payload is not an array");
		}
		onSequenceIndexed(IndexSequence((*reaction).toArray()));
	}
	void PlaylistInterface::onSequenceIndexed(quint8 domain, quint32 code, const QString& message)
	{

	}
	void PlaylistInterface::onSequenceIndexed(const QVector<Track>& tack_vector)
	{

	}

	void PlaylistInterface::onPlaybackPause(const Core::Net::CRPReaction& reaction)
	{
		if (!reaction) {
			return onPlaybackPaused(reaction.getCode(), reaction.getDomain(), reaction.getMessage());
		} else if (!(*reaction).isArray()) {
			return onPlaybackPaused(0, 5, "Payload is not an array");
		}
		onPlaybackPaused(IndexSequence((*reaction).toArray()));
	}
	void PlaylistInterface::onPlaybackPaused(quint8 domain, quint32 code, const QString& message)
	{

	}
	void PlaylistInterface::onPlaybackPaused(const QVector<Track>& tack_vector)
	{

	}
} } } }
