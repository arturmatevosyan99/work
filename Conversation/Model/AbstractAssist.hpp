#ifndef Cadabra_Queen_Conversation_Model_AbstractAssist_hpp
#define Cadabra_Queen_Conversation_Model_AbstractAssist_hpp

#include "Application/GroupChannel.hpp"
#include "../Meta/Assist.hpp"
#include "../Meta/Channel.hpp"
#include "../../Repository/Meta/Subject.hpp"
#include "../../Repository/Model/AbstractDescriptor.hpp"

#include <QJsonValue>
#include <QJsonObject>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class AbstractAssist : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Conversation::Model::GroupChannel* channelModel READ getChannelApplication CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Assist::Scheme scheme READ getScheme NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd schemeQd READ getSchemeQd NOTIFY imported)
		Q_PROPERTY(QList<Cadabra::Queen::Core::Meta::Qd> affectedQdSubjects READ makeAffectedSubjectQdList NOTIFY imported)
		Q_PROPERTY(QUuid uid READ getUId NOTIFY imported)
		Q_PROPERTY(Cadabra::Queen::Conversation::Meta::Assist::TargetLayout targetLayout READ getTargetLayout NOTIFY imported)
		Q_PROPERTY(QString previewText READ getPreviewText NOTIFY imported)
		Q_PROPERTY(QString preview READ getPreview NOTIFY imported)
		Q_PROPERTY(bool hasChannelApplication READ hasChannelApplication NOTIFY imported)
		Q_PROPERTY(bool isImported READ isImported NOTIFY imported)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractAssist(Meta::Assist::Scheme scheme, QObject* parent = nullptr);
		virtual ~AbstractAssist() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Cadabra::Queen::Conversation::Model::GroupChannel* _channel_application;
		const Meta::Assist::Scheme _scheme;
		QSet<Repository::Meta::Subject::Id> _affected_subject_id_set;
		Meta::Assist::TargetLayout _target_layout;
		QUuid _uid;
		bool _is_imported;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual bool import(GroupChannel* channel_application, const Cadabra::Queen::Conversation::Meta::Assist& data);
		bool import(GroupChannel* channel_application, const QUuid& uid, const QJsonObject& body, const QSet<Repository::Meta::Subject::Id>& affected_subject_id_set, Meta::Assist::TargetLayout target_layout);
		Q_INVOKABLE bool import(Cadabra::Queen::Conversation::Model::GroupChannel* channel_application, const QUuid& uid, const QJsonObject& body, const QVariantList& affected_subject_variant_list, Cadabra::Queen::Conversation::Meta::Assist::TargetLayout target_layout);
		Q_INVOKABLE Cadabra::Queen::Conversation::Meta::Assist exportMeta() const;

	protected:
		virtual QJsonObject exportBody() const = 0;
		virtual bool importBody(const QJsonObject& body) = 0;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Cadabra::Queen::Conversation::Model::GroupChannel* getChannelApplication() const;
		Cadabra::Queen::Conversation::Meta::Assist::Scheme getScheme() const;
		Cadabra::Queen::Core::Meta::Qd getSchemeQd() const;
		const QSet<Cadabra::Queen::Repository::Meta::Subject::Id>& getAffectedSubjectIdSet() const;
		const QUuid& getUId() const;
		Meta::Assist::TargetLayout getTargetLayout() const;
		virtual Meta::Assist::TargetLayout getDefaultTargetLayout() const;
		QString getPreview() const;
		virtual QString getPreviewText() const;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QList<Core::Meta::Qd> makeAffectedSubjectQdList() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void imported();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasChannelApplication() const;
		bool isImported() const;
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */
	};
} } } }

#endif
