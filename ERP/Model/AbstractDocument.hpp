#ifndef Cadabra_Queen_ERP_Model_AbstractDocument_hpp
#define Cadabra_Queen_ERP_Model_AbstractDocument_hpp

#include "../ERP.hpp"
#include "../../Repository/Meta/Essence.hpp"
#include "../../Repository/Meta/Object.hpp"
#include "../../Repository/Meta/Subject.hpp"

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace ERP { namespace Model {
	class AbstractDocument : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString description READ getDescription CONSTANT)
		Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)
		Q_PROPERTY(QString comment READ getComment NOTIFY commentChanged)
		Q_PROPERTY(Cadabra::Queen::ERP::DocumentDirection direction READ getDirection CONSTANT)
		Q_PROPERTY(Cadabra::Queen::ERP::DocumentState state READ getState NOTIFY stateChanged)
		Q_PROPERTY(bool isHeld READ isHeld NOTIFY stateChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractDocument(QObject* parent = nullptr);
		virtual ~AbstractDocument() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		QString _title;
		QString _comment;
		ERP::DocumentState _state;
		ERP::DocumentDirection _direction;
		Repository::Meta::Subject::Id _provoker_subject_id;
		Repository::Meta::Object::Id _journal_id;
		Repository::Meta::Essence::Id _id;
		Repository::Meta::Essence::Id _no;
		QDateTime _issue_timestamp;
		QDateTime _modification_timestamp;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		QJsonObject compose() const;
		bool parse(const QJsonObject& body);

	protected:
		virtual QJsonObject dump() const;
		virtual bool scan(const QJsonObject& body);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getDescription() const;
		Repository::Meta::Object::Id getJournalId() const;
		Repository::Meta::Essence::Id getId() const;
		Repository::Meta::Essence::Id getNo() const;
		Repository::Meta::Subject::Id getProvokerSubjectId() const;
		QString getTitle() const;
		QString getComment() const;
		ERP::DocumentState getState() const;
		ERP::DocumentDirection getDirection() const;
		const QDateTime& getIssueDate() const;
		const QDateTime& getModificationDate() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTitle(const QString& title);
		void setComment(const QString& comment);
		void setState(const ERP::DocumentState& state);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void parsed();
		void titleChanged();
		void commentChanged();
		void stateChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isHeld() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
