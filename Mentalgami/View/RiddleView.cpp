#include "RiddleView.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami { namespace View {
	RiddleView::RiddleView(QQuickItem *parent)
	:
		QQuickItem(parent),

		_specifications(Meta::RiddleInformation::Specification::Empty),
		_model(nullptr),
		_is_busy(false)
	{

	}

	QString RiddleView::getQuestion() const
	{
		return _model ? _model->getText() : _question;
	}

	QString RiddleView::getDescription() const
	{
		return _model ? _model->getDescription() : _description;
	}

	QFlags<Meta::RiddleInformation::Specification> RiddleView::getSpecifications() const
	{
		return _model ? _model->getSpecifications() : _specifications;
	}

	const QVector<QUrl>& RiddleView::getMediaList() const
	{
		return _model ? _model->getMediaList() : _media_list;
	}

	Model::AbstractRiddle* RiddleView::getModel() const
	{
		return _model;
	}

	QString RiddleView::getType() const
	{
		return _model ? _model->getType() : QString("Generic");
	}

	void RiddleView::setQuestion(const QString& question)
	{
		if (_model) {
			_model->setText(question);
		} else {
			if (_question != question) {
				_question = question;
				emit questionChanged();
			}
		}
	}

	void RiddleView::setDescription(const QString& description)
	{
		if (_model) {
			_model->setDescription(description);
		} else {
			if (_description != description) {
				_description = description;
				emit descriptionChanged();
			}
		}
	}

	void RiddleView::setSpecifications(const QFlags<Meta::RiddleInformation::Specification>& specifications)
	{
		if (_model) {
			_model->setSpecifications(specifications);
		} else {
			if (_specifications != specifications) {
				_specifications = specifications;
				emit specificationsChanged();
			}
		}
	}

	void RiddleView::setMediaList(const QVector<QUrl>& media_list)
	{
		if (_model) {
			_model->setMediaList(media_list);
		} else {
			if (_media_list != media_list) {
				_media_list = media_list;
				emit mediaListChanged();
			}
		}
	}

	void RiddleView::setModel(Model::AbstractRiddle* model)
	{
		if (_model != model) {
			_model = model;
			emit modelChanged();

			QObject::connect(_model, &Model::AbstractRiddle::textChanged, this, &RiddleView::questionChanged);
			QObject::connect(_model, &Model::AbstractRiddle::descriptionChanged, this, &RiddleView::descriptionChanged);
			QObject::connect(_model, &Model::AbstractRiddle::specificationsChanged, this, &RiddleView::specificationsChanged);
			QObject::connect(_model, &Model::AbstractRiddle::mediaListChanged, this, &RiddleView::mediaListChanged);
			QObject::connect(_model, &Model::AbstractRiddle::busyChanged, this, &RiddleView::busyChanged);
		}
	}

	bool RiddleView::isBusy() const
	{
		return _model ? _model->isBusy() : _is_busy;
	}
} } } }
