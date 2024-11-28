#ifndef Cadabra_Queen_Repository_Model_Application_WebApplication_hpp
#define Cadabra_Queen_Repository_Model_Application_WebApplication_hpp

#include "../AbstractApplication.hpp"

#include <QString>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class WebApplication : public AbstractApplication
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	private:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		WebApplication(QObject* parent = nullptr);
		virtual ~WebApplication() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void fetch(const QString& version);
		Q_INVOKABLE void sign(const QString& version);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void denied();
		void accessed(QUrl url, QString preset);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onFetched(quint8 domain, quint32 code, QString message);
		void onFetched(QJsonValue payload);

		void onSigned(quint8 domain, quint32 code, QString message);
		void onSigned(QJsonValue payload);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
//		virtual bool onClose() override;
//		virtual bool onTeardown() override;
	/** @} */
	};
} } } }

#endif
