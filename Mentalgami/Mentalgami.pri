MENTALGAMI_RESOURCES = $$PWD/Mentalgami.qrc

MENTALGAMI_HEADERS = \
	$$PWD/Mentalgami.hpp \
        $$PWD/Controller/Application/mg_QuizFlow.hpp \
        $$PWD/Data/Entity/mg_CourseBrief.hpp \
        $$PWD/Data/Entity/mg_CourseTrack.hpp \
        $$PWD/Data/mg_RiddleList.hpp \
        $$PWD/Meta/mg_Course.hpp \
        $$PWD/Meta/mg_RiddleInformation.hpp \
        $$PWD/Meta/mg_RiddleRender.hpp \
        $$PWD/Meta/mg_Track.hpp \
        $$PWD/Meta/mg_TrackRender.hpp \
        $$PWD/Model/mg_AbstractRangeValidator.hpp \
        $$PWD/Model/mg_AbstractRiddle.hpp \
        $$PWD/Model/mg_AbstractTrack.hpp \
        $$PWD/Model/mg_AnswerModel.hpp \
        $$PWD/Model/Application/mg_EurekaDirectory.hpp \
        $$PWD/Model/Application/mg_GenericQuiz.hpp \
        $$PWD/Model/Application/Interface/mg_QuizInterface.hpp \
        $$PWD/Model/Application/Quiz/mg_BMIQuiz.hpp \
        $$PWD/Model/Application/Quiz/mg_CalorieBalanceQuiz.hpp \
        $$PWD/Model/Application/Quiz/mg_IdealWeightQuiz.hpp \
        $$PWD/Model/Application/Quiz/mg_WelcomeQuiz.hpp \
        $$PWD/Model/Application/Quiz/mg_WHRQuiz.hpp \
        $$PWD/Model/Application/mg_SmartChannel.hpp \
        $$PWD/Model/Application/mg_SmartCourse.hpp \
        $$PWD/Model/Application/mg_SmartCourse.hpp \
        $$PWD/Model/Message/mg_AwardMessage.hpp \
        $$PWD/Model/Message/mg_SmartCourseMessage.hpp \
        $$PWD/Model/Riddle/mg_AuthorizationRiddle.hpp \
        $$PWD/Model/Riddle/mg_BoolRiddle.hpp \
        $$PWD/Model/Riddle/mg_CustomRiddle.hpp \
        $$PWD/Model/Riddle/mg_DateRiddle.hpp \
        $$PWD/Model/Riddle/mg_IntegerRiddle.hpp \
        $$PWD/Model/Riddle/mg_ListRiddle.hpp \
        $$PWD/Model/Riddle/mg_NullRiddle.hpp \
        $$PWD/Model/Riddle/mg_NumericRiddle.hpp \
        $$PWD/Model/Riddle/mg_TextRiddle.hpp \
        $$PWD/Model/mg_RiddleRangeValidator.hpp \
        $$PWD/Model/mg_RiddleValidator.hpp \
        $$PWD/Model/mg_TemplatedRiddle.hpp \
        $$PWD/Model/Track/mg_CalendarTrack.hpp \
        $$PWD/Model/Track/mg_HTMLTrack.hpp \
        $$PWD/Model/Track/mg_MSExcelTrack.hpp \
        $$PWD/Model/Track/mg_MSWordTrack.hpp \
        $$PWD/Model/Track/mg_PDFTrack.hpp \
        $$PWD/Model/Track/mg_RTFTrack.hpp \
        $$PWD/Model/Track/mg_TelegramTrack.hpp \
        $$PWD/Model/Track/mg_VideoTrack.hpp \
        $$PWD/Model/Track/mg_WebTrack.hpp \
        $$PWD/Net/Call/mg_WelcomeCall.hpp \
        $$PWD/Net/File/mg_MojiFile.hpp \
        $$PWD/View/mg_Moji.hpp \
        $$PWD/View/mg_RiddleView.hpp \
        $$PWD/View/mg_TrackLayout.hpp \
        $$PWD/View/mg_TrackStackview.hpp \
        $$PWD/View/mg_TrackView.hpp

