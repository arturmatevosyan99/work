#ifndef Cadabra_Queen_Repository_Meta_Tag_hpp
#define Cadabra_Queen_Repository_Meta_Tag_hpp

#include "../Meta/Essence.hpp"

#include "../../Core/Meta/Qd.hpp"
#include <QString>

namespace Cadabra { namespace Queen { namespace Repository { namespace Meta {
	class Tag : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString alias READ getAlias NOTIFY aliasChanged)
		Q_PROPERTY(Cadabra::Queen::Core::Meta::Qd qd READ getId CONSTANT)
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using Id = Essence::Id;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Tag(QObject* parent = nullptr);
		Tag(QObject* parent, QMap<Meta::Essence::Id, QObject*>::ConstIterator iterator);
		Tag(const Tag& origin);
		Tag(Tag&& origin);
		virtual ~Tag();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QMap<Meta::Tag::Id, QObject*>::ConstIterator _iterator;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		Tag& operator=(const Tag& rhs);
		Tag& operator=(Tag&& rhs);
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Meta::Tag::Id& getId() const;
		Core::Meta::Qd getQd() const;
		QString getAlias() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void aliasChanged();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};

	inline uint qHash(const Tag& key)
	{
		return static_cast<uint>(key.getId());
	}
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Meta::Tag)

#endif
