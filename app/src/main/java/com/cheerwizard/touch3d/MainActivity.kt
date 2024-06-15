package com.cheerwizard.touch3d

import android.app.Activity
import android.content.res.Configuration
import android.graphics.PixelFormat
import android.os.Build
import android.os.Bundle
import android.view.InputQueue
import android.view.Surface
import android.view.SurfaceHolder
import android.view.View
import android.view.ViewTreeObserver.OnGlobalLayoutListener
import android.view.WindowManager
import android.view.inputmethod.InputMethodManager

import java.io.File

class MainActivity : Activity(), SurfaceHolder.Callback2, InputQueue.Callback, OnGlobalLayoutListener {

    val location: IntArray = IntArray(2)
    var lastContentX: Int = 0
    var lastContentY: Int = 0
    var lastContentW: Int = 0
    var lastContentH: Int = 0

    private lateinit var mView: View
    private lateinit var mInputManager: InputMethodManager

    private var mCurrentSurfaceHolder: SurfaceHolder? = null
    private var mCurrentInputQueue: InputQueue? = null
    private var mDestroyed = false

    external fun nativeOnCreate()

    external fun nativeOnStart()

    external fun nativeOnResume()

    external fun nativeOnPause()

    external fun nativeOnStop()

    external fun nativeOnDestroy()

    external fun nativeOnConfigurationChanged()

    external fun nativeOnLowMemory()

    external fun nativeOnWindowFocusChanged(focused: Boolean)

    external fun nativeOnSurfaceCreated(surface: Surface)
    external fun nativeOnSurfaceChanged(
        surface: Surface,
        format: Int,
        width: Int, height: Int
    )
    external fun nativeOnSurfaceRedrawNeeded(surface: Surface)
    external fun nativeOnSurfaceDestroyed()

    external fun nativeOnInputQueueCreated(queuePtr: Long)
    external fun nativeOnInputQueueDestroyed(queuePtr: Long)

    external fun nativeOnContentRectChanged(x: Int, y: Int, w: Int, h: Int)

    override fun onCreate(savedInstanceState: Bundle?) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            mInputManager = getSystemService(InputMethodManager::class.java)
        } else {
            // TODO(cheerwizard): whole input and app won't work, so need workaround for lower SDK
            throw RuntimeException("getSystemService is not supported on SDK=${Build.VERSION.CODENAME}")
        }

        window.takeSurface(this)
        window.takeInputQueue(this)
        window.setFormat(PixelFormat.RGB_565)
        window.setSoftInputMode(
            WindowManager.LayoutParams.SOFT_INPUT_STATE_UNSPECIFIED
            or WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE
        )

        mView = View(this)
        setContentView(mView)
        mView.requestFocus()
        mView.viewTreeObserver.addOnGlobalLayoutListener(this)

        nativeOnCreate()
//        nativeOnRestoreInstanceState(nativeSavedState)

        super.onCreate(savedInstanceState)
    }

    private fun getAbsolutePath(file: File?): String? {
        return if ((file != null)) file.absolutePath else null
    }

    override fun onDestroy() {
        mDestroyed = true

        if (mCurrentSurfaceHolder != null) {
            nativeOnSurfaceDestroyed()
            mCurrentSurfaceHolder = null
        }

        if (mCurrentInputQueue != null) {
            nativeOnInputQueueDestroyed(mCurrentInputQueue.getNativePtr())
            mCurrentInputQueue = null
        }

        nativeOnDestroy()

        super.onDestroy()
    }

    override fun onPause() {
        super.onPause()
        nativeOnPause()
    }

    override fun onResume() {
        super.onResume()
        nativeOnResume()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
//        val state = nativeOnSaveInstanceState()
//        if (state != null) {
//            outState.putByteArray(K_SAVED_STATE, state)
//        }
    }

    override fun onRestoreInstanceState(savedInstanceState: Bundle) {
        super.onRestoreInstanceState(savedInstanceState)
//        val state = savedInstanceState.getByteArray(K_SAVED_STATE)
//        if (state != null) {
//            nativeOnRestoreInstanceState(state)
//        }
    }

    override fun onStart() {
        super.onStart()
        nativeOnStart()
    }

    override fun onStop() {
        super.onStop()
        nativeOnStop()
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        if (!mDestroyed) {
            nativeOnConfigurationChanged()
        }
    }

    override fun onLowMemory() {
        super.onLowMemory()
        if (!mDestroyed) {
            nativeOnLowMemory()
        }
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (!mDestroyed) {
            nativeOnWindowFocusChanged(hasFocus)
        }
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        if (!mDestroyed) {
            mCurrentSurfaceHolder = holder
            nativeOnSurfaceCreated(holder.surface)
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        if (!mDestroyed) {
            mCurrentSurfaceHolder = holder
            nativeOnSurfaceChanged(holder.surface, format, width, height)
        }
    }

    override fun surfaceRedrawNeeded(holder: SurfaceHolder) {
        if (!mDestroyed) {
            mCurrentSurfaceHolder = holder
            nativeOnSurfaceRedrawNeeded(holder.surface)
        }
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        mCurrentSurfaceHolder = null
        if (!mDestroyed) {
            nativeOnSurfaceDestroyed()
        }
    }

    override fun onInputQueueCreated(queue: InputQueue) {
        if (!mDestroyed) {
            mCurrentInputQueue = queue
            nativeOnInputQueueDestroyed(queue.getNativePtr())
        }
    }

    override fun onInputQueueDestroyed(queue: InputQueue) {
        if (!mDestroyed) {
            nativeOnInputQueueDestroyed(queue.getNativePtr())
            mCurrentInputQueue = null
        }
    }

    override fun onGlobalLayout() {
        mView.getLocationInWindow(location)
        val w = mView.width
        val h = mView.height
        if ((location[0] != lastContentX)
            || (location[1] != lastContentY)
            || (w != lastContentW)
            || (h != lastContentH)
        ) {
            lastContentX = location[0]
            lastContentY = location[1]
            lastContentW = w
            lastContentH = h

            if (!mDestroyed) {
                nativeOnContentRectChanged(
                    lastContentX, lastContentY,
                    lastContentW, lastContentH
                )
            }
        }
    }

    fun setWindowFlags(flags: Int, mask: Int) {
        window.setFlags(flags, mask)
    }

    fun setWindowFormat(format: Int) {
        window.setFormat(format)
    }

    fun showInput(mode: Int) {
        mInputManager.showSoftInput(mView, mode)
    }

    fun hideInput(mode: Int) {
        mInputManager.hideSoftInputFromWindow(mView.windowToken, mode)
    }
}

// TODO(cheerwizard): figure out, how to get the input queue native ptr
fun InputQueue?.getNativePtr(): Long = 0L