#ifndef Cadabra_Queen_Core_System_Alert_DialogAlert_hpp
#define Cadabra_Queen_Core_System_Alert_DialogAlert_hpp

#include "../Alert.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class DialogAlert : public AbstractAlert
	{
	/** @name classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(bool isModal READ isModal NOTIFY modalChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		DialogAlert(QObject* parent = nullptr, const QString& name = "", const QString& title = "", Level level = Level::Info);
		DialogAlert(const DialogAlert& origin);
		virtual ~DialogAlert() override;
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
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void modalChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		virtual bool isModal() const;
	/** @} */
	};
} } } }

Q_DECLARE_METATYPE(Cadabra::Queen::Core::System::DialogAlert)

#endif
