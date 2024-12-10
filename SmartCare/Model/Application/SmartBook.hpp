#ifndef Cadabra_Queen_SmartCare_Model_Application_SmartBook_hpp
#define Cadabra_Queen_SmartCare_Model_Application_SmartBook_hpp

#include "../../../Repository/Model/AbstractApplication.hpp"
#include "../BookIndex.hpp"

#include <QScopedPointer>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class SmartBook : public Repository::Model::AbstractApplication
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	Q_OBJECT
	Q_PROPERTY(QAbstractItemModel* index READ getIndexModel NOTIFY opened)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		SmartBook(QObject* parent = nullptr);
		~SmartBook() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QScopedPointer<BookIndex> _index_model;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	Q_INVOKABLE void setSource(const QUrl& url);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QAbstractItemModel* getIndexModel() const;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void opened();
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		bool onOpen() override;
		bool onClose() override;
	/** @} */
	};
} } } }

#endif
