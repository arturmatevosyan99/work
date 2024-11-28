DROBLOZHKO_RESOURCES = $$PWD/DrOblozhko.qrc

DROBLOZHKO_HEADERS = \
	$$PWD/DrOblozhko.hpp \
	$$PWD/Meta/Translation.hpp \
	$$PWD/Model/Application/AppointmentChannel.hpp \
	$$PWD/Model/Application/Reception.hpp \
	$$PWD/Model/Application/Webinar.hpp \
	$$PWD/Model/Application/Quiz/MetabolicPlateLengthQuiz.hpp \
	$$PWD/Model/Appointment.hpp \
	$$PWD/Model/Job/Cleanup.hpp \
	$$PWD/Model/Message/HandBookMessage.hpp \
	$$PWD/Model/Message/InteractiveApplicationMessage.hpp \
	$$PWD/Model/Message/VideoLessonMessage.hpp \
	$$PWD/Model/Message/WebinarMessage.hpp \
	$$PWD/Net/Call/AccessCall.hpp \
	$$PWD/Net/Call/AppointmentCall.hpp

DROBLOZHKO_SOURCES = \
	$$PWD/DrOblozhko.cpp \
	$$PWD/Meta/Translation.cpp \
	$$PWD/Model/Application/AppointmentChannel.cpp \
	$$PWD/Model/Application/Reception.cpp \
	$$PWD/Model/Application/Webinar.cpp \
	$$PWD/Model/Application/Quiz/MetabolicPlateLengthQuiz.cpp \
	$$PWD/Model/Appointment.cpp \
	$$PWD/Model/Job/Cleanup.cpp \
	$$PWD/Model/Message/HandBookMessage.cpp \
	$$PWD/Model/Message/InteractiveApplicationMessage.cpp \
	$$PWD/Model/Message/VideoLessonMessage.cpp \
	$$PWD/Model/Message/WebinarMessage.cpp \
	$$PWD/Net/Call/AccessCall.cpp \
	$$PWD/Net/Call/AppointmentCall.cpp

contains(QT, testlib) {
#	DROBLOZHKO_RESOURCES += $$PWD/Test/DrOblozhkoTest.qrc

	DROBLOZHKO_HEADERS += $$files($$PWD/Test/*.hpp, true)
	DROBLOZHKO_SOURCES += $$files($$PWD/Test/*.cpp, true)

	DROBLOZHKO_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
	DROBLOZHKO_SOURCES += $$files($$PWD/Shocker/*.cpp, true)
}
