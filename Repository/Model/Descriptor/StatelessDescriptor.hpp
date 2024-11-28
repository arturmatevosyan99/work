#ifndef Cadabra_Queen_Repository_Model_Descriptor_StatelessDescriptor_hpp
#define Cadabra_Queen_Repository_Model_Descriptor_StatelessDescriptor_hpp

#include "../AbstractDescriptor.hpp"

#include "SessionDescriptor.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class StatelessDescriptor : public AbstractDescriptor
	{
	/** @name Qroperties */
	/** @{ */	
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit StatelessDescriptor(const SessionDescriptor* session_descriptor, QSharedPointer<const Meta::Object> meta);
		virtual ~StatelessDescriptor() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const QString _moniker;
		const SessionDescriptor* _session_descriptor;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QUrlQuery makeMoniker() const override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual Cadabra::Queen::Repository::Model::SubjectKey getSubjectKey() override;
		virtual const Cadabra::Queen::Repository::Meta::SubjectId& getSubjectId() const override;
		virtual const QString& getMoniker() const override;
	/** @} */
	};
} } } }

#endif
