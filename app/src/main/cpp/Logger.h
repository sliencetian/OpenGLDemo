//
// Created by max on 2023/7/20.
//

#ifndef OPENGLDEMO_LOGGER_H
#define OPENGLDEMO_LOGGER_H

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GLDemo-native", __VA_ARGS__));
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "GLDemo-native", __VA_ARGS__));

class Logger {

};


#endif //OPENGLDEMO_LOGGER_H
