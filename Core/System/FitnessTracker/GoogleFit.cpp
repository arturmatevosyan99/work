#include "../FitnessTracker.hpp"

#include <QString>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	static void AccessGoogleFit(JNIEnv* environment, jobject self, jboolean success)
	{
		if (JNI_TRUE == success) {
			emit FitnessTracker::Instance()->registered();
		} else {
			//emit FitnessTracker::Instance()->unregistered();
		}
	}

	class GoogleFit : public FitnessTracker
	{
	public:
		GoogleFit()
		:
			FitnessTracker()
		{
			QtAndroid::runOnAndroidThreadSync(
												[=]() {
													JNINativeMethod methods[]{
																				{"AccessGoogleFit", "(Z)V", reinterpret_cast<void*>(Cadabra::Queen::Core::System::AccessGoogleFit)}
													};

													QAndroidJniObject java_class("systems.cadabra.queen.QueenActivity");
													QAndroidJniEnvironment jni_environment;
													jclass object_class = jni_environment->GetObjectClass(java_class.object<jobject>());
													jni_environment->RegisterNatives(object_class, methods, sizeof(methods) / sizeof(methods[0]));
													jni_environment->DeleteLocalRef(object_class);
												}
			);
		}

		virtual ~GoogleFit() override = default;

	public:
		virtual bool requestAccess() override
		{
			QtAndroid::runOnAndroidThread(
											[=]() {
												QtAndroid::androidActivity().callMethod<void>("requestFitAccess", "()V");
											}
			);
			return true;
		}

		virtual bool revokeAccess() override
		{
			QtAndroid::runOnAndroidThread(
											[=]() {
												QtAndroid::androidActivity().callMethod<void>("revokeFitAccess", "()V");
											}
			);
			return true;
		}

		virtual GoogleFit::Family getFamily() const override
		{
			return GoogleFit::Family::Google;
		}

		virtual bool hasAccess() const override
		{
			bool retval(false);
			QtAndroid::runOnAndroidThreadSync(
												[&retval]() {
													retval = QtAndroid::androidActivity().callMethod<jboolean>("hasFitAccess", "()Z") == JNI_TRUE;
												}
			);
			return retval;
		}
	};

	FitnessTracker* FitnessTracker::Instance()
	{
		static Cadabra::Queen::Core::System::GoogleFit retval;
		return &retval;
	}
} } } }
