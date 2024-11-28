#include "Conversation.hpp"

#include "../Repository/Meta/ApplicationRender.hpp"
#include "../Repository/Meta/JobSpecification.hpp"
#include "../Repository/Meta/SetupDependency.hpp"

#include "Meta/Message.hpp"
#include "Meta/MessageRender.hpp"

#include "Meta/Assist.hpp"
#include "Meta/AssistRender.hpp"

#include "Model/Application/Air.hpp"
#include "Model/Application/GroupChannel.hpp"

#include "Model/Job/Spread.hpp"

#include "View/MessageLayout.hpp"
#include "View/MessageView.hpp"
#include "View/AssistLayout.hpp"
#include "View/AssistView.hpp"

#include "Model/Message/AppletMessage.hpp"
#include "Model/Message/WithdrawalMessage.hpp"
#include "Model/Message/TextMessage.hpp"
#include "Model/Message/PollMessage.hpp"
#include "Model/Message/PictureMessage.hpp"
#include "Model/Message/HyperlinkMessage.hpp"
#include "Model/Message/YoutubeMessage.hpp"
#include "Model/Message/PushRequestMessage.hpp"
#include "Model/Message/HealthRequestMessage.hpp"
#include "Model/Message/AttachmentMessage.hpp"
#include "Model/Message/VideoscopeMessage.hpp"
#include "Model/Message/VoiceMessage.hpp"
#include "Model/Message/CommandMessage.hpp"
#include "Model/Message/EchoMessage.hpp"
#include "Model/Message/PDFMessage.hpp"
#include "Model/Message/AuthorizationMessage.hpp"

#include "Model/Assist/Layout/GridLayout.hpp"
#include "Model/Assist/Layout/VerticalLayout.hpp"
#include "Model/Assist/Layout/CarouselLayout.hpp"
#include "Model/Assist/TextPush.hpp"
#include "Model/Assist/HyperLink.hpp"

#include "Model/Accident/MessageTransfer.hpp"
#include "Model/Accident/MessageDelivery.hpp"
#include "Model/Action/BroadcastAction.hpp"

#include "Data/Setup/Revision1Setup.hpp"

namespace Cadabra { namespace Queen { namespace Conversation {
	Extension::Extension()
	:
		Queen::Bone("conversation")
	{

	}

	Extension::~Extension()
	{
	}

	bool Extension::registerMetaObject()
	{
		qmlRegisterUncreatableMetaObject(Conversation::staticMetaObject, "Conversation.Global", 0, 1, "Conversation", "Access to enums & flags only");

		return true;
	}

