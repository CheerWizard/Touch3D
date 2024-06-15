package com.cheerwizard.touch3d

import org.junit.Test

class MathNativeTest {

    init {
        System.loadLibrary("T3D_TESTS_ANDROID")
    }

    @Test
    external fun test()

}