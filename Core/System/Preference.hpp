#ifndef Cadabra_Queen_Core_System_Preference_hpp
#define Cadabra_Queen_Core_System_Preference_hpp

#include <QSettings>
#include <QDir>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	class Preference : public QSettings
	{
	/** @name Statics */
	/** @{ */
	public:
		static const QString FileExtension;
		static Preference& GlobalInstance();
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit Preference(const QString& file_name, const QString& default_group = "general");
		virtual ~Preference() = default;
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _group;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void endGroup();
		void clearGroup();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:	
		Q_INVOKABLE QVariant getOption(const QString& key, const QVariant& default_value = QVariant()) const;

		Q_INVOKABLE QString getLastGroup() const;
		Q_INVOKABLE QString group() const;
	/** @} */

	/** @name Mutators */
	/** @{ */
	public:
		Q_INVOKABLE void setOption(const QString& key, const QVariant& value);

		void beginGroup(const QString& name);
		bool setGroup(const QString& name);
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