	bool Extension::registerMetaType()
	{
		qRegisterMetaType<Conversation::Meta::Message::Id>("Cadabra::Queen::Conversation::Meta::Message::Id");
		qRegisterMetaType<Conversation::Meta::Message::Scheme>("Cadabra::Queen::Conversation::Meta::Message::Scheme");
		qRegisterMetaType<Conversation::Meta::Message>("Cadabra::Queen::Conversation::Meta::Message");
		qRegisterMetaType<Conversation::Meta::MessageRender>("Cadabra::Queen::Conversation::Meta::MessageRender");
		qRegisterMetaType<Conversation::Meta::AssistRender>("Cadabra::Queen::Conversation::Meta::AssistRender");
		qRegisterMetaType<Conversation::Meta::Assist::Scheme>("Cadabra::Queen::Conversation::Meta::Assist::Scheme");
		qRegisterMetaType<Conversation::RenderLayout>("Cadabra::Queen::Conversation::RenderLayout");

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
		qmlRegisterType<Conversation::View::MessageLayout>("Conversation.View", 0, 1, "MessageLayout");
		qmlRegisterType<Conversation::View::MessageView>("Conversation.View", 0, 1, "MessageView");
		qmlRegisterType<Conversation::View::AssistLayout>("Conversation.View", 0, 1, "AssistLayout");
		qmlRegisterType<Conversation::View::AssistView>("Conversation.View", 0, 1, "AssistView");
		qmlRegisterAnonymousType<Conversation::Meta::Message>("Conversation.Meta", 0);
		qmlRegisterAnonymousType<Conversation::Meta::Assist>("Conversation.Meta", 0);

		qmlRegisterType<Conversation::Model::Air>("Conversation.Model", 0, 1, "Air");
		qmlRegisterType<Conversation::Model::GroupChannel>("Conversation.Model", 0, 1, "GroupChannel");

		qmlRegisterAnonymousType<Conversation::Model::AbstractMessage>("Conversation.Model", 0);
		qmlRegisterType<Conversation::Model::WithdrawalMessage>("Conversation.Model", 0, 1, "WithdrawalMessage");
		qmlRegisterType<Conversation::Model::TextMessage>("Conversation.Model", 0, 1, "TextMessage");
		qmlRegisterType<Conversation::Model::PollMessage>("Conversation.Model", 0, 1, "PollMessage");
		qmlRegisterType<Conversation::Model::CommandMessage>("Conversation.Model", 0, 1, "CommandMessage");
		qmlRegisterType<Conversation::Model::EchoMessage>("Conversation.Model", 0, 1, "EchoMessage");
		qmlRegisterType<Conversation::Model::HyperlinkMessage>("Conversation.Model", 0, 1, "HyperlinkMessage");
		qmlRegisterType<Conversation::Model::AttachmentMessage>("Conversation.Model", 0, 1, "AttachmentMessage");
		qmlRegisterType<Conversation::Model::PictureMessage>("Conversation.Model", 0, 1, "PictureMessage");
		qmlRegisterType<Conversation::Model::VideoscopeMessage>("Conversation.Model", 0, 1, "VideoscopeMessage");
		qmlRegisterType<Conversation::Model::VoiceMessage>("Conversation.Model", 0, 1, "VoiceMessage");
		qmlRegisterType<Conversation::Model::PDFMessage>("Conversation.Model", 0, 1, "PDFMessage");
		qmlRegisterType<Conversation::Model::YoutubeMessage>("Conversation.Model", 0, 1, "YoutubeMessage");
		qmlRegisterType<Conversation::Model::PushRequestMessage>("Conversation.Model", 0, 1, "PushRequestMessage");
		qmlRegisterType<Conversation::Model::HealthRequestMessage>("Conversation.Model", 0, 1, "HealthRequestMessage");
		qmlRegisterType<Conversation::Model::AuthorizationMessage>("Conversation.Model", 0, 1, "AuthorizationMessage");

		qmlRegisterType<Conversation::Model::AppletMessage>("Conversation.Model", 0, 1, "AppletMessage");

		qmlRegisterAnonymousType<Conversation::Model::AbstractAssist>("Conversation.Model", 0);
		qmlRegisterType<Conversation::Model::CarouselLayout>("Conversation.Model", 0, 1, "CarouselLayout");
		qmlRegisterType<Conversation::Model::VerticalLayout>("Conversation.Model", 0, 1, "VerticalLayout");
		qmlRegisterType<Conversation::Model::TextPush>("Conversation.Model", 0, 1, "TextPush");
		qmlRegisterType<Conversation::Model::HyperLink>("Conversation.Model", 0, 1, "HyperLink");

		return true;
	}

	bool Extension::registerAceAction()
	{
		Repository::Model::AceEvent::Action<Conversation::Model::BroadcastAction>::RegisterMetaType("Cadabra::Ace::Conversation::Model::Broadcast");
		Repository::Model::AceEvent::Action<Conversation::Model::MessageTransfer>::RegisterMetaType("Cadabra::Ace::Conversation::Model::MessageTransfer");
		Repository::Model::AceEvent::Action<Conversation::Model::MessageDelivery>::RegisterMetaType("Cadabra::Ace::Conversation::Model::MessageDelivery");

		return true;
	}

