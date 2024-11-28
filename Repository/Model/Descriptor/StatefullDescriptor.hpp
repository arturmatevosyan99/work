#ifndef Cadabra_Queen_Repository_Model_Descriptor_StatefullDescriptor_hpp
#define Cadabra_Queen_Repository_Model_Descriptor_StatefullDescriptor_hpp

#include "../AbstractDescriptor.hpp"

#include "SessionDescriptor.hpp"

namespace Cadabra { namespace Queen { namespace Repository { namespace Model {
	class StatefullDescriptor : public AbstractDescriptor
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Repository::Meta::Object::Handle handle READ getHandle CONSTANT)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit StatefullDescriptor(const SessionDescriptor* session_descriptor, QSharedPointer<const Meta::Object> meta, Meta::Object::Handle handle, bool singleton = false, const QVariant& option = QVariant());
		virtual ~StatefullDescriptor() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		const SessionDescriptor* _session_descriptor;
		const bool _singleton;
		const QVariant _option;
		Repository::Meta::Object::Handle _handle;
		QString _moniker;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		Q_INVOKABLE virtual QUrlQuery makeMoniker() const override;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual QVariant opt(const QStringList& path) const override;
		bool bind(const SessionDescriptor* session_descriptor, Meta::Object::Handle handle);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const Meta::Object::Handle& getHandle() const;
		const Meta::Object::ApplicationScheme& getScheme() const;
		virtual SubjectKey getSubjectKey() override;
		virtual const Meta::SubjectId& getSubjectId() const override;
		virtual const QString& getMoniker() const override;
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isSingleton() const;
	/** @} */
	};
} } } }

#endif
