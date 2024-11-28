EC_RESOURCES = $$PWD/EC.qrc

EC_HEADERS = \
	$$PWD/EC.hpp \
	$$PWD/Data/Basket.hpp \
	$$PWD/Data/Showcase.hpp \
	$$PWD/Data/Stock.hpp \
	$$PWD/Meta/CustomerAccount.hpp \
	$$PWD/Meta/InappReceipt.hpp \
	$$PWD/Model/AbstractAcquirer.hpp \
	$$PWD/Model/Acquirer/SberAcquirer.hpp \
	$$PWD/Model/Acquirer/TinkoffAcquirer.hpp \
	$$PWD/Model/Application/OnlineShop.hpp \
	$$PWD/Model/Message/OfferMessage.hpp \
	$$PWD/Model/Message/PaymentMessage.hpp

 EC_SOURCES = \
	$$PWD/EC.cpp \
	$$PWD/Data/Basket.cpp \
	$$PWD/Data/Showcase.cpp \
	$$PWD/Data/Stock.cpp \
	$$PWD/Meta/CustomerAccount.cpp \
	$$PWD/Meta/InappReceipt.cpp \
	$$PWD/Model/AbstractAcquirer.cpp \
	$$PWD/Model/Acquirer/SberAcquirer.cpp \
	$$PWD/Model/Acquirer/TinkoffAcquirer.cpp \
	$$PWD/Model/Application/OnlineShop.cpp \
	$$PWD/Model/Message/OfferMessage.cpp \
	$$PWD/Model/Message/PaymentMessage.cpp

contains(QT, testlib) {
#	EC_RESOURCES += $$PWD/Test/ECTest.qrc

#	EC_HEADERS += $$files($$PWD/Test/*.hpp, true)
#	EC_SOURCES += $$files($$PWD/Test/*.cpp, true)

#	EC_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
#	EC_SOURCES += $$files($$PWD/Shocker/*.cpp, true)
}

contains(DEFINES, TKS_ASDK) {
	TINKOFFASDK_BUILD=TRUE
	ios {
		OBJECTIVE_SOURCES += $$PWD/EC/Model/Acquirer/iOSTinkoffAcquirer.mm
		TKS_ASDK_SRC_PATH = $$OUT_PWD/tinkoff-asdk
		TKS_ASDK_BUILD_PATH = $$OUT_PWD/tinkoff-asdk/build/$$QUEEN_BUILD_TYPE-$$QUEEN_IOS_SDK

		!exists($${TKS_ASDK_BUILD_PATH}) {
			message(Building TinkoffASDK for iOS $$QUEEN_ARCH in $$TKS_ASDK_BUILD_PATH)
			system(3party/tinkoffasdk/ios-build.sh $$QUEEN_ARCH $$QUEEN_BUILD_TYPE $$QUEEN_IOS_SDK $$TKS_ASDK_SRC_PATH):TINKOFFASDK_BUILD=TRUE
			!equals(TINKOFFASDK_BUILD,TRUE) {
				error("TinkoffASDK for iOS is missed")
			}
		}

		INCLUDEPATH += "$${TKS_ASDK_BUILD_PATH}/ASDKCore.framework/Headers"
		INCLUDEPATH += "$${TKS_ASDK_BUILD_PATH}/ASDKUI.framework/Headers"

		LIBS += -F$$TKS_ASDK_BUILD_PATH/
		LIBS += -framework ASDKCore
		LIBS += -framework ASDKUI
		LIBS += -framework PassKit
		LIBS += -framework Contacts

		TKS_ASDK_CORE.path = Frameworks
		TKS_ASDK_CORE.files += $$files($$TKS_ASDK_BUILD_PATH/ASDKCore.framework)
		QMAKE_BUNDLE_DATA += TKS_ASDK_CORE

		TKS_ASDK_UI.path = Frameworks
		TKS_ASDK_UI.files += $$files($$TKS_ASDK_BUILD_PATH/ASDKUI.framework)
		QMAKE_BUNDLE_DATA += TKS_ASDK_UI
	} android {
		# building by graddle
		SOURCES += $$PWD/Model/Acquirer/AndroidTinkoffAcquirer.cpp
	}
}

contains(DEFINES, INAPP_SDK) {
	ios {
		DEFINES -= INAPP_SDK
		DEFINES += INAPP_SDK=APS
		EC_HEADERS += $$PWD/Model/Acquirer/AppStore.hpp
		OBJECTIVE_HEADERS += $$PWD/Model/Acquirer/AppStore.h
		OBJECTIVE_SOURCES += $$PWD/Model/Acquirer/AppStore.mm
	} android {
#		DEFINES += INAPP_SDK=GPM
#		DEFINES += INAPP_SDK=HAP
		EC_HEADERS += $$PWD/Model/Acquirer/PlayMarket.hpp
		EC_SOURCES += $$PWD/Model/Acquirer/PlayMarket.cpp
	}
} else {
	EC_HEADERS += $$PWD/Model/Acquirer/InappCharger.hpp
	EC_SOURCES += $$PWD/Model/Acquirer/InappCharger.cpp
}
