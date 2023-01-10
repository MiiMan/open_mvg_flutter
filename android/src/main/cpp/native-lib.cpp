#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_open_1mvg_1flutter_1java_bridge_OpenMVG_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello";


    return env->NewStringUTF(hello.c_str());
}