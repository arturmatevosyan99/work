#include "Mentalgami.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"

#include "../Conversation/Meta/MessageRender.hpp"

#include "Meta/mg_Course.hpp"
#include "Meta/mg_Track.hpp"
#include "Meta/mg_TrackRender.hpp"

#include "View/mg_TrackLayout.hpp"
#include "View/mg_TrackView.hpp"
#include "View/mg_TrackStackview.hpp"
#include "View/mg_Moji.hpp"

#include "Model/Application/mg_SmartChannel.hpp"
#include "Model/Application/mg_GenericQuiz.hpp"
#include "Model/Application/Quiz/mg_WelcomeQuiz.hpp"
#include "Model/Application/Quiz/mg_BMIQuiz.hpp"
#include "Model/Application/Quiz/mg_IdealWeightQuiz.hpp"
#include "Model/Application/Quiz/mg_WHRQuiz.hpp"
#include "Model/Application/Quiz/mg_CalorieBalanceQuiz.hpp"
#include "Model/Application/mg_SmartCourse.hpp"
#include "Model/Application/mg_EurekaDirectory.hpp"
#include "Model/Track/mg_VideoTrack.hpp"
#include "Model/Track/mg_WebTrack.hpp"
#include "Model/Track/mg_HTMLTrack.hpp"
#include "Model/Track/mg_MSExcelTrack.hpp"
#include "Model/Track/mg_MSWordTrack.hpp"
#include "Model/Track/mg_RTFTrack.hpp"
#include "Model/Track/mg_PDFTrack.hpp"
#include "Model/Track/mg_CalendarTrack.hpp"
#include "Model/Track/mg_TelegramTrack.hpp"

#include "Model/Message/mg_SmartCourseMessage.hpp"
#include "Model/Message/mg_AwardMessage.hpp"

#include "Model/Riddle/mg_AuthorizationRiddle.hpp"
#include "Model/Riddle/mg_BoolRiddle.hpp"
#include "Model/Riddle/mg_DateRiddle.hpp"
#include "Model/Riddle/mg_IntegerRiddle.hpp"
#include "Model/Riddle/mg_ListRiddle.hpp"
#include "Model/Riddle/mg_NullRiddle.hpp"
#include "Model/Riddle/mg_NumericRiddle.hpp"
#include "Model/Riddle/mg_TextRiddle.hpp"

#include "View/mg_RiddleView.hpp"
#include "Meta/mg_RiddleRender.hpp"

#include "Controller/Application/mg_QuizFlow.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"

