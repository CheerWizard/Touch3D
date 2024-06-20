package com.cheerwizard.touch3d

import dalvik.annotation.optimization.CriticalNative

object NativeLog {
    @CriticalNative
    external fun v(tag: String, log: String)
    @CriticalNative
    external fun i(tag: String, log: String)
    @CriticalNative
    external fun d(tag: String, log: String)
    @CriticalNative
    external fun w(tag: String, log: String)
    @CriticalNative
    external fun e(tag: String, log: String)
}