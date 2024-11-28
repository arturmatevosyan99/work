#ifndef Cadabra_Queen_Conversation_Model_Accident_MessageDelivery_hpp
#define Cadabra_Queen_Conversation_Model_Accident_MessageDelivery_hpp

#include "../AbstractAccident.hpp"

#include "../../Meta/Message.hpp"
#include "../../../Repository/Meta/Subject.hpp"

#include <QUuid>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
    class MessageDelivery : public AbstractAccident
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Aliases */
	/** @{ */
	public:
        using Map = QMap<Repository::Meta::Subject::Id, QPair<Meta::Message::Id, Meta::Message::Id>>;
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
        MessageDelivery();
        MessageDelivery(const Map& map);
        MessageDelivery(const MessageDelivery& origin);
        MessageDelivery(MessageDelivery&& origin);
        virtual ~MessageDelivery() override;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
    private:
        Map _map;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
        const Map& getMap() const;
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