namespace Cadabra { namespace Queen { namespace Mentalgami {
	Extension::Extension()
	:
		Queen::Bone("Mentalgami")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(Mentalgami::staticMetaObject, "Mentalgami.Global", 0, 1, "Mentalgami", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<Mentalgami::Meta::Track::Scheme>("Cadabra::Queen::Mentalgami::Meta::Track::Scheme");
		qRegisterMetaType<Mentalgami::Meta::Course>("Cadabra::Queen::Mentalgami::Meta::Course");
		qRegisterMetaType<Mentalgami::Meta::Track>("Cadabra::Queen::Mentalgami::Meta::Track");
		qRegisterMetaType<Mentalgami::Meta::TrackRender>("Cadabra::Queen::Mentalgami::Meta::TrackRender");
		qRegisterMetaType<Mentalgami::TrackRenderLayout>("Cadabra::Queen::Mentalgami::TrackRenderLayout");

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
		qmlRegisterType<Mentalgami::View::Moji>("Mentalgami.View", 0, 1, "Moji");
		qmlRegisterType<Mentalgami::View::TrackLayout>("Mentalgami.View", 0, 1, "TrackLayout");
		qmlRegisterType<Mentalgami::View::TrackView>("Mentalgami.View", 0, 1, "TrackView");
		qmlRegisterType<Mentalgami::View::TrackStackview>("Mentalgami.View", 0, 1, "TrackStackview");
		qmlRegisterAnonymousType<Mentalgami::Meta::Track>("Mentalgami.Meta", 0);
		qmlRegisterAnonymousType<Mentalgami::Meta::Course>("Mentalgami.Meta", 0);
		qmlRegisterAnonymousType<Mentalgami::Meta::Track>("Mentalgami.Meta", 0);

		qmlRegisterAnonymousType<Mentalgami::Model::AbstractTrack>("Mentalgami.Model", 0);
		qmlRegisterType<Mentalgami::Model::VideoTrack>("Mentalgami.Model", 0, 1, "VideoTrack");
//		qmlRegisterType<Mentalgami::Model::AudioTrack>("Mentalgami.Model", 0, 1, "AudioTrack");
		qmlRegisterType<Mentalgami::Model::WebTrack>("Mentalgami.Model", 0, 1, "WebTrack");
		qmlRegisterType<Mentalgami::Model::HTMLTrack>("Mentalgami.Model", 0, 1, "HTMLTrack");
		qmlRegisterType<Mentalgami::Model::MSExcelTrack>("Mentalgami.Model", 0, 1, "MSExcelTrack");
		qmlRegisterType<Mentalgami::Model::MSWordTrack>("Mentalgami.Model", 0, 1, "MSWordTrack");
		qmlRegisterType<Mentalgami::Model::RTFTrack>("Mentalgami.Model", 0, 1, "RTFTrack");
		qmlRegisterType<Mentalgami::Model::PDFTrack>("Mentalgami.Model", 0, 1, "PDFTrack");
		qmlRegisterType<Mentalgami::Model::CalendarTrack>("Mentalgami.Model", 0, 1, "CalendarTrack");
		qmlRegisterType<Mentalgami::Model::TelegramTrack>("Mentalgami.Model", 0, 1, "TelegramTrack");

		qmlRegisterType<Mentalgami::Model::SmartCourse>("Mentalgami.Model", 0, 1, "SmartCourse");
		qmlRegisterType<Mentalgami::Model::SmartChannel>("Mentalgami.Model", 0, 1, "SmartChannel");
		qmlRegisterType<Mentalgami::Model::GenericQuiz>("Mentalgami.Model", 0, 1, "GenericQuiz");
		qmlRegisterType<Mentalgami::Model::WelcomeQuiz>("Mentalgami.Model", 0, 1, "WelcomeQuiz");

		qmlRegisterType<Mentalgami::Model::SmartCourseMessage>("Mentalgami.Model", 0, 1, "SmartCourseMessage");
		qmlRegisterType<Mentalgami::Model::AwardMessage>("Mentalgami.Model", 0, 1, "AwardMessage");

		qmlRegisterUncreatableType<Mentalgami::Model::AbstractRiddle>("Mentalgami.Model", 0, 1, "AbstractRiddle", "AbstractRiddle is an abstract type and cannot be created");
		qmlRegisterType<Mentalgami::Model::AuthorizationRiddle>("Mentalgami.Model", 0, 1, "AuthorizationRiddle");
		qmlRegisterType<Mentalgami::Model::BoolRiddle>("Mentalgami.Model", 0, 1, "BoolRiddle");
		qmlRegisterType<Mentalgami::Model::DateRiddle>("Mentalgami.Model", 0, 1, "DateRiddle");
		qmlRegisterType<Mentalgami::Model::IntegerRiddle>("Mentalgami.Model", 0, 1, "IntegerRiddle");
		qmlRegisterType<Mentalgami::Model::ListRiddle>("Mentalgami.Model", 0, 1, "ListRiddle");
		qmlRegisterType<Mentalgami::Model::NullRiddle>("Mentalgami.Model", 0, 1, "NullRiddle");
		qmlRegisterType<Mentalgami::Model::NumericRiddle>("Mentalgami.Model", 0, 1, "NumericRiddle");
		qmlRegisterType<Mentalgami::Model::TextRiddle>("Mentalgami.Model", 0, 1, "TextRiddle");

		qmlRegisterType<Mentalgami::Meta::RiddleInformation>("Mentalgami.Meta", 0, 1, "RiddleInformation");

		qmlRegisterType<Mentalgami::View::RiddleView>("Mentalgami.View", 0, 1, "RiddleView");

		qmlRegisterType<Mentalgami::Controller::QuizFlow>("Mentalgami.Controller", 0, 1, "QuizFlow");
		qmlRegisterUncreatableType<Mentalgami::Controller::QuizFlowAttachment>("Mentalgami.Controller", 0, 1, "QuizFlowAttachment", "QuizFlowAttachment is only available as an attached property");

		return true;
	}

	bool Extension::registerAceAction()
	{
		return true;
	}

