#include "SmartCare.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"

#include "../Conversation/Meta/MessageRender.hpp"

#include "Meta/Course.hpp"
#include "Meta/Track.hpp"
#include "Meta/TrackRender.hpp"

#include "View/TrackLayout.hpp"
#include "View/TrackView.hpp"
#include "View/TrackStackview.hpp"
#include "View/Moji.hpp"

#include "Model/Application/SmartChannel.hpp"
#include "Model/Application/GenericQuiz.hpp"
#include "Model/Application/Quiz/WelcomeQuiz.hpp"
#include "Model/Application/Quiz/BMIQuiz.hpp"
#include "Model/Application/Quiz/IdealWeightQuiz.hpp"
#include "Model/Application/Quiz/WHRQuiz.hpp"
#include "Model/Application/Quiz/CalorieBalanceQuiz.hpp"
#include "Model/Application/SmartCourse.hpp"
#include "Model/Application/EurekaDirectory.hpp"
#include "Model/Application/SmartBook.hpp"

#include "Model/Track/VideoTrack.hpp"
#include "Model/Track/WebTrack.hpp"
#include "Model/Track/HTMLTrack.hpp"
#include "Model/Track/MSExcelTrack.hpp"
#include "Model/Track/MSWordTrack.hpp"
#include "Model/Track/RTFTrack.hpp"
#include "Model/Track/PDFTrack.hpp"
#include "Model/Track/CalendarTrack.hpp"
#include "Model/Track/TelegramTrack.hpp"

#include "Model/Message/SmartCourseMessage.hpp"
#include "Model/Message/AwardMessage.hpp"

#include "Model/Riddle/AuthorizationRiddle.hpp"
#include "Model/Riddle/BoolRiddle.hpp"
#include "Model/Riddle/DateRiddle.hpp"
#include "Model/Riddle/IntegerRiddle.hpp"
#include "Model/Riddle/ListRiddle.hpp"
#include "Model/Riddle/NullRiddle.hpp"
#include "Model/Riddle/NumericRiddle.hpp"
#include "Model/Riddle/TextRiddle.hpp"

#include "View/RiddleView.hpp"
#include "Meta/RiddleRender.hpp"

#include "Controller/Application/QuizFlow.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"

namespace Cadabra { namespace Queen { namespace SmartCare {
	Extension::Extension()
	:
		Queen::Bone("SmartCare")
	{

	}

