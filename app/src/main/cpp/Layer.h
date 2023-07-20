//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_LAYER_H
#define OPENGLDEMO_LAYER_H

#include <android_native_app_glue.h>
#include "vector"
#include <EGL/egl.h>
#include <GLES3/gl3.h>


class Layer {

public:
    inline Layer(){};

    virtual ~Layer();

    virtual void handleInput(AInputEvent *event);

    virtual void draw(EGLint width, EGLint height);

public:
    std::vector<Layer *> layers;

};

class BackgroundLayer : public Layer {
    struct state {
        float angle;
        float x;
        float y;
    };
private:
    state state;

    void handleInput(AInputEvent *event);

    void draw(EGLint width, EGLint height);
};


#endif //OPENGLDEMO_LAYER_H
