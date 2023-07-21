//
// Created by max on 2023/7/20.
//

#include <algorithm>
#include <GLES3/gl3.h>
#include "Render.h"
#include "Logger.h"

void Render::initRenderer(android_app *pApp) {
    LOGI("initRenderer !")
    // 选择您的渲染属性
    constexpr EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };

    // Android 上的默认显示可能就是您想要的
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);

    // 找出有多少个配置
    EGLint numConfigs;
    eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
    // 获取配置列表
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

    // 找到我们喜欢的配置。
    // 如果我们不关心配置中的其他任何内容，可能会只获取第一个。
    // 否则就加入你自己的启发式
    config_ = *std::find_if(
            supportedConfigs.get(),
            supportedConfigs.get() + numConfigs,
            [&display](const EGLConfig &config) {
                EGLint red, green, blue, depth;
                if (eglGetConfigAttrib(display, config, EGL_RED_SIZE, &red)
                    && eglGetConfigAttrib(display, config, EGL_GREEN_SIZE,
                                          &green)
                    && eglGetConfigAttrib(display, config, EGL_BLUE_SIZE,
                                          &blue)
                    && eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE,
                                          &depth)) {
                    LOGI("Found config with: %d , %d , %d , %d", red,
                         green, blue, depth);
                    return red == 8 && green == 8 && blue == 8 &&
                           depth == 24;
                }
                return false;
            }
    );
    LOGI("Found %d Configs", numConfigs)

    // 创建 GLES 3 上下文
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    EGLContext context = eglCreateContext(display, config_, nullptr, contextAttribs);
    display_ = display;
    context_ = context;
    // 检查系统上的 openGL
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name: opengl_info) {
        auto info = glGetString(name);
        LOGI("OpenGL Info: %s", info);
    }
    initSurface(pApp);
    LOGI("init completed!")

}

void Render::initSurface(android_app *pApp) {
    // 创建合适的 window surface
    surface_ = eglCreateWindowSurface(display_, config_, pApp->window, nullptr);
    //获取一些窗口指标
    if (eglMakeCurrent(display_, surface_, surface_, context_) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent")
        return;
    }
    eglQuerySurface(display_, surface_, EGL_WIDTH, &width_);
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &height_);
    LOGI("QuerySurface : width = %d , height = %d", width_, height_)
}

void Render::destroySurface() {
    if (surface_ != EGL_NO_SURFACE) {
        eglDestroySurface(display_, surface_);
        surface_ = EGL_NO_SURFACE;
    }
}

Render::~Render() {
    if (display_ != EGL_NO_DISPLAY) {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context_ != EGL_NO_CONTEXT) {
            eglDestroyContext(display_, context_);
            context_ = EGL_NO_CONTEXT;
        }
        if (surface_ != EGL_NO_SURFACE) {
            eglDestroySurface(display_, surface_);
            surface_ = EGL_NO_SURFACE;
        }
        eglTerminate(display_);
        display_ = EGL_NO_DISPLAY;
    }
}

void Render::handleInput(AInputEvent *event) {
    layer_->handleInput(event);
}

void Render::render() {
    if (display_ == nullptr) {
        LOGI("No display.")
        return;
    }
    glViewport(0,0,width_,height_);
    layer_->draw(this);
    eglSwapBuffers(display_, surface_);
}