	bool Extension::registerAceScheme()
	{
		/// @note Setups
		Core::Meta::Scheme<Repository::Meta::SetupDependency>::Validate<Conversation::Data::Revision1Setup>(1, {"Conversation", "Extension", {{{"Repository", "Extension"}, 1}}});

		/// @note Application scheme
		Core::Meta::Scheme<Repository::Meta::ApplicationRender>::Validate<Conversation::Model::GroupChannel>(1, {"Conversation", "GroupChannel", 0, 1, "ChannelFeed"});

		/// @note Job specifications
		Core::Meta::Scheme<Repository::Meta::JobSpecification>::Validate<Conversation::Model::Spread>(1, {"Conversation", "Spread"});

		/// @note Message Renderers
		qRegisterMetaType<Conversation::Meta::MessageRender>("Cadabra::Queen::Conversation::Meta::MessageRender");
		Core::Meta::SchemeFactory::Bind<Conversation::Meta::MessageRender>("Conversation.Meta.MessageRender");
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::WithdrawalMessage>(0, {"Conversation", 0, 1, "WithdrawalMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::TextMessage>(1, {"Conversation", 0, 1, "TextMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::AttachmentMessage>(2, {"Conversation", 0, 1, "AttachmentMessage", {/*Conversation::RenderLayout::VerticalLayout*/}, tr("Attachments")});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::PictureMessage>(3, {"Conversation", 0, 1, "PictureMessage"/*, {Conversation::RenderLayout::Grid}*/});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::VideoMessage>(4, {"Conversation", 0, 1, "VideoMessage", {Conversation::RenderLayout::Grid}});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::VideoscopeMessage>(5, {"Conversation", 0, 1, "VideoscopeMessage"/*, {Conversation::RenderLayout::Grid}*/});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::VideostoryMessage>(6, {"Conversation", 0, 1, "VideostoryMessage", {Conversation::RenderLayout::Carousel}});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::AudioMessage>(7, {"Conversation", 0, 1, "AudioMessage", {Conversation::RenderLayout::VerticalLayout}});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::VoiceMessage>(8, {"Conversation", 0, 1, "VoiceMessage"/*, {Conversation::RenderLayout::Grid}*/});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::CommandMessage>(9, {"Conversation", 0, 1, "CommandMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::HyperlinkMessage>(10, {"Conversation", 0, 1, "HyperlinkMessage"/*, {Conversation::RenderLayout::VerticalLayout}*/});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::MarkdownMessage>(11, {"Conversation", 0, 1, "MarkdownMessage"});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::HTMLMessage>(12, {"Conversation", 0, 1, "HTMLMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::PollMessage>(13, {"Conversation", 0, 1, "PollMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::EchoMessage>(19, {"Conversation", 0, 1, "EchoMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::YoutubeMessage>(20, {"Conversation", 0, 1, "YoutubeMessage"/*, {Conversation::RenderLayout::Grid}*/});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::VimeoMessage>(21, {"Conversation", 0, 1, "VimeoVideoMessage", {Conversation::RenderLayout::Grid}});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::PlaylistMessage>(29, {"Conversation", 0, 1, "PlaylistMessage", {Conversation::RenderLayout::Grid}});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::Reserved>(50, {"Conversation", 0, 1, "Reserved"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::PushRequestMessage>(51, {"Conversation", 0, 1, "PushRequestMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::HealthRequestMessage>(52, {"Conversation", 0, 1, "HealthRequestMessage"});

		/// @todo
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::WebApplicationMessage>(90, {"Conversation", 0, 1, "WebApplicationMessage"});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::AppletMessage>(99, {"Conversation", 0, 1, "AppletMessage", {/*Conversation::RenderLayout::VerticalLayout*/}, tr("Applications")});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::FileMessage>(100, {"Conversation", 0, 1, "FileMessage", {Conversation::RenderLayout::VerticalLayout}});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::PDFMessage>(101, {"Conversation", 0, 1, "PDFMessage", {Conversation::RenderLayout::VerticalLayout}});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::DOCMessage>(102, {"Conversation", 0, 1, "DOCMessage", {Conversation::RenderLayout::VerticalLayout}});
//		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::XLSMessage>(103, {"Conversation", 0, 1, "XLSMessage", {Conversation::RenderLayout::VerticalLayout}});
		Core::Meta::Scheme<Conversation::Meta::MessageRender>::Validate<Conversation::Model::AuthorizationMessage>(666, {"Conversation", 0, 1, "AuthorizationMessage"});

		/// @note Assist renderers
		qRegisterMetaType<Conversation::Meta::AssistRender>("Cadabra::Queen::Conversation::Meta::AssistRender");
		Core::Meta::SchemeFactory::Bind<Conversation::Meta::AssistRender>("Conversation.Meta.AssistRender");
//		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::MessageComposer>(0, {"Conversation", 0, 1, "MessageComposer"});
		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::CarouselLayout>(1, {"Conversation", 0, 1, "CarouselLayout"});
		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::TextPush>(2, {"Conversation", 0, 1, "TextPush"});
//		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::KeyboardBar>(3, {"Conversation", 0, 1, "KeyboardBar"});
//		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::MasonryLayout>(4, {"Conversation", 0, 1, "MasonryLayout"});
		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::GridLayout>(5, {"Conversation", 0, 1, "GridLayout"});
		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::VerticalLayout>(6, {"Conversation", 0, 1, "VerticalLayout"});
//		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::HorizontalLayout>(7, {"Conversation", 0, 1, "HorizontalLayout"});

		Core::Meta::Scheme<Conversation::Meta::AssistRender>::Validate<Conversation::Model::HyperLink>(20, {"Conversation", 0, 1, "HyperLink"});

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
