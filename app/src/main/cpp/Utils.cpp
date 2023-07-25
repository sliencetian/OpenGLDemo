//
// Created by max on 2023/7/21.
//

#include "Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Logger.h"

unsigned int loadTexture(struct android_app *app, const char *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // NDK 中的 AssetManager
    AAssetManager *mgr = app->activity->assetManager;
    // 打开 Asset 文件夹下的文件
    AAsset *pathAsset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);
    // 获取文件的长度
    off_t assetLength = AAsset_getLength(pathAsset);
    // 获得文件对应的 Buffer
    auto *fileData = (unsigned char *) AAsset_getBuffer(pathAsset);
    // 图片信息
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    // load图片
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load_from_memory(fileData, assetLength,
                                                &width, &height, &nrChannels, 0);
    LOGI("loadTexture , path=%s , assetLength=%ld , width=%d , height=%d , nrChannels=%d",
         path,assetLength,width,height,nrChannels)
    if (data) {
        GLint format = GL_RGBA;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // 设置纹理包裹参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // 设置纹理过滤参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    stbi_image_free(data);
    AAsset_close(pathAsset);
    return textureID;
}

char* loadAssetFile(AAssetManager *assetManager,const char *path) {
    char *buffer;
    LOGI("GLUtils::openTextFile path [%s]", path)
    AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        LOGE("Couldn't load %s", path)
        return nullptr;
    }
    off_t length = AAsset_getLength(asset);
    buffer = new char[length + 1];
    int num = AAsset_read(asset, buffer, length);
    AAsset_close(asset);
    if (num != length) {
        LOGE("Couldn't read %s", path)
        delete[] buffer;
        return nullptr;
    }
    buffer[length] = '\0';
    return buffer;
}