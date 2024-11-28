#ifndef Cadabra_Queen_Repository_Model_TagSet_hpp
#define Cadabra_Queen_Repository_Model_TagSet_hpp

#include "../Meta/Tag.hpp"

#include <QSet>
#include <QSharedPointer>

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class Roaster;
	class TagSet
	{
	/** @name Qroperties */
	/** @{ */
	Q_GADGET
		Q_PROPERTY(int count READ getCount CONSTANT)
		Q_PROPERTY(QSet<Cadabra::Queen::Repository::Meta::Tag*> list READ getSet CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		TagSet();
		TagSet(const TagSet& origin);
		TagSet(TagSet&& origin);
		virtual ~TagSet();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QSharedPointer<QObject> _dummy;
		QSet<Cadabra::Queen::Repository::Meta::Tag*> _set;
	/** @} */

	/** @name Converters */
	/** @{ */
	public:
		Q_INVOKABLE QString toString() const noexcept;
		Q_INVOKABLE QStringList toStringList() const noexcept;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		int getCount() const;
		QSet<Cadabra::Queen::Repository::Meta::Tag*> getSet() const;
		Q_INVOKABLE Cadabra::Queen::Repository::Meta::Tag* getTag(int index) const;
		Q_INVOKABLE Cadabra::Queen::Core::Meta::Qd getQd(int index) const;
		Q_INVOKABLE QString getAlias(int index) const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void add(Meta::Tag* tag);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
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
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Repository::Model::TagSet)

#endif
