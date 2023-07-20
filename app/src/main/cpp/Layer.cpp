//
// Created by max on 2023/7/20.
//

#include "Layer.h"

#include "Render.h"

void Layer::handleInput(AInputEvent *event) {
    std::for_each(layers.begin(), layers.end(), [&](Layer *layer) {
        layer->handleInput(event);
    });
}

void Layer::draw(Render *render) {
    std::for_each(layers.begin(), layers.end(), [&](Layer *layer) {
        layer->draw(render);
    });
}

Layer::~Layer() {
    std::for_each(layers.begin(), layers.end(), [&](const Layer *item) {
        delete item;
    });
    layers.clear();
}

void TemplateLayer::draw(Render *render) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TemplateLayer::handleInput(AInputEvent *event) {
}

void BackgroundLayer::handleInput(AInputEvent *event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        state.x = AMotionEvent_getX(event, 0);
        state.y = AMotionEvent_getY(event, 0);
    }
}

void BackgroundLayer::draw(Render *render) {
    glClearColor(state.x / (float) render->width_,
                 state.angle,
                 state.y / (float) render->height_,
                 1);
    glClear(GL_COLOR_BUFFER_BIT);
}


void TriangleLayer::draw(Render *render) {
    // 绘制物体
    init();
    render->shader_->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void TriangleLayer::handleInput(AInputEvent *event) {

}
