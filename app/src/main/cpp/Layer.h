//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_LAYER_H
#define OPENGLDEMO_LAYER_H

#include <android_native_app_glue.h>
#include "vector"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "Utils.h"
#include "Render.h"
#include "Shader.h"

class Layer {

public:
    inline Layer() {};

    virtual ~Layer();

    virtual void handleInput(AInputEvent *event);

    virtual void draw(Render *render);

    virtual void init(Render *render){}

public:
    std::vector<Layer *> layers;

private:
    bool isInit = false;
protected:
    Shader* shader;

};

class TemplateLayer : public Layer {
    void handleInput(AInputEvent *event);

    void draw(Render *render);

    void init(Render *render){};
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
    unsigned int VBO = 0, VAO = 0 , VEO = 0;
    float* color = new float [0.0f,0.0f,0.0f,1.f];
    float dis = 0.005f;

    void init(Render *render) {
        shader = new Shader(render->app->activity->assetManager,"shader/triangle.vert","shader/triangle.frag");
        // 初始化代码（只运行一次 (除非你的物体频繁改变)）
        float vertices[] = {
                0.8f, 0.8f, 0.0f,   // 右上角
                0.8f, -0.8f, 0.0f,  // 右下角
                -0.8f, -0.8f, 0.0f, // 左下角
                -0.8f, 0.8f, 0.0f   // 左上角
        };
        unsigned int indices[] = {
                // 注意索引从0开始!
                // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
                // 这样可以由下标代表顶点组合成矩形
                0, 1, 3, // 第一个三角形
                1, 2, 3  // 第二个三角形
        };
        //创建 VAO,VBO,VEO
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VEO);
        // 绑定VAO
        glBindVertexArray(VAO);
        // 绑定缓冲,填充数据,把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 绑定索引，填充索引数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);
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
    ~TriangleLayer(){
        delete shader;
        glDeleteBuffers(1,&VAO);
        glDeleteBuffers(1,&VBO);
        glDeleteBuffers(1,&VEO);
    }

};

class TextureLayer : public Layer {
    void handleInput(AInputEvent *event);

    void draw(Render *render);

private:
    unsigned int texture;
    unsigned int VBO = 0, VAO = 0 , EBO = 0;

    void init(Render *render) {
        shader = new Shader(render->app->activity->assetManager,"shader/texture.vert","shader/texture.frag");
        // 初始化代码（只运行一次 (除非你的物体频繁改变)）
        float vertices[] = {
                //      ---- 位置 ----                     ---- 颜色 ----               - 纹理坐标 -
                0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
                0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
        };
        unsigned int indices[] = {
                // 注意索引从0开始!
                // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
                // 这样可以由下标代表顶点组合成矩形
                0, 1, 3, // 第一个三角形
                1, 2, 3  // 第二个三角形
        };
        //创建 VAO,VBO,VEO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // 绑定VAO
        glBindVertexArray(VAO);
        // 绑定缓冲,填充数据,把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 绑定索引，填充索引数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);
        // 设置 顶点属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 设置 颜色属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // 设置 纹理属性
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        //创建纹理
        texture = loadTexture(render->app,"1.jpeg");

        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

public:
    TextureLayer() {
    }
    ~TextureLayer(){
        glDeleteBuffers(1,&VAO);
        glDeleteBuffers(1,&VBO);
        glDeleteBuffers(1,&EBO);
    }

};

class CubeLayer : public Layer {
    void handleInput(AInputEvent *event);

    void draw(Render *render);

private:
    unsigned int texture;
    unsigned int VBO = 0, VAO = 0 , EBO = 0;
    float angle = 0.0f;
    float dis = 0.005f;

    void init(Render *render) {
        shader = new Shader(render->app->activity->assetManager,"shader/cube.vert","shader/cube.frag");
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        //创建 VAO,VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
//        glGenBuffers(1, &EBO);
        // 绑定VAO
        glBindVertexArray(VAO);
        // 绑定缓冲,填充数据,把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 设置 顶点属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 设置 纹理属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //创建纹理
        texture = loadTexture(render->app,"1.jpeg");

        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

public:
    CubeLayer() {
    }
    ~CubeLayer(){
        glDeleteBuffers(1,&VAO);
        glDeleteBuffers(1,&VBO);
    }

};

#endif //OPENGLDEMO_LAYER_H
