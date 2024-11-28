#ifndef Cadabra_Queen_Repository_Model_Hierarchy_hpp
#define Cadabra_Queen_Repository_Model_Hierarchy_hpp

#include <QAbstractItemModel>

#include "Roaster.hpp"
#include "Map/ObjectMap.hpp"
#include "Map/GroupMap.hpp"
#include "Map/SubjectMap.hpp"

#include "../Meta/Group.hpp"
#include "../Meta/Subject.hpp"
#include "../Meta/Object.hpp"

#include "../Net/Call/ObjectFile.hpp"

#include <QObject>
#include <QMap>
#include <QSet>
#include <QPair>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QMultiMap>
#include <QBuffer>
#include <QPair>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Realm; class Directory;
	class Hierarchy : public QAbstractItemModel
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using FileMap = QMap<QPair<Meta::Object::Id, QString>, QSharedPointer<Net::ObjectFile>>;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			EssenceTarget = Qt::UserRole,
			EssenceId,
			EssenceName,
			EssenceDescription,
			EssencePicture,
			EssenceCreationDateTime,
			EssenceModificationDateTime,
			EssenceMeta
		};

		enum class LoadMode: quint8
		{
			TimeOut = 0b00,
			CacheForce = 0b01,
			TimeLess = 0b10
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Hierarchy(const Realm* const realm);
		virtual ~Hierarchy() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const Realm* const _realm;
		Roaster _roaster;

		ObjectMap _object_model;
		GroupMap _group_model;
		SubjectMap _subject_model;
		QMap<Meta::Essence::Target, Meta::Essence::Id> _last_id_map;

		FileMap _file_map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		void disassemble();

	public:
		/**
		 * @brief assemblée
		 * @return
		 */
		bool assemble();

		QSharedPointer<Net::ObjectFile> loadObjectFile(Meta::Object::Id id, const QString& file_name, LoadMode mode = LoadMode::TimeOut);
		QSharedPointer<Net::ObjectFile> loadSubjectPicture(Meta::Subject::Id id, LoadMode mode = LoadMode::TimeOut);
		bool cacheObjectFile(Meta::Object::Id id, const QString& file_name, QBuffer* input);
		bool discardObjectFile(Meta::Object::Id id, const QString& file_name);

		//Q_INVOKABLE virtual void fetchMore(const QModelIndex& parent) override;
		//Q_INVOKABLE virtual bool canFetchMore(const QModelIndex& parent) const override;

		//?virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
		//?virtual QModelIndex buddy(const QModelIndex& index) const override;
		//?virtual QSize span(const QModelIndex& index) const override;
		//?Q_INVOKABLE virtual QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		qulonglong getExtensionRevision(const QString& name) const;

		Roaster& getRoaster();
		const Roaster& getRoaster() const;
		ObjectMap* getObjectModel();
		const ObjectMap* getObjectModel() const;
		SubjectMap* getSubjectModel();
		const SubjectMap* getSubjectModel() const;
		GroupMap* getGroupModel();
		const GroupMap* getGroupModel() const;

		const Meta::Essence::Id& getLastEssenceId(Meta::Essence::Target target, bool force = false);
		const Meta::Essence::Id& getLastObjectId(bool force = false);
		const Meta::Essence::Id& getLastSubjectId(bool force = false);
		const Meta::Essence::Id& getLastGroupId(bool force = false);

		QUrl getObjectCacheLocation(Meta::Object::Id id) const;
		QDir getObjectCachePath(Meta::Object::Id id, bool auto_mkdir = false) const;
		QUrl getObjectFileLocation(Meta::Object::Id id, const QString& file_name) const;

		Q_INVOKABLE Cadabra::Queen::Repository::Model::SubjectKey keySubject(Cadabra::Queen::Core::Meta::Qd qd);
//		Q_INVOKABLE Cadabra::Queen::Repository::Model::GroupKey keyGroup(Cadabra::Queen::Core::Meta::Qd qd);
//		Q_INVOKABLE Cadabra::Queen::Repository::Model::ObjectKey keyObject(Cadabra::Queen::Core::Meta::Qd qd);

		Q_INVOKABLE virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		Q_INVOKABLE virtual QModelIndex parent(const QModelIndex& child) const override;
		//Q_INVOKABLE virtual QModelIndex sibling(int row, int column, const QModelIndex& index) const override;

		Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

		//Q_INVOKABLE virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

		virtual QHash<int, QByteArray> roleNames() const override;
		//Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
		Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		bool upsert(Meta::Essence::Target target, Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump);
		bool tag(Meta::Essence::Target target, Meta::Essence::Id target_id, Meta::Essence::Id tag_id, bool onoff);
		bool remove(Meta::Essence::Target target, Meta::Essence::Id id);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void objectFileCached(Cadabra::Queen::Repository::Meta::Object::Id id, const QString& file_name, bool success);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onObjectFileLoaded(FileMap::iterator iterator, QNetworkReply::NetworkError error, QString message);
		void onObjectFileLoaded(FileMap::iterator iterator, QString filename);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasGroup(Meta::Group::Id id) const;
		bool hasSubject(Meta::Subject::Id id) const;
		bool hasObject(Meta::Object::Id id) const;
		bool hasObjectFile(Meta::Subject::Id id, const QString& file_name) const;
	/** @} */
	};
} } } }

#endif
