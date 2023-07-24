//
// Created by max on 2023/7/21.
//

#ifndef OPENGLDEMO_UTILS_H
#define OPENGLDEMO_UTILS_H

#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>

unsigned int loadTexture(struct android_app *app, const char *path);


#endif //OPENGLDEMO_UTILS_H
