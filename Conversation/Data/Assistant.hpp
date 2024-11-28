#ifndef Cadabra_Queen_Conversation_Data_Assistant_hpp
#define Cadabra_Queen_Conversation_Data_Assistant_hpp

#include "../Meta/Assist.hpp"

#include <QAbstractListModel>
#include <QList>
#include <QUuid>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Data {
	class Assistant : public QAbstractListModel
	{
	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			UId = Qt::UserRole + 1,
			Scheme,
			Body,
			AffectedSubjectQdList,
			Layout,
			Source
		};
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
		using List = QList<Meta::Assist>;
		using TargetLayout = Meta::Assist::TargetLayout;
		using SourceGroup = Meta::Assist::SourceGroup;
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(TargetLayout)
		Q_ENUM(SourceGroup)
	/** @} */

	/** @name Statics */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Assistant(QObject* parent = nullptr);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		List _list;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void emplace(const Meta::Assist& assist, Meta::Assist::SourceGroup group = SourceGroup::Default);
		void emplace(const QList<Meta::Assist>& assist_list, Meta::Assist::SourceGroup group = SourceGroup::Default);
		void dismiss(const Meta::Assist& target);
		void dismiss(const QUuid& target_uid);
		void dismiss(SourceGroup source);
		void dismiss(TargetLayout layout);
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int rowCount(const QModelIndex &parent) const override;
		virtual QVariant data(const QModelIndex &index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
