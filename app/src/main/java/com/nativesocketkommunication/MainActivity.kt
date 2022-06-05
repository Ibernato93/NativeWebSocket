package com.nativesocketkommunication

import android.app.Activity
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.Gravity
import android.widget.Button
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TextView
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    // load the native library on initialization of this class
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    private lateinit var linearChat: LinearLayout
    private external fun initLocal(host: String, activity: Activity)
    private external fun sendMessage(msg: String): Int;

    private lateinit var sendMsg: Button
    private lateinit var message: EditText

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        linearChat = findViewById(R.id.linear_chat)
        message = findViewById(R.id.message)
        this.sendMsg = findViewById(R.id.sendMsg)
        this.sendMsg.setOnClickListener(){
            if(!message.text.isEmpty()) {
                addMsg(message.text.toString(), 0)
                val ss = message.text.toString()
                message.setText("")
                sendMessage(ss)
            }
        }
        completeConnection()
    }

    private fun completeConnection() {
        // run the remote TCP/IP calls on an IO thread
        GlobalScope.launch(Dispatchers.IO) {
            var incomingData = ""

            // change to your remote server IP and port
            initLocal("ws://192.168.1.11:9001", this@MainActivity);
        }
    }

    fun addMsg(msg: String, gravity: Int){
        val textView = TextView(this)
        textView.text = msg
        textView.setBackgroundColor(resources.getColor(R.color.colorPrimary))
        textView.setPadding(20,20,20,20)
        textView.setTextColor(Color.WHITE)
        textView.textSize = 18.0f
        val param: LinearLayout.LayoutParams = LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
        LinearLayout.LayoutParams.WRAP_CONTENT)
        if(gravity == 0){
            param.gravity = Gravity.END
        }else{
            param.gravity = Gravity.START
        }
        param.topMargin = 10
        textView.layoutParams = param
        runOnUiThread {
            linearChat.addView(textView)
        }
    }
}
