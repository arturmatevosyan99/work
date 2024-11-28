MENTALGAMI_RESOURCES = $$PWD/Mentalgami.qrc

MENTALGAMI_HEADERS = \
	$$PWD/Mentalgami.hpp \
	$$PWD/Controller/Application/QuizFlow.hpp \
	$$PWD/Data/Entity/CourseBrief.hpp \
	$$PWD/Data/Entity/CourseTrack.hpp \
	$$PWD/Data/RiddleList.hpp \
	$$PWD/Meta/Course.hpp \
	$$PWD/Meta/RiddleInformation.hpp \
	$$PWD/Meta/RiddleRender.hpp \
	$$PWD/Meta/Track.hpp \
	$$PWD/Meta/TrackRender.hpp \
	$$PWD/Model/AbstractRangeValidator.hpp \
	$$PWD/Model/AbstractRiddle.hpp \
	$$PWD/Model/AbstractTrack.hpp \
	$$PWD/Model/AnswerModel.hpp \
	$$PWD/Model/Application/EurekaDirectory.hpp \
	$$PWD/Model/Application/GenericQuiz.hpp \
	$$PWD/Model/Application/Interface/QuizInterface.hpp \
	$$PWD/Model/Application/Quiz/BMIQuiz.hpp \
	$$PWD/Model/Application/Quiz/CalorieBalanceQuiz.hpp \
	$$PWD/Model/Application/Quiz/IdealWeightQuiz.hpp \
	$$PWD/Model/Application/Quiz/WelcomeQuiz.hpp \
	$$PWD/Model/Application/Quiz/WHRQuiz.hpp \
	$$PWD/Model/Application/SmartChannel.hpp \
	$$PWD/Model/Application/SmartCourse.hpp \
	$$PWD/Model/Application/SmartCourse.hpp \
	$$PWD/Model/Message/AwardMessage.hpp \
	$$PWD/Model/Message/SmartCourseMessage.hpp \
	$$PWD/Model/Riddle/AuthorizationRiddle.hpp \
	$$PWD/Model/Riddle/BoolRiddle.hpp \
	$$PWD/Model/Riddle/CustomRiddle.hpp \
	$$PWD/Model/Riddle/DateRiddle.hpp \
	$$PWD/Model/Riddle/IntegerRiddle.hpp \
	$$PWD/Model/Riddle/ListRiddle.hpp \
	$$PWD/Model/Riddle/NullRiddle.hpp \
	$$PWD/Model/Riddle/NumericRiddle.hpp \
	$$PWD/Model/Riddle/TextRiddle.hpp \
	$$PWD/Model/RiddleRangeValidator.hpp \
	$$PWD/Model/RiddleValidator.hpp \
        $$PWD/Model/SmartBookModel.hpp \
	$$PWD/Model/TemplatedRiddle.hpp \
	$$PWD/Model/Track/CalendarTrack.hpp \
	$$PWD/Model/Track/HTMLTrack.hpp \
	$$PWD/Model/Track/MSExcelTrack.hpp \
	$$PWD/Model/Track/MSWordTrack.hpp \
	$$PWD/Model/Track/PDFTrack.hpp \
	$$PWD/Model/Track/RTFTrack.hpp \
	$$PWD/Model/Track/TelegramTrack.hpp \
	$$PWD/Model/Track/VideoTrack.hpp \
	$$PWD/Model/Track/WebTrack.hpp \
	$$PWD/Net/Call/WelcomeCall.hpp \
	$$PWD/Net/File/MojiFile.hpp \
	$$PWD/View/Moji.hpp \
	$$PWD/View/RiddleView.hpp \
	$$PWD/View/TrackLayout.hpp \
	$$PWD/View/TrackStackview.hpp \
	$$PWD/View/TrackView.hpp

