#ifndef Cadabra_Queen_SmartCare_Meta_AnswerModel_hpp
#define Cadabra_Queen_SmartCare_Meta_AnswerModel_hpp

#include <QAbstractListModel>

namespace Cadabra { namespace Queen { namespace SmartCare { namespace Model {
	class AnswerModel : public QAbstractListModel
	{
	/** @name Aliases */
	/** @{ */
	public:
		using Vector = QVector<QPair<QString, QString>>;
	/** @} */

	/** @name Classes */
	/** @{ */
	public:
		enum Role
		{
			AnswerKey = Qt::UserRole + 1,
			AnswerValue
		};
	/** @} */

	/** @name Qroperties */
	/** @{ */
	Q_OBJECT
	/** @} */

	/** @name Constructors */
	/** @{ */
	public:
		AnswerModel(QObject* parent = nullptr);
		AnswerModel(const Model::AnswerModel& answer_model);
	/** @} */

	/** @name Properties */
	/** @{ */
	private:
		Vector _vector;
	/** @} */

	/** @name Procedures */
	/** @{ */
	public:
		void addAnswer(const QString& key, const QString& value);
	/** @} */

	/** @name Getters */
	/** @{ */
	public:
		virtual int rowCount(const QModelIndex& parent) const override;
		virtual QVariant data(const QModelIndex& index, int role) const override;
		virtual QHash<int, QByteArray> roleNames() const override;
	/** @} */
	};
} } } }

#endif
