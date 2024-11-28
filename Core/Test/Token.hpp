#ifndef Cadabra_Queen_Core_Test_Token_hpp
#define Cadabra_Queen_Core_Test_Token_hpp

#include "Node/Assertion.hpp"

#include <QObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Token : public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(QVector<Cadabra::Queen::Core::Test::Assertion*> positions READ getPositionVector WRITE setPositionVector NOTIFY positionVectorChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Token(const QString& description = "Empty description", const QVector<Assertion*>& position_vector = QVector<Assertion*>());
		virtual ~Token() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _description;
		QVector<Assertion*> _position_vector;
		Action* _next_action;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool passBy(Cadabra::Queen::Core::Test::Action* action);
		bool stopAt(Cadabra::Queen::Core::Test::Action* action);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getDescription() const;
		const QVector<Assertion*>& getPositionVector() const;
		Q_INVOKABLE QVariant getProperty(const QString& name, const QVariant& default_value = QVariant{});
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		void setDescription(const QString& description);
		void setPositionVector(const QVector<Assertion*>& position_vector);
		void addPosition(Assertion* position);
		Q_INVOKABLE void setProperty(const QString& name, const QVariant& value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void descriptionChanged();
		void positionVectorChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool hasProperty(const QString& name) const;
	/** @} */
	};
} } } }

#endif
