#include "Preference.hpp"

#include "Native.hpp"

namespace Cadabra { namespace Queen { namespace Core { namespace System {

#if defined(Q_OS_ANDROID)
	const QString Preference::FileExtension = ".conf";
#elif defined(Q_OS_IOS)
	const QString Preference::FileExtension = ".plist";
#elif defined(Q_OS_MACOS)
	const QString Preference::FileExtension = ".plist";
#else
	const QString Preference::FileExtension = "";
#endif

	Preference& Preference::GlobalInstance()
	{
		static Preference Instance
		(
			Core::System::Native::Instance()->getDataDirectory().absoluteFilePath("Queen").append(Core::System::Preference::FileExtension),
			"default"
		);
		return Instance;
	}

	Preference::Preference(const QString& file_name, const QString& default_group)
	:
		QSettings(
					file_name,
					QSettings::Format::NativeFormat,
					nullptr
		),

		_group(default_group)
	{

	}

	void Preference::beginGroup(const QString& name)
	{
		_group = name;
	}

	void Preference::clearGroup()
	{
		if (_group.isEmpty()) {
			return ;
		}
		remove(_group + "/");
		_group.clear();
	}

	void Preference::endGroup()
	{

	}

	QString Preference::group() const
	{
		return _group;
	}

	bool Preference::setGroup(const QString& name)
	{
		if (!childGroups().contains(name)) {
			return false;
		}

		_group = name;
		return true;
	}

	void Preference::setOption(const QString& key, const QVariant& value)
	{
		return setValue((_group + "/" + key), std::forward<const QVariant&>(value));
	}

	QVariant Preference::getOption(const QString& key, const QVariant& default_value) const
	{
		return value((_group + "/" + key), std::forward<const QVariant&>(default_value));
	}

	QString Preference::getLastGroup() const
	{
		return (childKeys().contains("_")) ? value("_").toString() : _group;
	}
} } } }
