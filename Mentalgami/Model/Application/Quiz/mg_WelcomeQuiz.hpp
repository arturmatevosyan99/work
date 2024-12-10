#ifndef Cadabra_Queen_Mentalgami_Model_Application_Quiz_WelcomeQuiz_hpp
#define Cadabra_Queen_Mentalgami_Model_Application_Quiz_WelcomeQuiz_hpp

#include "../mg_GenericQuiz.hpp"

#include "../../../Net/Call/mg_WelcomeCall.hpp"
#include "../../../../Repository/Model/Avatar.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class WelcomeQuiz : public GenericQuiz
	{
	/** @name Classes */
	/** @{ */
	public:
		enum class Phase : qint8
		{
			SignIn = 1,
			SignUp = 2,
			CodeIn = 3,
			CodeUp = 4,
			OnBoard = 5
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Phase)
		Q_PROPERTY(QAbstractListModel* listModel READ getListModel CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Avatar* avatar READ getAvatar WRITE setAvatar NOTIFY avatarChanged)
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Model::WelcomeQuiz::Phase phase READ getPhase WRITE setPhase NOTIFY phaseChanged)
		Q_PROPERTY(QString service READ getService WRITE setService NOTIFY serviceChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		WelcomeQuiz(QObject* parent = nullptr);
		virtual ~WelcomeQuiz() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Net::WelcomeCall _call;
		Repository::Model::Avatar* _avatar;
		Phase _phase;
		QString _service;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool request(const QString& alias);
		Q_INVOKABLE bool attach(const QVariant& variant);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractListModel* getListModel();
		Repository::Model::Avatar* getAvatar();
		WelcomeQuiz::Phase getPhase() const;
		const QString& getService() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setAvatar(Repository::Model::Avatar* avatar);
		void setPhase(WelcomeQuiz::Phase phase);
		void setService(const QString& service);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void avatarChanged();
		void phaseChanged();
		void serviceChanged();

		void invited();
		void locked();
		void rejected(const QString& message);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onResponded(const QString& message);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool onHandle() override;
		virtual bool onOpen() override;
	/** @} */
	};
} } } }

#endif
