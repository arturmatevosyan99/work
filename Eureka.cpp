#include "Eureka.hpp"

#include "Core/Core.hpp"
#include "Repository/Repository.hpp"
#include "Conversation/Conversation.hpp"
#include "ERP/ERP.hpp"
#include "SD/SD.hpp"
#include "EC/EC.hpp"
#include "Mentalgami/Mentalgami.hpp"
#include "DrOblozhko/DrOblozhko.hpp"

#include "Core/View/Theme.hpp"
#include "Core/System/UserTracker.hpp"

int SkeletonMain(int& argc, char** argv)
{
	return Cadabra::Queen::Eureka(argc, argv).run();
}

namespace Cadabra { namespace Queen {
	Eureka::Eureka(int& argc, char** argv, int flags)
	:
		Skeleton("eureka", argc, argv, flags)
	{
		QObject::connect(Core::System::Native::Instance(), &Core::System::Native::applicationStateChanged, this, &Eureka::onApplicationStateChanged);
	}

	bool Eureka::tune()
	{
		if (!Skeleton::tune()) {
			return false;
		}

		Core::View::Theme::Instance()->load("Guiness", Core::View::Theme::Light);

		Core::System::AudioRole::Instance()->store(Core::System::AudioRole::Mode::Playback, Core::System::AudioRole::Mix);

		Core::System::UserTracker::Lock(Skeleton::DebugMode);

		return true;
	}

	bool Eureka::extend()
	{
		Skeleton::extend(new Core::Extension());
		Skeleton::extend(new Repository::Extension());
		Skeleton::extend(new Conversation::Extension());
		Skeleton::extend(new ERP::Extension());
		Skeleton::extend(new SD::Extension());
		Skeleton::extend(new EC::Extension());
		Skeleton::extend(new Mentalgami::Extension());
		Skeleton::extend(new DrOblozhko::Extension());

		return true;
	}

	int Eureka::boot()
	{
		return Skeleton::boot(QUrl(QStringLiteral("qrc:/Eureka.qml")));
	}

	void Eureka::onApplicationStateChanged(Core::ApplicationState previous, Core::ApplicationState current)
	{
		if (current != previous && Core::ApplicationState::ActiveApplication == current && !Core::System::AudioRole::Instance()->hasMonitor()) {
			Core::System::AudioRole::Instance()->restore();
		}
	}
} }
