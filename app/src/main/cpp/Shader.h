//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_SHADER_H
#define OPENGLDEMO_SHADER_H

#include <EGL/egl.h>

#include "android_native_app_glue.h"
#include "Logger.h"
#include "Utils.h"

class Shader {
private:
    unsigned int shaderProgram = -1;

public:

    Shader(AAssetManager *assetManager,const char* vertexPath,const char* fragmentPath){
        init(assetManager,vertexPath,fragmentPath);
    }

    ~Shader() {
        glDeleteProgram(shaderProgram);
    }

    void use() {
        glUseProgram(shaderProgram);
    }

    void unUse() {
        glUseProgram(0);
    }

    void setFloat(const GLchar *name,const GLsizei count, const GLfloat *value) {
        glUniform4fv(glGetUniformLocation(shaderProgram, name),count,value);
    }

private:
    void init(AAssetManager *assetManager,const char* vertexPath,const char* fragmentPath){
        auto vertexCode = loadAssetFile(assetManager,vertexPath);
        LOGI("vertexCode = %s",vertexCode)
        // 顶点着色器
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexCode, NULL);
        glCompileShader(vertexShader);
        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 521, NULL, infoLog);
            LOGI("Compile Shader error = %s", infoLog)
        }
        // 片段着色器
        auto fragmentCode = loadAssetFile(assetManager,fragmentPath);
        LOGI("fragmentCode = %s",fragmentCode)
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
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
        LOGI("shaderProgram init success , shader=%d , vertexShader=%d , fragmentShader=%d",
             shaderProgram, vertexShader, fragmentShader)
    }

};

#endif //OPENGLDEMO_SHADER_H
