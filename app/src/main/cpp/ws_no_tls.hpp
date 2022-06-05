#ifndef NATIVESOCKETKOMMUNICATION_WS_NO_TLS_HPP
#define NATIVESOCKETKOMMUNICATION_WS_NO_TLS_HPP
#include <string>
#include <iostream>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <android/log.h>
#include "json.hpp"

#define TAG "WebSocketNoTLS"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

typedef websocketpp::client<websocketpp::config::asio_client> clientNoSSL;
using json = nlohmann::json;
using namespace std;

bool connectOpen = false;

clientNoSSL c2;
clientNoSSL::connection_ptr con;
websocketpp::connection_hdl tmpHdl;
jobject activity;
JNIEnv *envGlobal;

void callJavaVoidMethod(string msg){
    jstring jstr = envGlobal->NewStringUTF(msg.c_str());
    jclass clazz = envGlobal->FindClass("com/nativesocketkommunication/MainActivity");
    jmethodID call = envGlobal->GetMethodID(clazz, "addMsg", "(Ljava/lang/String;I)V");
    envGlobal->CallVoidMethod(activity, call, jstr, 8388611);
}

void on_open(websocketpp::connection_hdl hdl) {
    // now it is safe to use the connection
    tmpHdl = hdl;
    connectOpen = true;
}

void on_close(websocketpp::connection_hdl hdl) {
    LOGD("Connection close from server");
    connectOpen = false;
    callJavaVoidMethod("Disconnect");
}

void on_message(websocketpp::connection_hdl, clientNoSSL::message_ptr msg) {
    LOGD("On message: %s", msg->get_payload().c_str());
    callJavaVoidMethod(msg->get_payload().c_str());
}

void on_fail(websocketpp::connection_hdl hdl){
    LOGD("On fail");
    callJavaVoidMethod("Connection failed");
    hdl.lock();
}

class WebSocketNoTLS {

public:
    string host;

    WebSocketNoTLS() {
    }

    WebSocketNoTLS(string host, jobject act, JNIEnv *env) {
        this->host = host;
        activity = act;
        envGlobal = env;
        c2.set_access_channels(websocketpp::log::alevel::all);
        c2.clear_access_channels(websocketpp::log::alevel::frame_payload);
        c2.set_error_channels(websocketpp::log::elevel::all);

        c2.init_asio();

        c2.set_open_handler(&on_open);
        c2.set_close_handler(&on_close);
        c2.set_message_handler(&on_message);
        c2.set_fail_handler(&on_fail);
    }

    bool run() {
        try {
            // Set logging to be pretty verbose (everything except message payloads)

            websocketpp::lib::error_code ec;
            con = c2.get_connection(host, ec);
            if (ec) {
                LOGD("could not create connection because: %s", ec.message().c_str());
                return false;
            }

            c2.connect(con);
            c2.run();
        }catch (const std::exception &e) {
            LOGD("Connect fail %s: ", e.what());
        } catch (websocketpp::lib::error_code e) {
            LOGD("Connect fail %s: ", e.message().c_str());
        } catch (...) {
            LOGD("Connect fail");
        }
        return true;
    }

    bool sendMsg(string msg) {
        try {
            c2.send(tmpHdl, msg.c_str(), websocketpp::frame::opcode::text);
        } catch (websocketpp::exception const &e) {
            LOGD("Send exception: %s", e.what());
            return false;
        }
        return true;
    }

    bool isConnect() {
        return connectOpen;
    }

    void close(){
        c2.close(tmpHdl, 1001, "Goodbye");
    }

};

#endif


