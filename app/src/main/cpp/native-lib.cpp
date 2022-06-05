#include <jni.h>
#include "ws_no_tls.hpp"

WebSocketNoTLS wsOrch;

string convertIntoString(JNIEnv *env, jstring jniString){
    const jclass stringClass = env->GetObjectClass(jniString);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jniString, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string msg = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);
    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);

    return msg;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_nativesocketkommunication_MainActivity_sendMessage(JNIEnv *env,jobject thiz,
                                                                           jstring base64_string) {
    string msg = convertIntoString(env, base64_string);
    if(wsOrch.isConnect()){
        LOGD("Online");
    }
    wsOrch.sendMsg(msg);
    return 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nativesocketkommunication_MainActivity_initLocal(JNIEnv *env, jobject thiz,
                                                                         jstring host,
                                                                         jobject activity) {
    wsOrch = WebSocketNoTLS(convertIntoString(env, host), activity, env);
    wsOrch.run();
}