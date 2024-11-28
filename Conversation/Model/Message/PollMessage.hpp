#ifndef Cadabra_Queen_Conversation_Model_Message_PollMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_PollMessage_hpp

#include "../AbstractMessage.hpp"

#include <QQmlListProperty>
#include <QMap>
#include <QMultiMap>
#include <QSet>
#include <QVector>
#include <QPair>
#include <QString>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class PollMessage : public AbstractMessage
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class ElectionRule : char
		{
			OneOf = 'O',
			SomeOf = 'S',
			AllOf = 'A',
			NoneOf = 'N',
			OneExcept = 'o',
			SomeExcept = 's',
		};

		enum class DisplayMode : char
		{
			My = 'M',
			All = 'A',
			Quiz = 'Q'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(ElectionRule)
		Q_ENUM(DisplayMode)
		Q_PROPERTY(QString question READ getQuestion NOTIFY imported)
		Q_PROPERTY(int optionCount READ getOptionCount NOTIFY imported)
		Q_PROPERTY(bool eligible READ isEligible NOTIFY imported)
		Q_PROPERTY(bool votable READ isVotable NOTIFY voted)
		Q_PROPERTY(bool answered READ isAnswered NOTIFY imported)
		Q_PROPERTY(bool editableMode READ isEditableMode NOTIFY imported)
		Q_PROPERTY(bool multiplyMode READ isMultiplyMode NOTIFY imported)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using OptionMap = QMap<QString, QString>;
		using RuleVector = QVector<QPair<ElectionRule, QSet<QString>>>;
		using VoteMap = QMultiMap<Repository::Meta::Subject::Id, QString>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		PollMessage(QObject* parent = nullptr);
		virtual ~PollMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		int _sync_timer_id;
		bool _is_sync;

		QString _question;
		bool _editable_mode;
		DisplayMode _display_mode;
		QSet<Repository::Meta::Subject::Id> _eligible_set;
		OptionMap _option_map;
		VoteMap _vote_map;
		RuleVector _rule_vector;
		QSet<QString> _exit_set;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE QString opt(int index) const;
		Q_INVOKABLE QString describe(int index) const;
		Q_INVOKABLE bool check(const QString& option) const;
		Q_INVOKABLE bool vote(const QString& option, bool auto_sync = true);
		Q_INVOKABLE bool sync(bool timer = false);

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		const QString& getQuestion() const;
		DisplayMode getDisplayMode() const;
		const QSet<Repository::Meta::Subject::Id>& getEligibleSet() const;
		int getOptionCount() const;
		const OptionMap& getOptionMap() const;
		const VoteMap& getVoteMap() const;
		const RuleVector& getRuleVector() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void voted();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	private:
		virtual void timerEvent(QTimerEvent* event) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEligible() const;
		bool isVotable() const;
		bool isAnswered() const;
		bool isEditableMode() const;
		bool isMultiplyMode() const;
	/** @} */
	};
} } } }

#endif
