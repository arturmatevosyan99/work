CONVERSATION_RESOURCES = $$PWD/Conversation.qrc

CONVERSATION_HEADERS = \
	$$PWD/Conversation.hpp \
	$$PWD/Data/Assistant.hpp \
	$$PWD/Data/Entity/ChannelDraft.hpp \
	$$PWD/Data/Entity/ChannelMember.hpp \
	$$PWD/Data/Entity/ChannelMessage.hpp \
	$$PWD/Data/Entity/ChannelSwap.hpp \
	$$PWD/Data/Entity/ChannelSync.hpp \
	$$PWD/Data/Entity/ChannelTag.hpp \
	$$PWD/Data/Setup/Revision1Setup.hpp \
	$$PWD/Meta/Assist.hpp \
	$$PWD/Meta/AssistRender.hpp \
	$$PWD/Meta/Channel.hpp \
	$$PWD/Meta/Message.hpp \
	$$PWD/Meta/MessageRender.hpp \
	$$PWD/Model/AbstractAccident.hpp \
	$$PWD/Model/AbstractAssist.hpp \
	$$PWD/Model/AbstractMessage.hpp \
	$$PWD/Model/Accident/MessageDelivery.hpp \
	$$PWD/Model/Accident/MessageTransfer.hpp \
	$$PWD/Model/Action/BroadcastAction.hpp \
	$$PWD/Model/Action/SyncAction.hpp \
	$$PWD/Model/Application/Air.hpp \
	$$PWD/Model/Application/GroupChannel.hpp \
	$$PWD/Model/Application/Interface/ChatInterface.hpp \
	$$PWD/Model/Assist/AbstractLayout.hpp \
	$$PWD/Model/Assist/HyperLink.hpp \
	$$PWD/Model/Assist/Layout/CarouselLayout.hpp \
	$$PWD/Model/Assist/Layout/GridLayout.hpp \
	$$PWD/Model/Assist/Layout/VerticalLayout.hpp \
	$$PWD/Model/Assist/TextPush.hpp \
	$$PWD/Model/Event.hpp \
	$$PWD/Model/Job/Spread.hpp \
	$$PWD/Model/Message/AppletMessage.hpp \
	$$PWD/Model/Message/AttachmentMessage.hpp \
	$$PWD/Model/Message/AuthorizationMessage.hpp \
	$$PWD/Model/Message/CommandMessage.hpp \
	$$PWD/Model/Message/EchoMessage.hpp \
	$$PWD/Model/Message/HealthRequestMessage.hpp \
	$$PWD/Model/Message/HyperlinkMessage.hpp \
	$$PWD/Model/Message/PDFMessage.hpp \
	$$PWD/Model/Message/PictureMessage.hpp \
	$$PWD/Model/Message/PollMessage.hpp \
	$$PWD/Model/Message/ProxyMessage.hpp \
	$$PWD/Model/Message/PushRequestMessage.hpp \
	$$PWD/Model/Message/TextMessage.hpp \
	$$PWD/Model/Message/VideoscopeMessage.hpp \
	$$PWD/Model/Message/VoiceMessage.hpp \
	$$PWD/Model/Message/WithdrawalMessage.hpp \
	$$PWD/Model/Message/YoutubeMessage.hpp \
	$$PWD/View/AssistLayout.hpp \
	$$PWD/View/AssistView.hpp \
	$$PWD/View/MessageLayout.hpp \
	$$PWD/View/MessageView.hpp

CONVERSATION_SOURCES = \
	$$PWD/Conversation.cpp \
	$$PWD/Data/Assistant.cpp \
	$$PWD/Data/Entity/ChannelDraft.cpp \
	$$PWD/Data/Entity/ChannelMember.cpp \
	$$PWD/Data/Entity/ChannelMessage.cpp \
	$$PWD/Data/Entity/ChannelSwap.cpp \
	$$PWD/Data/Entity/ChannelSync.cpp \
	$$PWD/Data/Entity/ChannelTag.cpp \
	$$PWD/Data/Setup/Revision1Setup.cpp \
	$$PWD/Meta/Assist.cpp \
	$$PWD/Meta/AssistRender.cpp \
	$$PWD/Meta/Channel.cpp \
	$$PWD/Meta/Message.cpp \
	$$PWD/Meta/MessageRender.cpp \
	$$PWD/Model/AbstractAccident.cpp \
	$$PWD/Model/AbstractAssist.cpp \
	$$PWD/Model/AbstractMessage.cpp \
	$$PWD/Model/Accident/MessageDelivery.cpp \
	$$PWD/Model/Accident/MessageTransfer.cpp \
	$$PWD/Model/Action/BroadcastAction.cpp \
	$$PWD/Model/Action/SyncAction.cpp \
	$$PWD/Model/Application/Air.cpp \
	$$PWD/Model/Application/GroupChannel.cpp \
	$$PWD/Model/Application/Interface/ChatInterface.cpp \
	$$PWD/Model/Assist/AbstractLayout.cpp \
	$$PWD/Model/Assist/HyperLink.cpp \
	$$PWD/Model/Assist/Layout/CarouselLayout.cpp \
	$$PWD/Model/Assist/Layout/GridLayout.cpp \
	$$PWD/Model/Assist/Layout/VerticalLayout.cpp \
	$$PWD/Model/Assist/TextPush.cpp \
	$$PWD/Model/Event.cpp \
	$$PWD/Model/Job/Spread.cpp \
	$$PWD/Model/Message/AppletMessage.cpp \
	$$PWD/Model/Message/AttachmentMessage.cpp \
	$$PWD/Model/Message/AuthorizationMessage.cpp \
	$$PWD/Model/Message/CommandMessage.cpp \
	$$PWD/Model/Message/EchoMessage.cpp \
	$$PWD/Model/Message/HealthRequestMessage.cpp \
	$$PWD/Model/Message/HyperlinkMessage.cpp \
	$$PWD/Model/Message/PDFMessage.cpp \
	$$PWD/Model/Message/PictureMessage.cpp \
	$$PWD/Model/Message/PollMessage.cpp \
	$$PWD/Model/Message/ProxyMessage.cpp \
	$$PWD/Model/Message/PushRequestMessage.cpp \
	$$PWD/Model/Message/TextMessage.cpp \
	$$PWD/Model/Message/VideoscopeMessage.cpp \
	$$PWD/Model/Message/VoiceMessage.cpp \
	$$PWD/Model/Message/WithdrawalMessage.cpp \
	$$PWD/Model/Message/YoutubeMessage.cpp \
	$$PWD/View/AssistLayout.cpp \
	$$PWD/View/AssistView.cpp \
	$$PWD/View/MessageLayout.cpp \
	$$PWD/View/MessageView.cpp

contains(QT, testlib) {
	CONVERSATION_RESOURCES += $$PWD/Test/ConversationTest.qrc

	CONVERSATION_HEADERS += $$files($$PWD/Test/*.hpp, true)
	CONVERSATION_SOURCES += $$files($$PWD/Test/*.cpp, true)

	CONVERSATION_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
	CONVERSATION_SOURCES += $$files($$PWD/Shocker/*.cpp, true)
}