MENTALGAMI_SOURCES = \
	$$PWD/Mentalgami.cpp \
        $$PWD/Controller/Application/mg_QuizFlow.cpp \
        $$PWD/Data/Entity/mg_CourseBrief.cpp \
        $$PWD/Data/Entity/mg_CourseTrack.cpp \
        $$PWD/Data/mg_RiddleList.cpp \
        $$PWD/Meta/mg_Course.cpp \
        $$PWD/Meta/mg_RiddleInformation.cpp \
        $$PWD/Meta/mg_RiddleRender.cpp \
        $$PWD/Meta/mg_Track.cpp \
        $$PWD/Meta/mg_TrackRender.cpp \
        $$PWD/Model/mg_AbstractRangeValidator.cpp \
        $$PWD/Model/mg_AbstractRiddle.cpp \
        $$PWD/Model/mg_AbstractTrack.cpp \
        $$PWD/Model/mg_AnswerModel.cpp \
        $$PWD/Model/Application/mg_EurekaDirectory.cpp \
        $$PWD/Model/Application/mg_GenericQuiz.cpp \
        $$PWD/Model/Application/Interface/mg_QuizInterface.cpp \
        $$PWD/Model/Application/Quiz/mg_BMIQuiz.cpp \
        $$PWD/Model/Application/Quiz/mg_CalorieBalanceQuiz.cpp \
        $$PWD/Model/Application/Quiz/mg_IdealWeightQuiz.cpp \
        $$PWD/Model/Application/Quiz/mg_WelcomeQuiz.cpp \
        $$PWD/Model/Application/Quiz/mg_WHRQuiz.cpp \
        $$PWD/Model/Application/mg_SmartChannel.cpp \
        $$PWD/Model/Application/mg_SmartCourse.cpp \
        $$PWD/Model/Message/mg_AwardMessage.cpp \
        $$PWD/Model/Message/mg_SmartCourseMessage.cpp \
        $$PWD/Model/Riddle/mg_AuthorizationRiddle.cpp \
        $$PWD/Model/Riddle/mg_BoolRiddle.cpp \
        $$PWD/Model/Riddle/mg_CustomRiddle.cpp \
        $$PWD/Model/Riddle/mg_DateRiddle.cpp \
        $$PWD/Model/Riddle/mg_IntegerRiddle.cpp \
        $$PWD/Model/Riddle/mg_ListRiddle.cpp \
        $$PWD/Model/Riddle/mg_NullRiddle.cpp \
        $$PWD/Model/Riddle/mg_NumericRiddle.cpp \
        $$PWD/Model/Riddle/mg_TextRiddle.cpp \
        $$PWD/Model/mg_RiddleRangeValidator.cpp \
        $$PWD/Model/mg_RiddleValidator.cpp \
        $$PWD/Model/mg_TemplatedRiddle.cpp \
        $$PWD/Model/Track/mg_CalendarTrack.cpp \
        $$PWD/Model/Track/mg_HTMLTrack.cpp \
        $$PWD/Model/Track/mg_MSExcelTrack.cpp \
        $$PWD/Model/Track/mg_MSWordTrack.cpp \
        $$PWD/Model/Track/mg_PDFTrack.cpp \
        $$PWD/Model/Track/mg_RTFTrack.cpp \
        $$PWD/Model/Track/mg_TelegramTrack.cpp \
        $$PWD/Model/Track/mg_VideoTrack.cpp \
        $$PWD/Model/Track/mg_WebTrack.cpp \
        $$PWD/Net/Call/mg_WelcomeCall.cpp \
        $$PWD/Net/File/mg_MojiFile.cpp \
        $$PWD/View/mg_Moji.cpp \
        $$PWD/View/mg_RiddleView.cpp \
        $$PWD/View/mg_TrackLayout.cpp \
        $$PWD/View/mg_TrackStackview.cpp \
        $$PWD/View/mg_TrackView.cpp

contains(QT, testlib) {
        MENTALGAMI_RESOURCES += $$PWD/Test/MentalgamiTest.qrc

	MENTALGAMI_HEADERS += $$files($$PWD/Test/mg_*.hpp, true)
        MENTALGAMI_SOURCES += $$files($$PWD/Test/mg_*.cpp, true)

        MENTALGAMI_HEADERS += $$files($$PWD/Shocker/mg_*.hpp, true)
        MENTALGAMI_SOURCES += $$files($$PWD/Shocker/mg_*.cpp, true)
}
