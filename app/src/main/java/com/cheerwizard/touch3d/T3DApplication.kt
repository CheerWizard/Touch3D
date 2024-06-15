package com.cheerwizard.touch3d

import android.app.Application

class T3DApplication : Application() {

    init {
        System.loadLibrary("T3D")
    }

}