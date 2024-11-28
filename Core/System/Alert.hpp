#ifndef Cadabra_Queen_Core_System_Alert_hpp
#define Cadabra_Queen_Core_System_Alert_hpp

#include <QAbstractListModel>

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QVector>
#include <QQmlEngine>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class DialogAlert;
	class StateAlert;
	class SoundAlert;
	class InputAlert;
	class ActionAlert;

	class AbstractAlert : public QObject
	{
	/** @name classes */
	/** @{ */
	public:
		enum Level
		{
			Debug,
			Info,
			Warning,
			Error,
			Fault
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Level)
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(Level level READ getLevel WRITE setLevel NOTIFY levelChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AbstractAlert(QObject* parent = nullptr, const QString& name = "", const QString& title = "", Level level = Level::Info);
		AbstractAlert(const AbstractAlert& origin);
		virtual ~AbstractAlert() override;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _code;
		QString _title;
		Level _level;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		virtual Q_INVOKABLE bool warn();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		const QString& getTitle() const;
		Level getLevel() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setTitle(const QString& value);
		void setLevel(Level value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void titleChanged();
		void levelChanged();
	/** @} */

	/** @name States  */
	/** @{ */
	public:
	/** @} */
	};

	/**
	 * @brief The Alert class
	 * @todo Just one at a time can be headfull (default and enought when singleton is headfull)
	 */
	class Alert : public QAbstractListModel
	{
	/** @name Statics */
	/** @{ */
	public:
		static Alert* Instance();
		static QObject* QInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
	/** @} */

	/** @name Qroperties */
	/** @{ */
		enum class Role : int
		{
			Type = Qt::UserRole + 1,
			Level,
			Name,
			Title
		};

		enum Type
		{
			Sound,
			Effect,
			State,
			Input,
			Action
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(Type)
		Q_PROPERTY(bool headless READ isHeadless WRITE setHeadless NOTIFY headlessChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Alert(QObject* parent = nullptr);
		virtual ~Alert();
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		bool _headless;

		QThread* const _audio_thread;
		QQueue<StateAlert*> _state_queue;
		QQueue<DialogAlert*> _dialog_queue;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		Q_INVOKABLE bool flush();
		Q_INVOKABLE Cadabra::Queen::Core::System::AbstractAlert* find(Type type, const QString& name) const;
		Q_INVOKABLE void sound(const QString& name, Cadabra::Queen::Core::System::AbstractAlert::Level level = AbstractAlert::Level::Info);
		Q_INVOKABLE Cadabra::Queen::Core::System::StateAlert* state(const QString& name, const QString& title, const QString& description, QObject* source = nullptr, Cadabra::Queen::Core::System::AbstractAlert::Level level = AbstractAlert::Level::Info);
		Q_INVOKABLE Cadabra::Queen::Core::System::InputAlert* input(const QString& name, const QString& title, const QString& placeholder = "", int input_flagset = {});
		Q_INVOKABLE Cadabra::Queen::Core::System::ActionAlert* action(const QString& name, const QString& title, const QString& text, int option_flagset = {}, int action_flagset = {}, Cadabra::Queen::Core::System::AbstractAlert::Level level = AbstractAlert::Level::Info);

		bool enqueue(SoundAlert* sound);
		bool enqueue(StateAlert* state);
		bool enqueue(InputAlert* input);
		bool enqueue(ActionAlert* action);

		bool dequeue(StateAlert* state);
		bool dequeue(SoundAlert* sound);
		bool dequeue(InputAlert* input);
		bool dequeue(ActionAlert* action);

	protected:
		void head(StateAlert* state);
		void head(InputAlert* input);
		void head(ActionAlert* action);

	private:
		void queue(DialogAlert* dialog);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		bool getHeadless() const;
		const QQueue<StateAlert*>& getStateQueue() const;
		const QQueue<DialogAlert*>& getDialogQueue() const;

		virtual int rowCount(const QModelIndex& parent) const;
		virtual QVariant data(const QModelIndex& index, int role) const;
		virtual QHash<int, QByteArray> roleNames() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setHeadless(bool onoff);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void headlessChanged();
		void stated(Cadabra::Queen::Core::System::StateAlert* state, bool onoff);
//		void vibrated();
//		void played();
		void inputed(Cadabra::Queen::Core::System::InputAlert* input, bool onoff);
		void actioned(Cadabra::Queen::Core::System::ActionAlert* message, bool onoff);
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isHeadless() const;
		bool isModal(const StateAlert* state) const;
		bool isModal(const DialogAlert* dialog) const;
	/** @} */
	};
} } } }

#endif
