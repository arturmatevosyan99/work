#ifndef Cadabra_Queen_Core_Net_Call_File_CubeFile_hpp
#define Cadabra_Queen_Core_Net_Call_File_CubeFile_hpp

#include "../FileCall.hpp"

#include <QUuid>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class CubeFile : public FileCall
	{
	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit CubeFile(const QString& api_key = "0:0:0:0:0");
		virtual ~CubeFile() override = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _api_key;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		State load(const QString& channel, const QString& collection, const QString& file, const QString& extension);
		State load(const QUuid& uid, const QString& extension);

	private:
		virtual State load(const QUrl& url) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Mutators */
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

	/** @name Hooks */
	/** @{ */
	protected:
	/** @} */
	};
} } } }

#endif
