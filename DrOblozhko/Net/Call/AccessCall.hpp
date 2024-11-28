#ifndef Cadabra_Queen_DrOblozhko_Net_Call_AccessCall_hpp
#define Cadabra_Queen_DrOblozhko_Net_Call_AccessCall_hpp

#include "../../../Core/Net/ServiceCall.hpp"
#include "../../../Repository/Meta/Subject.hpp"
#include "../../../Repository/Meta/Object.hpp"
#include "../../../Repository/Meta/Session.hpp"
#include "../../../Repository/Model/Application/Session.hpp"

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Net {
	class AccessCall : public Core::Net::ServiceCall
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AccessCall(Core::Net::Endpoint* endpoint = nullptr);
		virtual ~AccessCall() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _account;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief Запрос распознавания аккаунта
		 * Или восстановление(как в recover) или подготовка к верификации для дальнейшей регистрации
		 * Может выбросить recovered, prepared или unprepared
		 * @param account Адрес электронной почты, номер мобильного телефона, ...
		 */
		void recognize(const QString& account);

		/**
		 * @brief Запрос кода для восстановления доступа к аккаунту
		 * Может выбросить unrecovered или recovered
		 * @param account Адрес электронной почты, номер мобильного телефона, ...
		 */
		void recover(const QString& account);

		/**
		 * @brief Подтверждение кода восстановления доступа к аккаунту или верификация аккаунта после подготовки к регистрации
		 * Может выбросить unrestored, restored или ratified
		 * @param code
		 */
		void restore(const QString& code);

		/**
		 * @brief Подтверждение кода восстановления доступа к аккаунту или верификация аккаунта после подготовки к регистрации
		 * Может выбросить unrestored, restored или ratified
		 * @param code
		 */
		void ratify(const QString& code);

		/**
		 * @brief Переназначение подготовленного и верифицированного аккаунта(или назначение имени и завершение регистрации)
		 * Может выбросить reassigned или unassigned
		 * @param code
		 */
		void reassign(const QString& name);

		/**
		 * @brief Удаление аккаунта на backend и всех связанных данных
		 * В реальности произойдет Oblivion (пометка о забвении)
		 * @param session
		 */
		void resign(Cadabra::Queen::Repository::Model::Session* session);
	/** @} */

	/** @name Getters */
	/** @{ */
	private slots:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void restored(Cadabra::Queen::Repository::Meta::SubjectId session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme session_scheme, Cadabra::Queen::Repository::Meta::ObjectId channel_id, Cadabra::Queen::Repository::Meta::Object::Handle channel_hash, Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channel_scheme);
		void reassigned(Cadabra::Queen::Repository::Meta::SubjectId session_id, Cadabra::Queen::Repository::Meta::Session::Handle session_handle, Cadabra::Queen::Repository::Meta::Subject::AvatarScheme session_scheme, Cadabra::Queen::Repository::Meta::ObjectId channel_id, Cadabra::Queen::Repository::Meta::Object::Handle channel_hash, Cadabra::Queen::Repository::Meta::Object::ApplicationScheme channel_scheme);
		void failed(QString error);
		void ratified(bool success);
		void recovered(bool success);
		void prepared(bool success);
		void resigned();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasAccount() const;
	/** @} */
	};
} } } }

#endif