	Extension::~Extension()
	{

	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(SmartCare::staticMetaObject, "SmartCare.Global", 0, 1, "SmartCare", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<SmartCare::Meta::Track::Scheme>("Cadabra::Queen::SmartCare::Meta::Track::Scheme");
		qRegisterMetaType<SmartCare::Meta::Course>("Cadabra::Queen::SmartCare::Meta::Course");
		qRegisterMetaType<SmartCare::Meta::Track>("Cadabra::Queen::SmartCare::Meta::Track");
		qRegisterMetaType<SmartCare::Meta::TrackRender>("Cadabra::Queen::SmartCare::Meta::TrackRender");
		qRegisterMetaType<SmartCare::TrackRenderLayout>("Cadabra::Queen::SmartCare::TrackRenderLayout");

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
		qmlRegisterType<SmartCare::View::Moji>("SmartCare.View", 0, 1, "Moji");
		qmlRegisterType<SmartCare::View::TrackLayout>("SmartCare.View", 0, 1, "TrackLayout");
		qmlRegisterType<SmartCare::View::TrackView>("SmartCare.View", 0, 1, "TrackView");
		qmlRegisterType<SmartCare::View::TrackStackview>("SmartCare.View", 0, 1, "TrackStackview");
		qmlRegisterAnonymousType<SmartCare::Meta::Track>("SmartCare.Meta", 0);
		qmlRegisterAnonymousType<SmartCare::Meta::Course>("SmartCare.Meta", 0);
		qmlRegisterAnonymousType<SmartCare::Meta::Track>("SmartCare.Meta", 0);

		qmlRegisterAnonymousType<SmartCare::Model::AbstractTrack>("SmartCare.Model", 0);
		qmlRegisterType<SmartCare::Model::VideoTrack>("SmartCare.Model", 0, 1, "VideoTrack");
//		qmlRegisterType<SmartCare::Model::AudioTrack>("SmartCare.Model", 0, 1, "AudioTrack");
		qmlRegisterType<SmartCare::Model::WebTrack>("SmartCare.Model", 0, 1, "WebTrack");
		qmlRegisterType<SmartCare::Model::HTMLTrack>("SmartCare.Model", 0, 1, "HTMLTrack");
		qmlRegisterType<SmartCare::Model::MSExcelTrack>("SmartCare.Model", 0, 1, "MSExcelTrack");
		qmlRegisterType<SmartCare::Model::MSWordTrack>("SmartCare.Model", 0, 1, "MSWordTrack");
		qmlRegisterType<SmartCare::Model::RTFTrack>("SmartCare.Model", 0, 1, "RTFTrack");
		qmlRegisterType<SmartCare::Model::PDFTrack>("SmartCare.Model", 0, 1, "PDFTrack");
		qmlRegisterType<SmartCare::Model::CalendarTrack>("SmartCare.Model", 0, 1, "CalendarTrack");
		qmlRegisterType<SmartCare::Model::TelegramTrack>("SmartCare.Model", 0, 1, "TelegramTrack");

		qmlRegisterType<SmartCare::Model::SmartCourse>("SmartCare.Model", 0, 1, "SmartCourse");
		qmlRegisterType<SmartCare::Model::SmartChannel>("SmartCare.Model", 0, 1, "SmartChannel");
		qmlRegisterType<SmartCare::Model::GenericQuiz>("SmartCare.Model", 0, 1, "GenericQuiz");
		qmlRegisterType<SmartCare::Model::WelcomeQuiz>("SmartCare.Model", 0, 1, "WelcomeQuiz");		
		qmlRegisterType<SmartCare::Model::SmartBook>("SmartCare.Model", 0, 1, "SmartBook");

		qmlRegisterType<SmartCare::Model::SmartCourseMessage>("SmartCare.Model", 0, 1, "SmartCourseMessage");
		qmlRegisterType<SmartCare::Model::AwardMessage>("SmartCare.Model", 0, 1, "AwardMessage");

		qmlRegisterUncreatableType<SmartCare::Model::AbstractRiddle>("SmartCare.Model", 0, 1, "AbstractRiddle", "AbstractRiddle is an abstract type and cannot be created");
		qmlRegisterType<SmartCare::Model::AuthorizationRiddle>("SmartCare.Model", 0, 1, "AuthorizationRiddle");
		qmlRegisterType<SmartCare::Model::BoolRiddle>("SmartCare.Model", 0, 1, "BoolRiddle");
		qmlRegisterType<SmartCare::Model::DateRiddle>("SmartCare.Model", 0, 1, "DateRiddle");
		qmlRegisterType<SmartCare::Model::IntegerRiddle>("SmartCare.Model", 0, 1, "IntegerRiddle");
		qmlRegisterType<SmartCare::Model::ListRiddle>("SmartCare.Model", 0, 1, "ListRiddle");
		qmlRegisterType<SmartCare::Model::NullRiddle>("SmartCare.Model", 0, 1, "NullRiddle");
		qmlRegisterType<SmartCare::Model::NumericRiddle>("SmartCare.Model", 0, 1, "NumericRiddle");
		qmlRegisterType<SmartCare::Model::TextRiddle>("SmartCare.Model", 0, 1, "TextRiddle");
		qmlRegisterType<SmartCare::Meta::RiddleInformation>("SmartCare.Meta", 0, 1, "RiddleInformation");

		qmlRegisterType<SmartCare::View::RiddleView>("SmartCare.View", 0, 1, "RiddleView");

		qmlRegisterType<SmartCare::Controller::QuizFlow>("SmartCare.Controller", 0, 1, "QuizFlow");
		qmlRegisterUncreatableType<SmartCare::Controller::QuizFlowAttachment>("SmartCare.Controller", 0, 1, "QuizFlowAttachment", "QuizFlowAttachment is only available as an attached property");        
		return true;
	}

	bool Extension::registerAceAction()
	{
		return true;
	}

	bool Extension::registerAceScheme()
	{
		/// @note Application scheme
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::SmartCourse>(3, {"SmartCare", "SmartCourse", 0, 1, "CoursePlaylist"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::SmartChannel>(5, {"SmartCare", "SmartChannel", 0, 1, "Conversation/View/Application/ChannelFeed"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::EurekaDirectory>(6, {"SmartCare", "EurekaDirectory", 0, 1, "EurekaDirectory"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::GenericQuiz>(10, {"SmartCare", "GenericQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::WelcomeQuiz>(14, {"SmartCare", "WelcomeQuiz", 0, 1, "AuthorizationQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::BMIQuiz>(15, {"SmartCare", "BMIQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::IdealWeightQuiz>(16, {"SmartCare", "IdealWeightQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::WHRQuiz>(17, {"SmartCare", "WHRQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::CalorieBalanceQuiz>(18, {"SmartCare", "CalorieBalanceQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::GenericQuiz>(19, {"SmartCare", "DEBQuiz", 0, 1, "PatientQuiz"});
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<SmartCare::Model::GenericQuiz>(20, {"SmartCare", "EA26Quiz", 0, 1, "PatientQuiz"});

		/// @note Riddle renderers
		qRegisterMetaType<SmartCare::Meta::RiddleRender>("Cadabra::Queen::SmartCare::Meta::RiddleRender");
		Core::Meta::SchemeFactory::Bind<SmartCare::Meta::RiddleRender>("SmartCare.Meta.RiddleRender");

		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::BoolRiddle>(0, {"SmartCare", 0, 1, "BoolRiddle"});
		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::DateRiddle>(1, {"SmartCare", 0, 1, "DateRiddle"});
		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::IntegerRiddle>(2, {"SmartCare", 0, 1, "IntegerRiddle"});
		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::ListRiddle>(3, {"SmartCare", 0, 1, "ListRiddle"});
		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::NullRiddle>(4, {"SmartCare", 0, 1, "NullRiddle"});
		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::NumericRiddle>(5, {"SmartCare", 0, 1, "NumericRiddle"});
		Core::Meta::Scheme<SmartCare::Meta::RiddleRender>::Validate<SmartCare::Model::TextRiddle>(6, {"SmartCare", 0, 1, "TextRiddle"});

		/// @note Track renderers
		qRegisterMetaType<SmartCare::Meta::TrackRender>("Cadabra::Queen::SmartCare::Meta::TrackRender");
		Core::Meta::SchemeFactory::Bind<SmartCare::Meta::TrackRender>("SmartCare.Meta.TrackRender");
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::VideoTrack>(1, {"SmartCare", 0, 1, "VideoTrack", {SmartCare::TrackRenderLayout::StackviewLayout}, tr("Video file")});
//		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::AudioTrack>(2, {"SmartCare", 0, 1, "AudioTrack", {SmartCare::TrackRenderLayout::StackviewLayout}, tr("Audio file")});
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::WebTrack>(9, {"SmartCare", 0, 1, "WebTrack", {SmartCare::TrackRenderLayout::StackviewLayout}, tr("Web application")});

		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::HTMLTrack>(10, {"SmartCare", 0, 1, "HTMLTrack", {SmartCare::TrackRenderLayout::StackviewLayout}, tr("Web page")});
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::MSExcelTrack>(11, {"SmartCare", 0, 1, "MSExcelTrack", {/*SmartCare::TrackRenderLayout::PopupviewLayout*/}, tr("MSExcel spreadsheet")});
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::MSWordTrack>(12, {"SmartCare", 0, 1, "MSWordTrack", {/*SmartCare::TrackRenderLayout::PopupviewLayout*/}, tr("MSWord document")});
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::RTFTrack>(13, {"SmartCare", 0, 1, "RTFTrack", {/*SmartCare::TrackRenderLayout::PopupviewLayout*/}, tr("Richtext document")});
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::PDFTrack>(14, {"SmartCare", 0, 1, "PDFTrack", {/*SmartCare::TrackRenderLayout::PopupviewLayout*/}, tr("Portable document")});

		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::CalendarTrack>(20, {"SmartCare", 0, 1, "CalendarTrack", {/*SmartCare::TrackRenderLayout::PopupviewLayout*/}, tr("Calendar event")});
		Core::Meta::Scheme<SmartCare::Meta::TrackRender>::Validate<SmartCare::Model::TelegramTrack>(21, {"SmartCare", 0, 1, "TelegramTrack", {/*SmartCare::TrackRenderLayout::PopupviewLayout*/}, tr("Telegram channel")});

		/// @note Message renderers
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<SmartCare::Model::SmartCourseMessage>(3, {"SmartCare", 0, 1, "SmartCourseMessage", {Conversation::RenderLayout::VerticalLayout}, tr("Courses")});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<SmartCare::Model::AwardMessage>(4, {"SmartCare", 0, 1, "AwardMessage", {Conversation::RenderLayout::GridLayout}, tr("Awards")});

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
