#ifndef Cadabra_Queen_SmartCare_Meta_RiddleInformation_hpp
#define Cadabra_Queen_SmartCare_Meta_RiddleInformation_hpp

#include "../../Core/Meta/Scheme.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Meta {
	class RiddleInformation : public QObject
	{
	/** @name Aliases */
	/** @{ */
	public:
		using QuizScheme = Core::Meta::SchemeId;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			Scheme = Qt::UserRole + 1,
			Object,
			Delegate
		};

		enum class Specification : qint16
		{
			Empty = 0,

			BackwardAvailable = 1,
			ForwardAvailable = 2,
			BackwardActive = 4,
			ForwardActive = 8,

			Chronological = 16,
			Historical = 32,
			ReadOnly = 64
		};

		Q_DECLARE_FLAGS(Specifications, Specification)
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_FLAG(Specifications)
	/** @} */
	};
} } } }

Q_DECLARE_OPERATORS_FOR_FLAGS(Cadabra::Queen::SmartCare::Meta::RiddleInformation::Specifications)

#endif
