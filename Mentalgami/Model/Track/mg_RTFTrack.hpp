#ifndef Cadabra_Queen_Mentalgami_Model_Track_RTFTrack_hpp
#define Cadabra_Queen_Mentalgami_Model_Track_RTFTrack_hpp

#include "../mg_AbstractTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class RTFTrack : public AbstractTrack
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		RTFTrack(QObject* parent = nullptr);
		virtual ~RTFTrack() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
		virtual QString makePreviewText() const override;
	/** @} */

	/** @name Getters */
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
	};
} } } }

#endif
