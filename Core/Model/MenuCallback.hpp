#ifndef Cadabra_Queen_Core_Model_MenuCallback_hpp
#define Cadabra_Queen_Core_Model_MenuCallback_hpp

#include <QObject>

#include <QUrl>
#include <QString>

namespace Cadabra { namespace Queen { namespace Core { namespace Model {
	class MenuCallback : public QObject
	{
	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
		Q_PROPERTY(QString section READ getSection WRITE setSection NOTIFY sectionChanged)
		Q_PROPERTY(QUrl icon READ getIcon WRITE setIcon NOTIFY iconChanged)
		Q_PROPERTY(bool active READ isActivated WRITE setActive NOTIFY activeChanged)
		Q_PROPERTY(bool highlight READ isHighlighted WRITE setHighlight NOTIFY highlightChanged)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		MenuCallback(QObject* parent = nullptr);
		~MenuCallback();
	/** @} */

	/** @name Factories */
	/** @{ */
	public:
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _title;
		QString _section;
		QUrl _icon;
		bool _is_activated;
		bool _is_highlighted;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QString getTitle() const;
		QString getSection() const;
		QUrl getIcon() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:	
		void setTitle(QString value);
		void setSection(QString value);
		void setIcon(QUrl value);
		void setActive(bool value);
		void setHighlight(bool value);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void titleChanged();
		void sectionChanged();
		void iconChanged();
		void activeChanged();
		void highlightChanged();
		void called();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isActivated() const;
		bool isHighlighted() const;
	/** @} */
	};
} } } }
#endif
