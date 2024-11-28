STRESS_RESOURCES = $$PWD/Stress.qrc

STRESS_HEADERS += \
	$$PWD/Stress.hpp \
	$$PWD/System/LogClient.hpp \
	$$PWD/System/TestThread.hpp \
	$$PWD/System/TestContext.hpp \
	$$PWD/Meta/TestSchemeCache.hpp \
	$$PWD/Meta/TestSchemeCache/FileSystemTestDirectory.hpp \
	$$PWD/Meta/TestSchemeCache/WebDAVTestDirectory.hpp \
	$$PWD/Model/CheckList.hpp \
	$$PWD/Model/LogList.hpp \
	$$PWD/Net/TCPLogClient.hpp \
	$$PWD/Net/Call/SignalCall.hpp \
	$$PWD/Test/Shocker/AceShocker.hpp \
	$$PWD/Test/Shocker/NetShocker.hpp \
	$$PWD/Test/Shocker/MemoryShocker.hpp \
	$$PWD/Test/Unit/CAP-653.hpp

STRESS_SOURCES += \
	$$PWD/Stress.cpp \
	$$PWD/System/LogClient.cpp \
	$$PWD/System/TestThread.cpp \
	$$PWD/System/TestContext.cpp \
	$$PWD/Meta/TestSchemeCache.cpp \
	$$PWD/Meta/TestSchemeCache/FileSystemTestDirectory.cpp \
	$$PWD/Meta/TestSchemeCache/WebDAVTestDirectory.cpp \
	$$PWD/Model/CheckList.cpp \
	$$PWD/Model/LogList.cpp \
	$$PWD/Net/TCPLogClient.cpp \
	$$PWD/Net/Call/SignalCall.cpp \
	$$PWD/Test/Shocker/AceShocker.cpp \
	$$PWD/Test/Shocker/NetShocker.cpp \
	$$PWD/Test/Shocker/MemoryShocker.cpp \
	$$PWD/Test/Unit/CAP-653.cpp
