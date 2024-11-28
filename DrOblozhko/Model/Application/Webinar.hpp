#ifndef Cadabra_Queen_DrOblozhko_Model_Application_Webinar_hpp
#define Cadabra_Queen_DrOblozhko_Model_Application_Webinar_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko { namespace Model {
	class Webinar : public Repository::Model::AbstractApplication
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Type
		{
			Unknown,
			YouTube,
			CadabraTube,
			HyperLink,
			File,
			RTSP
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Type type READ getType NOTIFY fetched)
		Q_PROPERTY(QUrl source READ getSource NOTIFY fetched)
		Q_PROPERTY(QString title READ getTitle NOTIFY fetched)
		Q_ENUM(Type)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Webinar(QObject* parent = nullptr);
		virtual ~Webinar() override;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Type _type;
		QUrl _source;
		QString _title;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE void fetch();
		Q_INVOKABLE void page();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Type getType() const;
		QUrl getSource() const;
		QString getTitle() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void fetched();
		void denied();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
		void onFetched(quint8 domain, quint32 code, QString message);
		void onPaged(QJsonValue payload);
		void onFetched(QJsonValue payload);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void customEvent(QEvent* event) override;
		virtual bool onHandle() override;
		virtual bool onOpen() override;
		virtual bool onSetup() override;
//		virtual bool onClose() override;
//		virtual bool onTeardown() override;
	/** @} */
	};
} } } }

#endif
