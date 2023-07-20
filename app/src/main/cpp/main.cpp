//
// Created by max on 2023/7/19.
//

#include <android_native_app_glue.h>
#include <jni.h>

#include "Render.h"
#include "Logger.h"

static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    if (app->userData) {
        auto *render = reinterpret_cast<Render*>(app->userData);
        render->handleInput(event);
    }
    return 0;
}

static void handle_cmd(struct android_app *app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            LOGI("APP_CMD_INIT_WINDOW")
            app->userData = new Render(app);
            break;
        }
        case APP_CMD_TERM_WINDOW: {
            LOGI("APP_CMD_TERM_WINDOW")
            if (app->userData) {
                auto *render = reinterpret_cast<Render*>(app->userData);
                app->userData = nullptr;
                delete render;
            }
            break;
        }
        case APP_CMD_RESUME: {
            LOGI("APP_CMD_RESUME")
            if (app->userData) {
                auto *render = reinterpret_cast<Render*>(app->userData);
                render->isPause = false;
            }
            break;
        }
        case APP_CMD_PAUSE: {
            LOGI("APP_CMD_PAUSE")
            if (app->userData) {
                auto *render = reinterpret_cast<Render*>(app->userData);
                render->isPause = true;
            }
            break;
        }
        case APP_CMD_STOP: {
            LOGI("APP_CMD_STOP")
            break;
        }
        case APP_CMD_DESTROY: {
            LOGI("APP_CMD_DESTROY")
            break;
        }
        default:
            break;
    }
}

void android_main(struct android_app *app) {
    LOGI("Welcome to android_main")
    app->onAppCmd = handle_cmd;
    app->onInputEvent = handle_input;

    // 循环等待要做的事情。
    while (!app->destroyRequested) {
        int events;
        struct android_poll_source *source;
        // 获取需要处理的事件
        if (ALooper_pollAll(0, nullptr, &events, (void **) &source) >= 0) {
            if (source) {
                source->process(app, source);
            }
        }
        // do app something
        if (app->userData) {
            auto *pRenderer = reinterpret_cast<Render *>(app->userData);
            // Render a frame
            pRenderer->render();
        }
    }
}