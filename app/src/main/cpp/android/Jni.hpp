#include <jni.h>

class Jni final {
public:
    static JavaVM* vm;
    static JNIEnv* Get();
};