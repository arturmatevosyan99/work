/**
 @file AbstractScheme.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Core_Meta_AbstractScheme_hpp
#define Cadabra_Queen_Core_Meta_AbstractScheme_hpp

#include "Scheme.hpp"
#include "Qd.hpp"

#include <QObject>
#include <QHash>
#include <QList>
#include <QQmlEngine>
#include <QByteArray>
#include <type_traits>

namespace Cadabra { namespace Queen { namespace Core { namespace Meta {
	class AbstractScheme
	{	
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(QString vendorName READ getVendorName CONSTANT)
		Q_PROPERTY(QString moduleName READ getModuleName CONSTANT)
		Q_PROPERTY(QString modelName READ getModelName CONSTANT)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			VendorName = Qt::UserRole + 1,
			ModuleName,
			ModelName,
			ExtensionURI,
			ExtensionHash,
			CustomRole
		};
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractScheme();
		AbstractScheme(const QString& extname, const QString& modelname);
		AbstractScheme(const QString& vendorname, const QString& modulename, const QString& modelname);
		AbstractScheme(const AbstractScheme&);
		AbstractScheme(AbstractScheme&&);
		virtual ~AbstractScheme();
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		AbstractScheme& operator=(const AbstractScheme&);
		AbstractScheme& operator=(AbstractScheme&&);
		bool operator==(const QString& pattern) const;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _vendor_name;
		QString _module_name;
		QString _model_name;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool load(const Cadabra::Queen::Core::Meta::Qd& qd) = 0;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QHash<int, QByteArray> roleNames() const;
		virtual QVariant data(int role) const;

		const QString& getVendorName() const;
		const QString& getModuleName() const;
		const QString& getModelName() const;
		QString getExtensionURI() const;
		quint32 getExtensionHash() const;
	/** @} */
	};
} } } }

#endif
