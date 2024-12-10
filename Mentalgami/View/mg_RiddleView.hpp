#ifndef Cadabra_Queen_Mentalgami_View_RiddleView_hpp
#define Cadabra_Queen_Mentalgami_View_RiddleView_hpp

#include "../Meta/mg_RiddleInformation.hpp"

#include "../Model/mg_AbstractRiddle.hpp"

#include <QQuickItem>

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace View {
	class RiddleView : public QQuickItem
	{
	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
		Q_PROPERTY(QString question READ getQuestion WRITE setQuestion NOTIFY questionChanged)
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(QString type READ getType NOTIFY modelChanged)
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Meta::RiddleInformation::Specifications specifications READ getSpecifications WRITE setSpecifications NOTIFY specificationsChanged)
		Q_PROPERTY(QVector<QUrl> mediaList READ getMediaList WRITE setMediaList NOTIFY mediaListChanged)
		Q_PROPERTY(Cadabra::Queen::Mentalgami::Model::AbstractRiddle* model READ getModel WRITE setModel NOTIFY modelChanged)
		Q_PROPERTY(bool isBusy READ isBusy NOTIFY busyChanged)
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		explicit RiddleView(QQuickItem* parent = nullptr);
		virtual ~RiddleView() override = default;
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		QString _question;
		QString _description;
		QFlags<Meta::RiddleInformation::Specification> _specifications;
		QVector<QUrl> _media_list;
		Model::AbstractRiddle* _model;
		bool _is_busy;
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		QString getQuestion() const;
		QString getDescription() const;
		QFlags<Meta::RiddleInformation::Specification> getSpecifications() const;
		const QVector<QUrl>& getMediaList() const;
		Model::AbstractRiddle* getModel() const;
		QString getType() const;
	/** @} */

	/** @name Setters */
	/** @{ */
	public:
		void setQuestion(const QString& question);
		void setDescription(const QString& description);
		void setSpecifications(const QFlags<Meta::RiddleInformation::Specification>& specifications);
		void setMediaList(const QVector<QUrl>& media_list);
		void setModel(Model::AbstractRiddle* model);
	/** @} */

	/** @name Signals */
	/** @{ */
	signals:
		void questionChanged();
		void descriptionChanged();
		void specificationsChanged();
		void mediaListChanged();
		void modelChanged();
		void answered();
		void answerReceived(const QVariant& answer);
		void busyChanged();
	/** @} */

	/** @name States */
	/** @{ */
	public:
		bool isBusy() const;
	/** @} */
	};
} } } }

#endif
