package com.example.opengldemo

import android.app.NativeActivity

class MainActivity : NativeActivity() {

    companion object {
        init {
            System.loadLibrary("OpenGLDemo")
        }
    }

}