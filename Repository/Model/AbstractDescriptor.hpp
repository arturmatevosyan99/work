#ifndef Cadabra_Queen_Repository_Model_AbstractDescriptor_hpp
#define Cadabra_Queen_Repository_Model_AbstractDescriptor_hpp

#include "AceEvent.hpp"
#include "Realm.hpp"
#include "../Meta/Object.hpp"
#include "../../Core/Meta/Qd.hpp"

#include <QObject>
#include <QList>
#include <QPointer>
#include <QScopedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class AbstractDescriptor : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
//		Q_PROPERTY(Cadabra::Queen::Repository::Meta::ObjectKey objectKey READ getObjectKey CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::ObjectId objectId READ getObjectId CONSTANT)
		Q_PROPERTY(QString objectName READ getObjectName CONSTANT)
		Q_PROPERTY(QString objectSimplifiedName READ getObjectSimplifiedName CONSTANT)
		Q_PROPERTY(QUrl objectPicture READ getObjectPictureURL CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Object::ApplicationScheme objectApplicationScheme READ getObjectApplicationScheme CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::SubjectKey subjectKey READ getSubjectKey CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::SubjectId subjectId READ getSubjectId CONSTANT)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit AbstractDescriptor(Realm* realm, QSharedPointer<const Meta::Object> object);
		virtual ~AbstractDescriptor() = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QUrlQuery makeMoniker() const = 0;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QPointer<Realm> _realm;
		const QSharedPointer<const Meta::Object> _object;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE virtual QVariant opt(const QStringList& path) const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Realm* getRealm() const;

		const Meta::ObjectId& getObjectId() const;
		const QString& getObjectName() const;
		const QString getObjectSimplifiedName() const;
		QUrl getObjectPictureURL() const;
		const Meta::Object::ApplicationScheme& getObjectApplicationScheme() const;
		QSharedPointer<const Meta::Object> getObject() const;

		virtual SubjectKey getSubjectKey() = 0;
		virtual const Meta::SubjectId& getSubjectId() const = 0;
		virtual const QString& getMoniker() const = 0;
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
		Q_INVOKABLE bool isOwner(Cadabra::Queen::Core::Meta::Qd subject_qd) const;
		Q_INVOKABLE bool hasRealm() const;
	/** @} */
	};
} } } }

#endif
