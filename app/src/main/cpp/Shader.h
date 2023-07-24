//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_SHADER_H
#define OPENGLDEMO_SHADER_H

#include <EGL/egl.h>

#include "Logger.h"

class Shader {
private:
    unsigned int triangleShaderProgram = -1;
    unsigned int textureShaderProgram = -1;

public:

    Shader() {
        initTriangleShader();
        initTextureShader();
    }

    ~Shader() {
        glDeleteProgram(triangleShaderProgram);
        glDeleteProgram(textureShaderProgram);
    }

    void useTriangleShader() {
        glUseProgram(triangleShaderProgram);
    }

    void useTextureShader() {
        glUseProgram(textureShaderProgram);
    }

    void unUse() {
        glUseProgram(0);
    }

    void setFloat(const GLchar *name,const GLsizei count, const GLfloat *value) {
        glUniform4fv(glGetUniformLocation(triangleShaderProgram, name),count,value);
    }

private:
    //编译链接着色器
    void initTriangleShader() {
        if (triangleShaderProgram != -1) {
            return;
        }
        // 顶点着色器
        const char *vertexShaderSource = "#version 300 es                          \n"
                                         "layout(location = 0) in vec4 vPosition;  \n"
                                         "void main()                              \n"
                                         "{                                        \n"
                                         "   gl_Position = vPosition;              \n"
                                         "}                                        \n";
        LOGI("vertexShaderSource = %s",vertexShaderSource)
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 521, NULL, infoLog);
            LOGI("Compile Shader error = %s", infoLog)
        }
        // 片段着色器
        const char *fragmentShaderSource = "#version 300 es                              \n"
                                           "precision mediump float;                     \n"
                                           "out vec4 fragColor;                          \n"
                                           "uniform vec4 ourColor;                       \n"
                                           "void main()                                  \n"
                                           "{                                            \n"
                                           "   fragColor = ourColor;  \n"
                                           "}                                            \n";
        LOGI("fragmentShaderSource = %s",fragmentShaderSource)
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 521, NULL, infoLog);
            LOGI("Compile fragment error = %s", infoLog)
        }

        //着色器程序
        triangleShaderProgram = glCreateProgram();
        glAttachShader(triangleShaderProgram, vertexShader);
        glAttachShader(triangleShaderProgram, fragmentShader);
        glLinkProgram(triangleShaderProgram);
        glGetProgramiv(triangleShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(triangleShaderProgram, 521, NULL, infoLog);
            LOGI("glLinkProgram error = %s", infoLog)
        }

        // 把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        LOGI("triangleShaderProgram init success , shader=%d , vertexShader=%d , fragmentShader=%d",
             triangleShaderProgram, vertexShader, fragmentShader)
    }

    //编译链接着色器
    void initTextureShader() {
        if (textureShaderProgram != -1) {
            return;
        }
        // 顶点着色器
        const char *vertexShaderSource = "#version 300 es                          \n"
                                         "layout(location = 0) in vec4 aPos;  \n"
                                         "layout (location = 1) in vec3 aColor;    \n"
                                         "layout (location = 2) in vec2 aTexCoord; \n"
                                         "out vec3 ourColor; \n"
                                         "out vec2 TexCoord; \n"
                                         "void main()                              \n"
                                         "{                                        \n"
                                         "   gl_Position = aPos;              \n"
                                         "   ourColor = aColor;              \n"
                                         "   TexCoord = aTexCoord;              \n"
                                         "}                                        \n";
        LOGI("vertexShaderSource = %s",vertexShaderSource)
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 521, NULL, infoLog);
            LOGE("Compile Shader error = %s", infoLog)
        }
        // 片段着色器
        const char *fragmentShaderSource = "#version 300 es                              \n"
                                           "precision mediump float;                     \n"
                                           "out vec4 fragColor;                          \n"
                                           "in vec3 ourColor;                       \n"
                                           "in vec2 TexCoord;                       \n"
                                           "uniform sampler2D ourTexture;                       \n"
                                           "void main()                                  \n"
                                           "{                                            \n"
                                           "   fragColor = texture(ourTexture, TexCoord);  \n"
                                           "}                                            \n";
        LOGI("fragmentShaderSource = %s",fragmentShaderSource)
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 521, NULL, infoLog);
            LOGE("Compile fragment error = %s", infoLog)
        }

        //着色器程序
        textureShaderProgram = glCreateProgram();
        glAttachShader(textureShaderProgram, vertexShader);
        glAttachShader(textureShaderProgram, fragmentShader);
        glLinkProgram(textureShaderProgram);
        glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(textureShaderProgram, 521, NULL, infoLog);
            LOGI("glLinkProgram error = %s", infoLog)
        }

        // 把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        LOGI("textureShaderProgram init success , shader=%d , vertexShader=%d , fragmentShader=%d",
             textureShaderProgram, vertexShader, fragmentShader)
    }

};

#endif //OPENGLDEMO_SHADER_H
