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
        if (!layer->isInit) {
            layer->isInit = true;
            layer->init(render);
        }
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
    shader->use();
    glBindVertexArray(VAO);

    // 更新uniform颜色
    color[0] = color[0] + dis;
    color[1] = color[1] + dis;
    if (color[1] >= 1.0f) {
        color[0] = 1.0f;
        color[1] = 1.0f;
        dis = -0.005f;
    } else if (color[1] <= 0.0f) {
        color[0] = 0.0f;
        color[1] = 0.0f;
        dis = 0.005f;
    }
//    int vertexColorLocation = render->shader_->getUniformLocation("ourColor");
//    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
//    glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
//    glUniform4fv(vertexColorLocation,0,color);
//    glUniform4fv(vertexColorLocation,1,color);
    shader->setFloat("ourColor",0,color);
    shader->setFloat("ourColor",1,color);

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    shader->unUse();
}

void TriangleLayer::handleInput(AInputEvent *event) {

}

void TextureLayer::handleInput(AInputEvent *event) {
}

void TextureLayer::draw(Render *render) {
    // 绘制物体
    shader->use();
    glBindTexture(GL_TEXTURE_2D,texture);
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
    shader->unUse();
}

void CubeLayer::handleInput(AInputEvent *event) {

}

void CubeLayer::draw(Render *render) {
    // 绘制物体
    shader->use();
    glBindTexture(GL_TEXTURE_2D,texture);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES,0,36);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
    shader->unUse();
}
