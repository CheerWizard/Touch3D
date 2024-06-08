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

    companion object {
        const val K_LIB_NAME: String = "T3D"
        const val K_SAVED_STATE: String = "T3D_saved_state"

        init {
            System.loadLibrary(K_LIB_NAME)
        }
    }

    val location: IntArray = IntArray(2)
    var lastContentX: Int = 0
    var lastContentY: Int = 0
    var lastContentW: Int = 0
    var lastContentH: Int = 0

    private lateinit var _contentView: View
    private lateinit var _inputManager: InputMethodManager

    private var _currentSurfaceHolder: SurfaceHolder? = null
    private var _currentInputQueue: InputQueue? = null
    private var _destroyed = false

    private external fun nativeGetError(): String

    private external fun nativeOnCreate()
    private external fun nativeOnStart()
    private external fun nativeOnResume()
    private external fun nativeOnPause()
    private external fun nativeOnStop()
    private external fun nativeOnDestroy()

    private external fun nativeOnSaveInstanceState(): ByteArray?
    private external fun nativeOnRestoreInstanceState(state: ByteArray?)

    private external fun nativeOnConfigurationChanged()

    private external fun nativeOnLowMemory()

    private external fun nativeOnWindowFocusChanged(focused: Boolean)

    private external fun nativeOnSurfaceCreated(surface: Surface)
    private external fun nativeOnSurfaceChanged(
        surface: Surface,
        format: Int,
        width: Int, height: Int
    )
    private external fun nativeOnSurfaceRedrawNeeded(surface: Surface)
    private external fun nativeOnSurfaceDestroyed()

    private external fun nativeOnInputQueueCreated(queuePtr: Long)
    private external fun nativeOnInputQueueDestroyed(queuePtr: Long)

    private external fun nativeOnContentRectChanged(x: Int, y: Int, w: Int, h: Int)

    override fun onCreate(savedInstanceState: Bundle?) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            _inputManager = getSystemService(InputMethodManager::class.java)
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

        _contentView = View(this)
        setContentView(_contentView)
        _contentView.requestFocus()
        _contentView.viewTreeObserver.addOnGlobalLayoutListener(this)

        val nativeSavedState = savedInstanceState?.getByteArray(K_SAVED_STATE)

        nativeOnCreate()
        nativeOnRestoreInstanceState(nativeSavedState)

        super.onCreate(savedInstanceState)
    }

    private fun getAbsolutePath(file: File?): String? {
        return if ((file != null)) file.absolutePath else null
    }

    override fun onDestroy() {
        _destroyed = true

        if (_currentSurfaceHolder != null) {
            nativeOnSurfaceDestroyed()
            _currentSurfaceHolder = null
        }

        if (_currentInputQueue != null) {
            nativeOnInputQueueDestroyed(_currentInputQueue.getNativePtr())
            _currentInputQueue = null
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
        val state = nativeOnSaveInstanceState()
        if (state != null) {
            outState.putByteArray(K_SAVED_STATE, state)
        }
    }

    override fun onRestoreInstanceState(savedInstanceState: Bundle) {
        super.onRestoreInstanceState(savedInstanceState)
        val state = savedInstanceState.getByteArray(K_SAVED_STATE)
        if (state != null) {
            nativeOnRestoreInstanceState(state)
        }
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
        if (!_destroyed) {
            nativeOnConfigurationChanged()
        }
    }

    override fun onLowMemory() {
        super.onLowMemory()
        if (!_destroyed) {
            nativeOnLowMemory()
        }
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (!_destroyed) {
            nativeOnWindowFocusChanged(hasFocus)
        }
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        if (!_destroyed) {
            _currentSurfaceHolder = holder
            nativeOnSurfaceCreated(holder.surface)
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        if (!_destroyed) {
            _currentSurfaceHolder = holder
            nativeOnSurfaceChanged(holder.surface, format, width, height)
        }
    }

    override fun surfaceRedrawNeeded(holder: SurfaceHolder) {
        if (!_destroyed) {
            _currentSurfaceHolder = holder
            nativeOnSurfaceRedrawNeeded(holder.surface)
        }
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        _currentSurfaceHolder = null
        if (!_destroyed) {
            nativeOnSurfaceDestroyed()
        }
    }

    override fun onInputQueueCreated(queue: InputQueue) {
        if (!_destroyed) {
            _currentInputQueue = queue
            nativeOnInputQueueDestroyed(queue.getNativePtr())
        }
    }

    override fun onInputQueueDestroyed(queue: InputQueue) {
        if (!_destroyed) {
            nativeOnInputQueueDestroyed(queue.getNativePtr())
            _currentInputQueue = null
        }
    }

    override fun onGlobalLayout() {
        _contentView.getLocationInWindow(location)
        val w = _contentView.width
        val h = _contentView.height
        if ((location[0] != lastContentX)
            || (location[1] != lastContentY)
            || (w != lastContentW)
            || (h != lastContentH)
        ) {
            lastContentX = location[0]
            lastContentY = location[1]
            lastContentW = w
            lastContentH = h

            if (!_destroyed) {
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
        _inputManager.showSoftInput(_contentView, mode)
    }

    fun hideInput(mode: Int) {
        _inputManager.hideSoftInputFromWindow(_contentView.windowToken, mode)
    }
}

// TODO(cheerwizard): figure out, how to get the input queue native ptr
fun InputQueue?.getNativePtr(): Long = 0L