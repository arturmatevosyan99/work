#include "DrOblozhko.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"
#include "../Repository/Meta/JobSpecification.hpp"
#include "../Conversation/Meta/MessageRender.hpp"

#include "Model/Appointment.hpp"

#include "Model/Application/AppointmentChannel.hpp"
#include "Model/Application/Reception.hpp"
#include "Model/Application/Webinar.hpp"

#include "Model/Application/Quiz/MetabolicPlateLengthQuiz.hpp"

#include "Model/Job/Cleanup.hpp"

#include "Model/Message/InteractiveApplicationMessage.hpp"
#include "Model/Message/VideoLessonMessage.hpp"
#include "Model/Message/HandBookMessage.hpp"
#include "Model/Message/WebinarMessage.hpp"

namespace Cadabra { namespace Queen { namespace DrOblozhko {
	Extension::Extension()
	:
		Queen::Bone("droblozhko")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaType()
	{
		return true;
	}

	bool Extension::registerMetaTypeConverter()
	{
		return true;
	}

	bool Extension::registerMetaTypeStreamOperator()
	{
		return true;
	}

	bool Extension::registerQMLType()
	{
		qmlRegisterType<DrOblozhko::Model::Reception>("DrOblozhko.Model", 0, 1, "Reception");
		qmlRegisterType<DrOblozhko::Model::Appointment>("DrOblozhko.Model", 0, 1, "Appointment");
		qmlRegisterType<DrOblozhko::Model::Webinar>("DrOblozhko.Model", 0, 1, "Webinar");

		qmlRegisterType<DrOblozhko::Model::InteractiveApplicationMessage>("DrOblozhko.Model", 0, 1, "InteractiveApplicationMessage");
		qmlRegisterType<DrOblozhko::Model::VideoLessonMessage>("DrOblozhko.Model", 0, 1, "VideoLessonMessage");
		qmlRegisterType<DrOblozhko::Model::HandBookMessage>("DrOblozhko.Model", 0, 1, "HandBookMessage");
		qmlRegisterType<DrOblozhko::Model::WebinarMessage>("DrOblozhko.Model", 0, 1, "WebinarMessage");

		return true;
	}

	bool Extension::registerAceAction()
	{
		return true;
	}

	bool Extension::registerAceScheme()
	{
		/// @note Application renderers
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<DrOblozhko::Model::Webinar>(20, {"DrOblozhko", "Webinar", 0, 1, "Webinar"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<DrOblozhko::Model::MetabolicPlateLengthQuiz>(21, {"DrOblozhko", "MetabolicPlateLengthQuiz", 0, 1, "Mentalgami/View/Application/PatientQuiz"});

		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<DrOblozhko::Model::AppointmentChannel>(1, {"DrOblozhko", "AppointmentChannel", 0, 1, "Conversation/View/Application/ChannelFeed"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<DrOblozhko::Model::Reception>(2, {"DrOblozhko", "Reception", 0, 1, "ReceptionOffice"});
		/// @??? Что делать со 1+n приложениями для одного объекта с какой-то одной моделью -> MultiMap в Core::Meta::Scheme
//		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<DrOblozhko::Model::Reception>(2, {"DrOblozhko", "Reception", 0, 1, "ReceptionLobby"});

		/// @note Job specifications
		Core::Meta::Scheme<Repository::Meta::JobSpecification>::Validate<DrOblozhko::Model::Cleanup>(1, {"DrOblozhko", "Cleanup"});

		/// @note Message renderers
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<DrOblozhko::Model::VideoLessonMessage>(10, {"DrOblozhko", 0, 1, "VideoLessonMessage", {Conversation::RenderLayout::VerticalLayout}, tr("Lessons")});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<DrOblozhko::Model::HandBookMessage>(11, {"DrOblozhko", 0, 1, "HandBookMessage", {Conversation::RenderLayout::VerticalLayout}, tr("Handbooks")});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<DrOblozhko::Model::InteractiveApplicationMessage>(12, {"DrOblozhko", 0, 1, "InteractiveApplicationMessage", {Conversation::RenderLayout::VerticalLayout}, tr("Applications")});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<DrOblozhko::Model::WebinarMessage>(20, {"DrOblozhko", 0, 1, "WebinarMessage", {Conversation::RenderLayout::VerticalLayout}, tr("Webinars")});

		return true;
	}

	bool Extension::registerQMLSingletonType()
	{
		return true;
	}

	bool Extension::tweakRootContext(QQmlContext& context)
	{
		return true;
	}
} } }
