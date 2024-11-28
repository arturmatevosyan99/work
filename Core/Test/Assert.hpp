#ifndef Cadabra_Queen_Core_Test_Assert_hpp
#define Cadabra_Queen_Core_Test_Assert_hpp

#include <QObject>

namespace Cadabra { namespace Queen { namespace Core { namespace Test {
	class Action;

	class Assert: public QObject
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(Cadabra::Queen::Core::Test::Action* targetAction READ getTargetAction WRITE setTargetAction NOTIFY targetActionChanged)
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		Assert(Action* target_action = nullptr, const QString& description = QString());
		virtual ~Assert() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Action* _target_action;
		QString _description;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual void beginRun();
		virtual void endRun();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		Action* getTargetAction() const;
		const QString& getDescription() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTargetAction(Action* target_action);
		void setDescription(const QString& description);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void targetActionChanged();
		void descriptionChanged();
		void actionPrepared();
		void actionCompleted();
	/** @} */
	};
} } } }

#endif
