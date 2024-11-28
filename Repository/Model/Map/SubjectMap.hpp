#ifndef Cadabra_Queen_Repository_Model_Essence_SubjectMap_hpp
#define Cadabra_Queen_Repository_Model_Essence_SubjectMap_hpp

#include "../AbstractMap.hpp"

#include "../../Meta/Subject.hpp"
#include "../../Data/Entity/Record/Subject.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class SubjectMap : public AbstractMap
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Map = QMap<Meta::Subject::Id, QSharedPointer<Meta::Subject>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SubjectMap(QObject* parent = nullptr);
		virtual ~SubjectMap() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Map _map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool load(Roaster* roaster, bool lazy_mode = false) override;
		virtual bool unload() override;

		virtual bool apply(Meta::Essence::Id id, const QDateTime& modification_datetime, const QJsonObject& dump) override;
		virtual bool remove(Meta::Essence::Id id) override;

		virtual bool tag(Meta::Essence::Id id, Meta::Essence::Id tag_id, bool onoff = true) override;
		virtual bool opt(Meta::Essence::Id id, const QString& key, const QVariant& value) override;

		QSharedPointer<const Meta::Subject> upsert(Meta::Subject::Id id, Meta::Subject::AvatarScheme avatar_scheme, const QString& name, const QString& description, const QString& picture, const QString& locale, const QString& timezone, int creation_timestamp, int modification_timestamp);

	protected:
		virtual bool fetch(const QList<Meta::Essence::Id>& id_list) override;

	private:
		bool fetch(QSharedPointer<Data::Subject> data);

	public slots:
		virtual bool fetch(QSharedPointer<Core::Data::Rowset> row_set) override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QSharedPointer<Data::AbstractRecord> makeDataRecord() const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QSharedPointer<const Meta::Subject> getSubject(Meta::Subject::Id id);

		virtual int rowCount(const QModelIndex& parent) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QVariant data(const Meta::Essence::Id& id, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isExist(const Meta::Essence::Id& id) const override;
	/** @} */
	};

	class SubjectKey
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd id READ getId)
		Q_PROPERTY(QString displayId READ getDisplayId)
		Q_PROPERTY(QString avatarScheme READ getAvatarScheme)
		Q_PROPERTY(QString name READ getName)
		Q_PROPERTY(QString displayName READ getDisplayName)
		Q_PROPERTY(QString description READ getDescription)
		Q_PROPERTY(QString picture READ getPicture)
		Q_PROPERTY(QString locale READ getLocale)
		Q_PROPERTY(QString timezone READ getTimezone)
		Q_PROPERTY(QDateTime creationDateTime READ getCreationDateTime)
		Q_PROPERTY(QDateTime modificationDateTime READ getModificationDateTime)
		Q_PROPERTY(bool isValid READ isValid)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SubjectKey(QWeakPointer<const Meta::Subject>&& subject = QWeakPointer<const Meta::Subject>());
		SubjectKey(SubjectKey&& origin);
		SubjectKey(const SubjectKey& origin);
		~SubjectKey() = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QWeakPointer<const Meta::Subject> _subject;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		SubjectKey& operator=(SubjectKey&& origin);
		SubjectKey& operator=(const SubjectKey& origin);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Core::Meta::Qd getId() const;
		QString getDisplayId() const;
		QString getAvatarScheme() const;
		QString getName() const;
		QString getDisplayName() const;
		QString getDescription() const;
		QString getPicture() const;
		QString getLocale() const;
		QString getTimezone() const;
		QDateTime getCreationDateTime() const;
		QDateTime getModificationDateTime() const;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		bool isValid() const;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Model::SubjectKey)

#endif
