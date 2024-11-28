#include "HealthRequestMessage.hpp"

#include "../../../Core/System/FitnessTracker.hpp"

#include <QJsonObject>

namespace Cadabra { namespace Queen { namespace Conversation { namespace Model {
	HealthRequestMessage::HealthRequestMessage(QObject* parent)
	:
		AbstractMessage(parent)
	{
		QObject::connect(Core::System::FitnessTracker::Instance(), &Core::System::FitnessTracker::registered, this, &HealthRequestMessage::onRegistered);
	}

	Q_INVOKABLE void HealthRequestMessage::requestAccess()
	{
		if (!hasPermission() && hasChannelDescriptor()) {
			if (!Core::System::FitnessTracker::Instance()->requestAccess()) {
				_local_permission = "no";
				emit permissionChanged();
				emit modified();
			}
		}
	}

	QJsonObject HealthRequestMessage::exportBody() const
	{
		return QJsonObject{
							{"comment", _comment},
							{"permission", (_local_permission.contains("yes") || _permission.contains("yes")) ? "yes" : ""},
							{"kit", static_cast<char>(Core::System::FitnessTracker::Instance()->getFamily())},
							{"subject_id", QString::number(_subject_id)}
		};
	}

	bool HealthRequestMessage::importBody(const QJsonObject& body)
	{
		setComment(body["comment"].toString());
		_subject_id = body["subject_id"].toString("0").toLongLong();
		_permission = body["permission"].toString("");
		_local_permission = "";

		if (_permission.contains("yes")) {
			permissionChanged();
		}

		return true;
	}

	QString HealthRequestMessage::getPreviewText() const
	{
		return QString("[%1]").arg(tr("Health Access Request"));
	}

	QString HealthRequestMessage::getComment() const
	{
		return _comment;
	}

	QString HealthRequestMessage::getPermission() const
	{
		return _permission;
	}

	Repository::Meta::Subject::Id HealthRequestMessage::getSubjectId() const
	{
		return _subject_id;
	}

	void HealthRequestMessage::setComment(const QString& comment)
	{
		if (_comment != comment) {
			_comment = comment;
			emit commentChanged();
		}
	}

	void HealthRequestMessage::setPermission(const QString& permission)
	{
		if (_permission != permission) {
			_permission = permission;
			emit permissionChanged();
		}
	}

	Q_INVOKABLE bool HealthRequestMessage::hasPermission() const
	{
		return _permission.contains("yes") || _local_permission.contains("yes");
	}

	void HealthRequestMessage::onRegistered()
	{
		if (/*_permission != permission && */ getChannelDescriptor()->getSubjectId() == _subject_id) {
			_local_permission = "yes";
			emit permissionChanged();
			emit modified();
		}
	}
} } } }
