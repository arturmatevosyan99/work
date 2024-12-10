#ifndef Cadabra_Queen_Mentalgami_Model_Track_HTMLTrack_hpp
#define Cadabra_Queen_Mentalgami_Model_Track_HTMLTrack_hpp

#include "../mg_AbstractTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class HTMLTrack : public AbstractTrack
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		HTMLTrack(QObject* parent = nullptr);
		virtual ~HTMLTrack() override = default;
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
