//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_RENDER_H
#define OPENGLDEMO_RENDER_H

#include <android_native_app_glue.h>
#include <EGL/egl.h>

class Layer;

class Render {
public:

    inline Render(android_app *pApp, Layer *layer) :
            app(pApp),
            layer_(layer),
            display_(EGL_NO_DISPLAY),
            surface_(EGL_NO_SURFACE),
            context_(EGL_NO_CONTEXT),
            width_(0),
            height_(0) {
        initRenderer(pApp);
    }

    virtual ~Render();

    void initSurface(android_app *pApp);

    void destroySurface();

    void handleInput(AInputEvent *event);

    void render();

private:

    void initRenderer(android_app *pApp);

    Layer *layer_;
    EGLDisplay display_;
    EGLConfig config_;
    EGLSurface surface_;
    EGLContext context_;

public:
    EGLint width_;
    EGLint height_;
    android_app *app;

};


#endif //OPENGLDEMO_RENDER_H
