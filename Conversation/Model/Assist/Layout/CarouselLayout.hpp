#ifndef Cadabra_Queen_Conversation_Model_Assist_Layout_CarouselLayout_hpp
#define Cadabra_Queen_Conversation_Model_Assist_Layout_CarouselLayout_hpp

#include "../AbstractLayout.hpp"

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class CarouselLayout : public AbstractLayout
	{
	/** @name Statics */
	/** @{ */
	private:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		CarouselLayout(QObject* parent = nullptr);
		virtual ~CarouselLayout() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
	/** @} */

	/** @name Setters */
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