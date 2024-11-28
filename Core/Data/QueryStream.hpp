#ifndef Cadabra_Queen_Core_Data_QueryStream_hpp
#define Cadabra_Queen_Core_Data_QueryStream_hpp

#include <QTextStream>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Data {
	class Factory;

	class QueryStream : public QTextStream
	{
	/** @name Constructors */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		QueryStream();
		QueryStream(const QString& query);
		virtual ~QueryStream() override = default;
	/** @} */

	/** @name Operators */
	/** @{ */
	public:
		operator const QString&() const;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _query_string;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void clear();
		bool append(const QString& text);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isEmpty() const;
	/** @} */
	};
} } } }

#endif
