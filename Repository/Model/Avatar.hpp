#ifndef Cadabra_Queen_Repository_Model_Avatar_hpp
#define Cadabra_Queen_Repository_Model_Avatar_hpp

#include <QObject>

#include "Realm.hpp"
#include "Application/Session.hpp"
#include "Application/Directory.hpp"

#include "../Meta/Profile.hpp"

#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	/**
	 * @brief The Avatar class
	 * @todo Добавить кэширование данных так, чтобы начинать повторный запуск приложения с ранее полученной информацей и асинхронно обновлять
	 */
	class Avatar : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Flag
		{
			Default = 0b0000
		};
		Q_DECLARE_FLAGS(Flags, Flag)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAGS(Flags)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Realm* realm READ getRealm CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Session* session READ getSession CONSTANT)
		Q_PROPERTY(Cadabra::Queen::Repository::Model::Directory* home READ getHome CONSTANT)
		Q_PROPERTY(QString alias READ getAlias WRITE setAlias NOTIFY aliasChanged)
		Q_PROPERTY(Flags flag READ getFlagSet WRITE setFlagSet NOTIFY flagsetChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		/**
		 * @brief Avatar
		 * @param parent
		 * @param alias "droblozhko.patient", "mentalgami.patient"
		 */
		Avatar(QObject* parent = nullptr, const QString& alias = "droblozhko.patient");
		virtual ~Avatar() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Realm* const _realm;
		Session* const _session;
		Directory* const _home;
		QString _alias;
		Flags _flag_set;

		QSharedPointer<Meta::Profile> _subject_profile;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool reviveSession();
		Q_INVOKABLE bool pointEnd(const QUrl& endpoint);
		Q_INVOKABLE bool pickUpProfile();
		Q_INVOKABLE bool indexHome();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Realm* getRealm();
		Session* getSession();
		Directory* getHome();
		const QString& getAlias() const;
		const Flags& getFlagSet() const;
		const Meta::Object::Id& getHomeObjectId() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setAlias(const QString& value);
		void setFlagSet(const Flags& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void profilePickedUp();
		void aliasChanged();
		void flagsetChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		void onPickProfileUp(const Core::Net::CRPReaction& reaction);
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::Repository::Model::Avatar::Flags)
Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Model::Avatar::Flags)

#endif
