#ifndef Cadabra_Queen_Mentalgami_Model_Track_VideoTrack_hpp
#define Cadabra_Queen_Mentalgami_Model_Track_VideoTrack_hpp

#include "../AbstractTrack.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class VideoTrack : public AbstractTrack
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		VideoTrack(QObject* parent = nullptr);
		virtual ~VideoTrack() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
	/** @} */

	/** @name Procedures */
	/** @{ */
	protected:
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

	/** @name States */
	/** @{ */
	public:
	/** @} */
	};
} } } }

#endif
