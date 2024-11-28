#ifndef Cadabra_Queen_Core_Test_Context_hpp
#define Cadabra_Queen_Core_Test_Context_hpp

#include <QObject>

#include <QVariant>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Context : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Context(QObject* parent = nullptr);
		virtual ~Context() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Q_INVOKABLE QVariant getProperty(const QString& name, const QVariant& default_value = QVariant{});
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		Q_INVOKABLE void setProperty(const QString& name, const QVariant& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasProperty(const QString& name) const;
	/** @} */
	};
} } } }

#endif
