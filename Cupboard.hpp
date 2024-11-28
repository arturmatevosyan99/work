#ifndef Cadabra_Queen_Cupboard_hpp
#define Cadabra_Queen_Cupboard_hpp

#include "Qaos/Zip.hpp"

#include <QFile>
#include <QSharedPointer>

namespace Cadabra { namespace Queen {
	class Cupboard : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Cupboard(QObject* parent = nullptr);
		virtual ~Cupboard() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QSharedPointer<QTemporaryFile> _file;
		Qaos::ZipWriter _archiver;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		QIODevice& pack();
		bool file(const QString& name, QIODevice* io_device);
		bool file(const QString& name, const QByteArray& byte_array);
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	protected:
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:

	/** @} */

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} }

#endif
