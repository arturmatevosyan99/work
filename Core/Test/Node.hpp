#ifndef Cadabra_Queen_Core_Test_Node_hpp
#define Cadabra_Queen_Core_Test_Node_hpp

#include <QObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Node : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Node(const QString& identifier, const QString& description);
		virtual ~Node() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _identifier;
		QString _description;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getIdentifier() const;
		const QString& getDescription() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setDescription(const QString& description);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void descriptionChanged();
	/** @} */
	};
} } } }

#endif
