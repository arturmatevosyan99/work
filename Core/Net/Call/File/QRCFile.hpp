#ifndef Cadabra_Queen_Core_Net_Call_File_QRCFile_hpp
#define Cadabra_Queen_Core_Net_Call_File_QRCFile_hpp

#include "../FileCall.hpp"

#include <QUuid>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Net {
	class QRCFile : public FileCall
	{
	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		QRCFile();
		virtual ~QRCFile() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	private:
		virtual State load(const QUrl& url) override;

	public:
		State load(const QString& extension, const QString& filename);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
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
