#ifndef Cadabra_Queen_Core_Data_Command_ProviderConnect_hpp
#define Cadabra_Queen_Core_Data_Command_ProviderConnect_hpp

#include "../AbstractCommand.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class ProviderConnect : public AbstractCommand
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
		ProviderConnect();
		ProviderConnect(const QString& file_path, const QString& connection_name);
		ProviderConnect(const ProviderConnect& origin);
		ProviderConnect(ProviderConnect&& origin);
		virtual ~ProviderConnect() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _file_path;
		const QString _connection_name;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getFilePath() const;
		const QString& getConnectionName() const;
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
