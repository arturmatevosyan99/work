#ifndef Cadabra_Queen_SmartCare_View_AbstractMoji_hpp
#define Cadabra_Queen_SmartCare_View_AbstractMoji_hpp

#include <QObject>

#include "../Net/File/MojiFile.hpp"

#include <QDir>
#include <QUrl>
#include <QChar>
#include <QPair>
#include <QFuture>
#include <QFutureWatcher>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace View {
	/**
	 * @brief The Moji class
	 * @todo Derive from QQuickImage
	 */
	class Moji : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QUrl url READ getURL WRITE setURL NOTIFY loaded)
		Q_PROPERTY(QChar unicode READ getUnicode WRITE setUnicode NOTIFY loaded)
		Q_PROPERTY(QString meaning READ getMeaning WRITE setMeaning NOTIFY loaded)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Moji(QObject* parent = nullptr);
		virtual ~Moji() override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		QString _family;
		QString _collection;
		QString _symbol;
		QUrl _url;
		QChar _unicode;
		QString _meaning;
		QFutureWatcher<void> _;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		/**
		 * @brief load
		 * @param family (award, food, mood, ...)
		 * @param name (com.oblozhko.metabolicplate.z)
		 */
		Q_INVOKABLE bool load(const QString& family, const QString& name);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QUrl& getURL() const;
		const QChar& getUnicode() const;
		const QString& getMeaning() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		Q_INVOKABLE void set(const QUrl& url, const QString& meaning, const QChar& unicode);
		void setURL(const QUrl& value);
		void setUnicode(const QChar& value);
		void setMeaning(const QString& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void loaded(bool success);
	/** @} */
	};
} } } }

#endif
