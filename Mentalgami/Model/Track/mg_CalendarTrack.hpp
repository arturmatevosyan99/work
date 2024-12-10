#ifndef Cadabra_Queen_Mentalgami_Model_Track_CalendarTrack_hpp
#define Cadabra_Queen_Mentalgami_Model_Track_CalendarTrack_hpp

#include "../mg_AbstractTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class CalendarTrack : public AbstractTrack
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		CalendarTrack(QObject* parent = nullptr);
		virtual ~CalendarTrack() override = default;
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
