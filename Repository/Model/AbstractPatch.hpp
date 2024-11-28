/**
 @file AbstractPatch.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Model_AbstractPatch_hpp
#define Cadabra_Queen_Repository_Model_AbstractPatch_hpp

#include "../Repository.hpp"
#include "../Meta/Essence.hpp"
#include "../Data/Entity/AbstractRecord.hpp"

#include <QMap>
#include <QSet>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class AbstractMap;

	class AbstractPatch : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum class Resolution : quint8
		{
			Unknown		= 0b00000,
			Ignore		= 0b00001,
			Obsolete	= 0b00010,
			Duplicate	= 0b00100,
			Malformed	= 0b01000,
			Accept		= 0b10000
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using AlterMap = QMultiMap<Meta::Essence::Id, EssenceAlter>;
		using LogStamp = QPair<QString, QString>;
	/** @} */

	/** @name Classes */
	/** @{ */
	protected:
		struct Diff
		{
			LogStamp log_stamp;
			QJsonObject property_map;
			QMap<Meta::Essence::Id, bool> tag_map;
			bool ignore;
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using StashMap = QMap<Meta::Essence::Id, Diff>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractPatch();
		virtual ~AbstractPatch();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		AbstractMap* _target_map;

		StashMap _stash_map;

		QPair<LogStamp, LogStamp> _log_stamp_range;
		QPair<Meta::Essence::Id, Meta::Essence::Id> _tag_id_range;

		QStringList _error_list;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool apply();
		bool unmap();
		void clear();
		bool flush(Meta::Essence::Id id, LogStamp log_stamp);
		Resolution stash(const QJsonArray& array);

	protected:
		bool map(AbstractMap* target_map);
		Diff& target(Meta::Essence::Id id, const LogStamp& log_stamp);
		Resolution stash(Meta::Essence::Id id, const LogStamp& logstamp, const QJsonArray& tag_array);
		virtual Resolution stash(const LogStamp& logstamp, const QJsonObject& object, const QJsonValue& meta);

	private:
		Resolution stash(const QJsonObject& object);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Meta::Essence::Id getLastId() const;
		const LogStamp& getLogStampOffset() const;
		const QPair<LogStamp, LogStamp>& getLogStampRange() const;
		const QPair<Meta::Essence::Id, Meta::Essence::Id>& getTagIdRange() const;
		const QStringList& getErrorList() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setLogStampOffset(const LogStamp& log_stamp);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasMap() const;
		bool hasErrors() const;
		bool hasStash() const;
		bool hasLogStampOffset() const;
	/** @} */
	};

	class ZilchPatch : public AbstractPatch
	{
	/** @name Constructors */
	/** @{ */
	public:
		ZilchPatch() = default;
		virtual ~ZilchPatch() override = default;
	/** @} */
	};
} } } }

#endif
