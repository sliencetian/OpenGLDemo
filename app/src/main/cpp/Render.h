//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_RENDER_H
#define OPENGLDEMO_RENDER_H

#include <android_native_app_glue.h>
#include <EGL/egl.h>

struct android_app;

struct saved_state {
    float angle;
    float x;
    float y;
};

class Render {
public:

    inline Render(android_app *pApp) :
            app_(pApp),
            display_(EGL_NO_DISPLAY),
            surface_(EGL_NO_SURFACE),
            context_(EGL_NO_CONTEXT),
            width_(0),
            height_(0) {
        initRenderer();
    }

    virtual ~Render();

    void handleInput(AInputEvent* event);

    void render();

private:

    void initRenderer();

    android_app *app_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLint width_;
    EGLint height_;

public:
    bool isPause = false;
    saved_state state;

};


#endif //OPENGLDEMO_RENDER_H