MENTALGAMI_SOURCES = \
	$$PWD/Mentalgami.cpp \
	$$PWD/Controller/Application/QuizFlow.cpp \
	$$PWD/Data/Entity/CourseBrief.cpp \
	$$PWD/Data/Entity/CourseTrack.cpp \
	$$PWD/Data/RiddleList.cpp \
	$$PWD/Meta/Course.cpp \
	$$PWD/Meta/RiddleInformation.cpp \
	$$PWD/Meta/RiddleRender.cpp \
	$$PWD/Meta/Track.cpp \
	$$PWD/Meta/TrackRender.cpp \
	$$PWD/Model/AbstractRangeValidator.cpp \
	$$PWD/Model/AbstractRiddle.cpp \
	$$PWD/Model/AbstractTrack.cpp \
	$$PWD/Model/AnswerModel.cpp \
	$$PWD/Model/Application/EurekaDirectory.cpp \
	$$PWD/Model/Application/GenericQuiz.cpp \
	$$PWD/Model/Application/Interface/QuizInterface.cpp \
	$$PWD/Model/Application/Quiz/BMIQuiz.cpp \
	$$PWD/Model/Application/Quiz/CalorieBalanceQuiz.cpp \
	$$PWD/Model/Application/Quiz/IdealWeightQuiz.cpp \
	$$PWD/Model/Application/Quiz/WelcomeQuiz.cpp \
	$$PWD/Model/Application/Quiz/WHRQuiz.cpp \
	$$PWD/Model/Application/SmartChannel.cpp \
	$$PWD/Model/Application/SmartCourse.cpp \
	$$PWD/Model/Message/AwardMessage.cpp \
	$$PWD/Model/Message/SmartCourseMessage.cpp \
	$$PWD/Model/Riddle/AuthorizationRiddle.cpp \
	$$PWD/Model/Riddle/BoolRiddle.cpp \
	$$PWD/Model/Riddle/CustomRiddle.cpp \
	$$PWD/Model/Riddle/DateRiddle.cpp \
	$$PWD/Model/Riddle/IntegerRiddle.cpp \
	$$PWD/Model/Riddle/ListRiddle.cpp \
	$$PWD/Model/Riddle/NullRiddle.cpp \
	$$PWD/Model/Riddle/NumericRiddle.cpp \
	$$PWD/Model/Riddle/TextRiddle.cpp \
	$$PWD/Model/RiddleRangeValidator.cpp \
	$$PWD/Model/RiddleValidator.cpp \
        $$PWD/Model/SmartBookModel.cpp \
	$$PWD/Model/TemplatedRiddle.cpp \
	$$PWD/Model/Track/CalendarTrack.cpp \
	$$PWD/Model/Track/HTMLTrack.cpp \
	$$PWD/Model/Track/MSExcelTrack.cpp \
	$$PWD/Model/Track/MSWordTrack.cpp \
	$$PWD/Model/Track/PDFTrack.cpp \
	$$PWD/Model/Track/RTFTrack.cpp \
	$$PWD/Model/Track/TelegramTrack.cpp \
	$$PWD/Model/Track/VideoTrack.cpp \
	$$PWD/Model/Track/WebTrack.cpp \
	$$PWD/Net/Call/WelcomeCall.cpp \
	$$PWD/Net/File/MojiFile.cpp \
	$$PWD/View/Moji.cpp \
	$$PWD/View/RiddleView.cpp \
	$$PWD/View/TrackLayout.cpp \
	$$PWD/View/TrackStackview.cpp \
	$$PWD/View/TrackView.cpp

contains(QT, testlib) {
	MENTALGAMI_RESOURCES += $$PWD/Test/MentalgamiTest.qrc

	MENTALGAMI_HEADERS += $$files($$PWD/Test/*.hpp, true)
	MENTALGAMI_SOURCES += $$files($$PWD/Test/*.cpp, true)

	MENTALGAMI_HEADERS += $$files($$PWD/Shocker/*.hpp, true)
	MENTALGAMI_SOURCES += $$files($$PWD/Shocker/*.cpp, true)
}
