//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_RENDER_H
#define OPENGLDEMO_RENDER_H

#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include "Layer.h"
#include "Shader.h"

class Render {
public:

    inline Render(android_app *pApp, Layer *layer) :
            layer_(layer),
            app_(pApp),
            display_(EGL_NO_DISPLAY),
            surface_(EGL_NO_SURFACE),
            context_(EGL_NO_CONTEXT),
            width_(0),
            height_(0) {
        initRenderer();
        shader_ = new Shader();
    }

    virtual ~Render();

    void handleInput(AInputEvent *event);

    void render();

private:

    void initRenderer();

    Layer *layer_;
    android_app *app_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;

public:
    EGLint width_;
    EGLint height_;
    Shader *shader_;

};


#endif //OPENGLDEMO_RENDER_H
