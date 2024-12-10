#ifndef Cadabra_Queen_SmartCare_Model_Track_PDFTrack_hpp
#define Cadabra_Queen_SmartCare_Model_Track_PDFTrack_hpp

#include "../AbstractTrack.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class PDFTrack : public AbstractTrack
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		PDFTrack(QObject* parent = nullptr);
		virtual ~PDFTrack() override = default;
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
