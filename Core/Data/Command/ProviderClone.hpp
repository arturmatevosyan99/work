#ifndef Cadabra_Queen_Core_Data_Command_ProviderClone_hpp
#define Cadabra_Queen_Core_Data_Command_ProviderClone_hpp

#include "../AbstractCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class ProviderClone : public AbstractCommand
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		ProviderClone();
		ProviderClone(const QString& origin_name, const QString& destination_name);
		ProviderClone(const ProviderClone& origin);
		ProviderClone(ProviderClone&& origin);
		virtual ~ProviderClone() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _origin_name;
		const QString _destination_name;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getOriginName() const;
		const QString& getDestinationName() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
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

#endif
