#ifndef Cadabra_Queen_Conversation_Model_Message_AttachmentMessage_hpp
#define Cadabra_Queen_Conversation_Model_Message_AttachmentMessage_hpp

#include "../AbstractMessage.hpp"

#include <QUrl>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	class AttachmentMessage : public AbstractMessage
	{
	/** @name Classes */
	/** @{ */
	public:
		enum FileState {
			Unknown,
			Loading,
			Cached
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_ENUM(FileState)
		Q_PROPERTY(QString fileName READ getFileName NOTIFY imported)
		Q_PROPERTY(QString fileDescription READ getFileDescription NOTIFY imported)
		Q_PROPERTY(QUrl cachePath READ getCachePath NOTIFY imported)
		Q_PROPERTY(FileState cacheState READ getFileState NOTIFY cached)
		Q_PROPERTY(bool isCaching READ isCaching NOTIFY cached)
		Q_PROPERTY(bool isCached READ isCached NOTIFY cached)
		Q_PROPERTY(bool isDescribed READ isDescribed NOTIFY imported)
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		AttachmentMessage(QObject* parent = nullptr, Meta::Message::SchemeIdent scheme = -8034428181974876158);
		virtual ~AttachmentMessage() override = default;
	/** @} */

	/** @name Propreties */
	/** @{ */
	private:
		Repository::Meta::ObjectId _object_id;
		QString _file_name;
		QString _file_description;
		FileState _file_state;
		QSharedPointer<Repository::Net::ObjectFile> _file_loader;
	/** @} */

	/** @name Procedures */
	/** @{ */		
	public:
		Q_INVOKABLE bool cache(bool force = false);

	protected:
		virtual QJsonObject exportBody() const override;
		virtual bool importBody(const QJsonObject& data) override;

	private:
		bool validateCache();
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual QString getPreviewText() const override;
		Repository::Meta::ObjectId getObjectId() const;
		QString getFileName() const;
		QString getFileDescription() const;
		FileState getFileState() const;
		QUrl getCachePath() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void cached();
	/** @} */

	/** @name Slots */
	/** @{ */
	private slots:		
		void onFileSaved(QString file_name, int size);
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isCached() const;
		bool isCaching() const;
		bool isDescribed() const;
	/** @} */
	};
} } } }

#endif
