#include <Jni.hpp>
#include <Log.hpp>

JavaVM* Jni::vm = nullptr;

JNIEnv* Jni::Get() {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LogAssert(false, "Failed to get JNIEnv!", "");
    }
    return env;
}