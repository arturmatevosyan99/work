/**
 @file Assist.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Conversation_Meta_Assist_hpp
#define Cadabra_Queen_Conversation_Meta_Assist_hpp

#include "../../Core/Meta/Qd.hpp"
#include "../../Repository/Meta/Subject.hpp"

#include <QObject>
#include <QJsonObject>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Meta {
	class Assist : public QObject
	{
	/** @name Aliases */
	/** @{ */
	public:
		enum class TargetLayout : char
		{
			Default = '0',
			FeedFooter = 'F',
			FeedHeader = 'H',
			ComposerKeyboard = 'K'
		};

		enum class SourceGroup : char {
			Default = '0',
			User = 'U',
			LastMessage = 'M',
			FeedControl = 'C'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(TargetLayout)
		Q_ENUM(SourceGroup)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd scheme READ makeSchemeQd CONSTANT)
		Q_PROPERTY(QJsonValue body READ getBody CONSTANT)
		Q_PROPERTY(QVariantList affectedSubjectQdList READ makeAffectedSubjectQdVariantList CONSTANT)
		Q_PROPERTY(TargetLayout targetLayout READ getTargetLayout CONSTANT)
		Q_PROPERTY(SourceGroup sourceGroup READ getSourceGroup NOTIFY sourceGroupChanged)
		Q_PROPERTY(QUuid uid READ getUId CONSTANT)
		Q_PROPERTY(bool isValid READ isValid CONSTANT)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Scheme = qint64;
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Assist();
		Assist(Scheme scheme, const QUuid& uid, const QJsonObject& body, const QSet<Repository::Meta::Subject::Id>& affected_subject_id_set, Assist::TargetLayout target_layout = Assist::TargetLayout::Default);
		Assist(const QJsonObject& object);
		Assist(const Assist& origin);
		Assist(Assist&& origin);
		~Assist();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Scheme _scheme;
		QUuid _uid;
		QJsonObject _body;
		QSet<Repository::Meta::Subject::Id> _affected_subject_id_set;
		Assist::TargetLayout _target_layout;
		Assist::SourceGroup _source_group;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Core::Meta::Qd makeSchemeQd() const;
		QList<Core::Meta::Qd> makeAffectedSubjectQdList() const;
		QVariantList makeAffectedSubjectQdVariantList() const;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator QJsonObject() const;
		Assist& operator=(const Assist& rhs);
		Assist& operator=(Assist&& rhs);
	/** @} */

	/** @name Procedures  */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setSourceGroup(SourceGroup group);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Cadabra::Queen::Conversation::Meta::Assist::Scheme& getScheme() const;
		const QUuid& getUId() const;
		const QJsonObject& getBody() const;
		const QSet<Repository::Meta::Subject::Id>& getAffectedSubjectIdSet() const;
		const Assist::TargetLayout& getTargetLayout() const;
		const Assist::SourceGroup& getSourceGroup() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void sourceGroupChanged(SourceGroup backup);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isValid() const;
	/** @} */

	/** @name Hooks */
	/** @{ */
	public:
	/** @} */

	/** @name Templates */
	/** @{ */
	public:
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Conversation::Meta::Assist)

#endif
