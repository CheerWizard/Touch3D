#include <Math.hpp>

static bool TestMath() {
    return true;
}

#if defined(T3D_ANDROID)

#include <Jni.hpp>

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MathNativeTest_test(
        JNIEnv *env, jobject thiz
) {
    TestMath();
}

#else

int main() {
    TestMath();
    return 0;
}

#endif