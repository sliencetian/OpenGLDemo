//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_LAYER_H
#define OPENGLDEMO_LAYER_H

#include <android_native_app_glue.h>
#include "vector"
#include <EGL/egl.h>
#include <GLES3/gl3.h>

class Render;

class Layer {

public:
    inline Layer() {};

    virtual ~Layer();

    virtual void handleInput(AInputEvent *event);

    virtual void draw(Render *render);

public:
    std::vector<Layer *> layers;

};

class TemplateLayer : public Layer {
    void handleInput(AInputEvent *event);

    void draw(Render *render);
};

class BackgroundLayer : public Layer {
    void handleInput(AInputEvent *event);

    void draw(Render *render);

private:
    struct state {
        float angle;
        float x;
        float y;
    };
    state state;

};

class TriangleLayer : public Layer {
    void handleInput(AInputEvent *event);

    void draw(Render *render);

private:
    unsigned int VBO = 0, VAO = 0;
    bool isInit = false;

    void init() {
        if (isInit) {
            return;
        }
        isInit = true;
        // 初始化代码（只运行一次 (除非你的物体频繁改变)）
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
        };
        //创建 VAO,VBO
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        // 绑定VAO
        glBindVertexArray(VAO);
        // 绑定缓冲
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // 填充数据,把顶点数组复制到缓冲中供OpenGL使用
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

public:
    TriangleLayer() {
    }

};


#endif //OPENGLDEMO_LAYER_H
