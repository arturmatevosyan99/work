#ifndef Cadabra_Queen_Core_Test_Node_Port_hpp
#define Cadabra_Queen_Core_Test_Node_Port_hpp

#include "../Node.hpp"

#include <QQmlParserStatus>
#include <QPointer>
#include <QUrl>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class PetrI;
	class PetriCase;
	class Port: public Node, public QQmlParserStatus
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QUrl source READ getSource WRITE setSource CONSTANT)
		Q_INTERFACES(QQmlParserStatus)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Port(const QString& description = "Empty description");
		virtual ~Port() override;
	/** @} */

	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		PetrI* _origin;
		QUrl _source;
		QPointer<PetriCase> _case;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		bool dock();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		PetrI* getPetrI();
		PetrI* getOriginPetrI();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QUrl& getSource() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setSource(const QUrl& url);
	/** @} */

	/** @name Hooks */
	/** @{ */
	protected:
		virtual void classBegin() override;
		virtual void componentComplete() override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */
	};
} } } }

#endif
