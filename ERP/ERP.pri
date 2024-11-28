ERP_RESOURCES = $$PWD/ERP.qrc

ERP_HEADERS = \
	$$PWD/ERP.hpp \
	$$PWD/Model/AbstractDocument.hpp \
	$$PWD/Model/Message/DocumentMessage.hpp

ERP_SOURCES = \
	$$PWD/ERP.cpp \
	$$PWD/Model/AbstractDocument.cpp \
	$$PWD/Model/Message/DocumentMessage.cpp

contains(QT, testlib) {
#	ERP_RESOURCES += $$PWD/Test/ERPTest.qrc

#	ERP_HEADERS += $$files($$PWD/Test/*.hpp, true)
#	ERP_SOURCES += $$files($$PWD/Test/*.cpp, true)

#	ERP_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
#	ERP_SOURCES += $$files($$PWD/Shocker/*.cpp, true)
}
