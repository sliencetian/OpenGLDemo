//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_SHADER_H
#define OPENGLDEMO_SHADER_H

#include <EGL/egl.h>

#include "Logger.h"

class Shader {
private:
    unsigned int shaderProgram = -1;

public:

    Shader() {
        init();
    }

    ~Shader() {
        glDeleteProgram(shaderProgram);
    }

    void use() {
        glUseProgram(shaderProgram);
    }

    void setFloat(const GLchar *name,const GLsizei count, const GLfloat *value) {
        glUniform4fv(glGetUniformLocation(shaderProgram, name),count,value);
    }

private:
    //编译链接着色器
    void init() {
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
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 521, NULL, infoLog);
            LOGI("glLinkProgram error = %s", infoLog)
        }

        // 把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        LOGI("shader init success , shader=%d , vertexShader=%d , fragmentShader=%d",
             shaderProgram, vertexShader, fragmentShader)
    }

};

#endif //OPENGLDEMO_SHADER_H