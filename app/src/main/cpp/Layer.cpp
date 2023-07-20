//
// Created by max on 2023/7/20.
//

#include "Layer.h"

void Layer::handleInput(AInputEvent *event) {
    std::for_each(layers.begin(), layers.end(), [&](Layer *layer) {
        layer->handleInput(event);
    });
}

void Layer::draw(EGLint width, EGLint height) {
    std::for_each(layers.begin(), layers.end(), [&](Layer *layer) {
        layer->draw(width, height);
    });
}

Layer::~Layer() {
    std::for_each(layers.begin(), layers.end(), [&](const Layer *item) {
        delete item;
    });
    layers.clear();
}

void BackgroundLayer::handleInput(AInputEvent *event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        state.x = AMotionEvent_getX(event, 0);
        state.y = AMotionEvent_getY(event, 0);
    }
}

void BackgroundLayer::draw(EGLint width, EGLint height) {
    glClearColor(state.x / (float) width, state.angle, state.y / (float) height, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
