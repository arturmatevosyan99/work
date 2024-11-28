SD_RESOURCES = $$PWD/SD.qrc

SD_HEADERS = \
	$$PWD/SD.hpp \
	$$PWD/Meta/ContactCounterparty.hpp \
	$$PWD/Meta/Counterparty.hpp \
	$$PWD/Meta/LegalCounterparty.hpp \
	$$PWD/Meta/Money.hpp \
	$$PWD/Meta/PaymentPreference.hpp \
	$$PWD/Meta/PurchaseItem.hpp \
	$$PWD/Model/Document/PurchaseOrder.hpp

SD_SOURCES = \
	$$PWD/SD.cpp \
	$$PWD/Meta/ContactCounterparty.cpp \
	$$PWD/Meta/Counterparty.cpp \
	$$PWD/Meta/LegalCounterparty.cpp \
	$$PWD/Meta/Money.cpp \
	$$PWD/Meta/PaymentPreference.cpp \
	$$PWD/Meta/PurchaseItem.cpp \
	$$PWD/Model/Document/PurchaseOrder.cpp

contains(QT, testlib) {
#	SD_RESOURCES += $$PWD/Test/SDTest.qrc

#	SD_HEADERS += $$files($$PWD/Test/*.hpp, true)
#	SD_RESOURCES += $$files($$PWD/Test/*.cpp, true)

#	SD_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
#	SD_RESOURCES += $$files($$PWD/Shocker/*.cpp, true)
}
