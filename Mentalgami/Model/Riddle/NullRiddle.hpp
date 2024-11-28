#ifndef Cadabra_Queen_Mentalgami_Model_Question_NullRiddle_hpp
#define Cadabra_Queen_Mentalgami_Model_Question_NullRiddle_hpp

#include "../TemplatedRiddle.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace Model {
	class NullRiddle : public TemplatedRiddle<std::nullptr_t>
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString imageSource READ getImageSource NOTIFY imageSourceChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		NullRiddle(const QString& text = QString(), const QString& descriptor = QString());
		virtual ~NullRiddle() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _image_source;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& body) override;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getImageSource() const;
		virtual QString getType() const override;
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void imageSourceChanged();
	/** @} */
	};
} } } }

#endif
