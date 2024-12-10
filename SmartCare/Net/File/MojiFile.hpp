#ifndef Cadabra_Queen_SmartCare_Net_MojiFile_hpp
#define Cadabra_Queen_SmartCare_Net_MojiFile_hpp

#include <QObject>

#include "../../../Core/Net/Call/File/QRCFile.hpp"

#include <array>
#include <QDir>
#include <QUrl>
#include <QChar>
#include <QString>
#include <QFuture>
#include <QJsonObject>
#include <QFutureWatcher>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Net {
	/**
	 * @brief The MojiFile class
	 * @todo Migrate to CubeFile
	 */
	class MojiFile : public Core::Net::QRCFile
	{
	/** @name Aliases */
	/** @{ */
	public:
		struct E
		{
			QUrl url;
			QChar unicode;
			QString meaning;
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit MojiFile(const QString& family);
		virtual ~MojiFile() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QDir _storage_path;
		const QString _family;
		QString _collection;
		QJsonObject _index;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		State load(const QString& collection = "");
		E locate(const QString& name) const;
		E locate(const QChar& code) const;

	private:
		bool unpack();
		bool index();
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
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