	bool Extension::registerAceScheme()
	{
		/// @note Application scheme
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::SmartCourse>(3, {"Mentalgami", "SmartCourse", 0, 1, "CoursePlaylist"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::SmartChannel>(5, {"Mentalgami", "SmartChannel", 0, 1, "Conversation/View/Application/ChannelFeed"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::EurekaDirectory>(6, {"Mentalgami", "EurekaDirectory", 0, 1, "EurekaDirectory"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::GenericQuiz>(10, {"Mentalgami", "GenericQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::WelcomeQuiz>(14, {"Mentalgami", "WelcomeQuiz", 0, 1, "AuthorizationQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::BMIQuiz>(15, {"Mentalgami", "BMIQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::IdealWeightQuiz>(16, {"Mentalgami", "IdealWeightQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::WHRQuiz>(17, {"Mentalgami", "WHRQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::CalorieBalanceQuiz>(18, {"Mentalgami", "CalorieBalanceQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::GenericQuiz>(19, {"Mentalgami", "DEBQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Mentalgami::Model::GenericQuiz>(20, {"Mentalgami", "EA26Quiz", 0, 1, "PatientQuiz"});

		/// @note Riddle renderers
		qRegisterMetaType<Mentalgami::Meta::RiddleRender>("Cadabra::Queen::Mentalgami::Meta::RiddleRender");
		Core::Meta::SchemeFactory::Bind<Mentalgami::Meta::RiddleRender>("Mentalgami.Meta.RiddleRender");

		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::BoolRiddle>(0, {"Mentalgami", 0, 1, "BoolRiddle"});
		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::DateRiddle>(1, {"Mentalgami", 0, 1, "DateRiddle"});
		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::IntegerRiddle>(2, {"Mentalgami", 0, 1, "IntegerRiddle"});
		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::ListRiddle>(3, {"Mentalgami", 0, 1, "ListRiddle"});
		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::NullRiddle>(4, {"Mentalgami", 0, 1, "NullRiddle"});
		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::NumericRiddle>(5, {"Mentalgami", 0, 1, "NumericRiddle"});
		Core::Meta::Scheme<Mentalgami::Meta::RiddleRender>::Validate<Mentalgami::Model::TextRiddle>(6, {"Mentalgami", 0, 1, "TextRiddle"});

		/// @note Track renderers
		qRegisterMetaType<Mentalgami::Meta::TrackRender>("Cadabra::Queen::Mentalgami::Meta::TrackRender");
		Core::Meta::SchemeFactory::Bind<Mentalgami::Meta::TrackRender>("Mentalgami.Meta.TrackRender");
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::VideoTrack>(1, {"Mentalgami", 0, 1, "VideoTrack", {Mentalgami::TrackRenderLayout::StackviewLayout}, tr("Video file")});
//		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::AudioTrack>(2, {"Mentalgami", 0, 1, "AudioTrack", {Mentalgami::TrackRenderLayout::StackviewLayout}, tr("Audio file")});
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::WebTrack>(9, {"Mentalgami", 0, 1, "WebTrack", {Mentalgami::TrackRenderLayout::StackviewLayout}, tr("Web application")});

		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::HTMLTrack>(10, {"Mentalgami", 0, 1, "HTMLTrack", {Mentalgami::TrackRenderLayout::StackviewLayout}, tr("Web page")});
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::MSExcelTrack>(11, {"Mentalgami", 0, 1, "MSExcelTrack", {/*Mentalgami::TrackRenderLayout::PopupviewLayout*/}, tr("MSExcel spreadsheet")});
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::MSWordTrack>(12, {"Mentalgami", 0, 1, "MSWordTrack", {/*Mentalgami::TrackRenderLayout::PopupviewLayout*/}, tr("MSWord document")});
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::RTFTrack>(13, {"Mentalgami", 0, 1, "RTFTrack", {/*Mentalgami::TrackRenderLayout::PopupviewLayout*/}, tr("Richtext document")});
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::PDFTrack>(14, {"Mentalgami", 0, 1, "PDFTrack", {/*Mentalgami::TrackRenderLayout::PopupviewLayout*/}, tr("Portable document")});

		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::CalendarTrack>(20, {"Mentalgami", 0, 1, "CalendarTrack", {/*Mentalgami::TrackRenderLayout::PopupviewLayout*/}, tr("Calendar event")});
		Core::Meta::Scheme<Mentalgami::Meta::TrackRender>::Validate<Mentalgami::Model::TelegramTrack>(21, {"Mentalgami", 0, 1, "TelegramTrack", {/*Mentalgami::TrackRenderLayout::PopupviewLayout*/}, tr("Telegram channel")});

		/// @note Message renderers
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Mentalgami::Model::SmartCourseMessage>(3, {"Mentalgami", 0, 1, "SmartCourseMessage", {Conversation::RenderLayout::VerticalLayout}, tr("Courses")});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Mentalgami::Model::AwardMessage>(4, {"Mentalgami", 0, 1, "AwardMessage", {Conversation::RenderLayout::GridLayout}, tr("Awards")});

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
