/**
 @file EssenceToken.hpp
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
*/

#ifndef Cadabra_Queen_Repository_Meta_EssenceToken_hpp
#define Cadabra_Queen_Repository_Meta_EssenceToken_hpp

#include "EssenceExpression.hpp"

#include <QVariant>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class EssenceToken : public QObject
	{
	/** @name Constructors */
	/** @{ */
	public:
		enum class Type : char
		{
			Invalid = '!',
			Any = '?',
			Object = '$',
			Subject = '@',
			Group = '%',
			Path = '/',
			TimeFrame = '~',
			BeforeDate = '<',
			AfterDate = '>',
			Tag = '#',
			Interface = '(',
			Method = ')',
			Attribute = ':'
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Type)
		Q_PROPERTY(bool active MEMBER active NOTIFY changed)
		Q_PROPERTY(bool invalid READ isInvalid NOTIFY changed)
		Q_PROPERTY(QString value MEMBER value NOTIFY changed)
		Q_PROPERTY(Type type MEMBER type NOTIFY changed)
		Q_PROPERTY(QString sign READ getSign NOTIFY changed)
		Q_PROPERTY(EssenceExpression expression READ express NOTIFY changed)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		EssenceToken(QObject* parent = nullptr);
		EssenceToken(const EssenceToken& origin);
		EssenceToken(EssenceToken&& origin);
		~EssenceToken() = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator qint64() const;
		operator QVariant() const;
		bool operator<(const qint64& rhs) const;
		bool operator>(const qint64& rhs) const;
		bool operator==(const qint64& rhs) const;
		bool operator<(const EssenceToken& rhs) const;
		bool operator>(const EssenceToken& rhs) const;
		bool operator==(const EssenceToken& rhs) const;
		EssenceToken& operator=(const EssenceToken& rhs);
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool parse(const QVariant& value);
		Q_INVOKABLE bool parse(const QString& value);
		Q_INVOKABLE EssenceExpression express() const;
	/** @} */

	/** @name Properties */
	/** @{ */
	public:
		bool active;
		Type type;
		QString value;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QString getSign() const;
	/** @} */

	/** @name Signals*/
	/** @{ */
	signals:
		void changed();
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		Q_INVOKABLE QString dump() const noexcept;
	/** @} */

	/** @name Friends */
	/** @{ */
	friend QDebug operator<<(QDebug debug, const EssenceToken& handle);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isInvalid() const;
	/** @} */
	};

	inline uint qHash(const EssenceToken& token)
	{
		return ::qHash(token.value) ^ static_cast<qint8>(token.type);
	}

	inline uint qHash(const EssenceToken::Type& token_type)
	{
		return ::qHash(static_cast<char>(token_type));
	}
} } } }

#endif
