#ifndef Cadabra_Queen_Core_Net_Call_FileCall_hpp
#define Cadabra_Queen_Core_Net_Call_FileCall_hpp

#include "../ServiceCall.hpp"

#include <QTemporaryFile>
#include <QByteArray>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class FileCall : public ServiceCall
	{
	/** @name Statics */
	/** @{ */
	private:
		static Endpoint GeneralEndpoint;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		using CheckSum = QPair<QString, ulong>;
		enum class State : quint8
		{
			Idle = 0,
			Fault = 1,
			Cached = 2,
			Deffer = 3
		};
	/** @} */

	Q_OBJECT
	/** @name Constructors */
	/** @{ */
	public:
		FileCall(const QString& filename_template = "XXXXXX.tmp");
		virtual ~FileCall() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	protected:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QByteArray _data;
		QTemporaryFile _file;
		QString _file_path;
		QString _filename_template;
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
		State load(QByteArray&& data);
		bool remove();

	public:
		virtual State load(const QUrl& url);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		QUrl makeURL() const;
		CheckSum makeCheckSum() const;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getFilenameTemplate() const;
		const QString& getCachePath() const;
		const QByteArray& getData() const;
		const QFile& getFile() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setAutoRemove(bool onoff = true);
		void setFilenameTemplate(const QString& name = "");
		void setCachePath(const QString& path = "", bool auto_create = false);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void loaded();
		void faulted(QString error);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void onSuccess(QSharedPointer<QNetworkReply> reply) override;
		virtual void onFault(QSharedPointer<QNetworkReply> reply) override;
	/** @} */
	};
} } } }

#endif
