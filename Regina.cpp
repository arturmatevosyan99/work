#include "Regina.hpp"

#include "Core/Core.hpp"
#include "Repository/Repository.hpp"
#include "Conversation/Conversation.hpp"
#include "ERP/ERP.hpp"
#include "SD/SD.hpp"
#include "EC/EC.hpp"
#include "SmartCare/SmartCare.hpp"
#include "Mentalgami/Mentalgami.hpp"
#include "DrOblozhko/DrOblozhko.hpp"

#include "Core/View/Theme.hpp"

int SkeletonMain(int& argc, char** argv)
{
	return Cadabra::Queen::Regina(argc, argv).run();
}

namespace Cadabra { namespace Queen {
	Regina::Regina(int& argc, char** argv, int flags)
	:
		Skeleton("regina", argc, argv, flags)
	{
		QObject::connect(Core::System::Native::Instance(), &Core::System::Native::applicationStateChanged, this, &Regina::onApplicationStateChanged);
	}

	bool Regina::tune()
	{
		if (!Skeleton::tune()) {
			return false;
		}

		Core::View::Theme::Instance()->load("Guiness", Core::View::Theme::Light);

		Core::System::AudioRole::Instance()->store(Core::System::AudioRole::Mode::Playback, Core::System::AudioRole::Mix);

		return true;
	}

	bool Regina::extend()
	{
		Skeleton::extend(new Core::Extension());
		Skeleton::extend(new Repository::Extension());
		Skeleton::extend(new Conversation::Extension());
		Skeleton::extend(new ERP::Extension());
		Skeleton::extend(new SD::Extension());
		Skeleton::extend(new EC::Extension());
		Skeleton::extend(new SmartCare::Extension());
		Skeleton::extend(new Mentalgami::Extension());
		Skeleton::extend(new DrOblozhko::Extension());

		return true;
	}

	int Regina::boot()
	{
		return Skeleton::boot(QUrl(QStringLiteral("qrc:/Regina.qml")));
	}

	void Regina::onApplicationStateChanged(Core::ApplicationState previous, Core::ApplicationState current)
	{
		if (current != previous && Core::ApplicationState::ActiveApplication == current && !Core::System::AudioRole::Instance()->hasMonitor()) {
			Core::System::AudioRole::Instance()->restore();
		}
	}
} }
