/**
  ******************************************************************************
  * File Name          : app_logger.h
  * Description        : This file provides the interface for logger.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_LOGGER_H
#define APP_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "app.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <stdio.h>

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define APP_NAME      			"ArtNet"

#define APP_LOGGER_ERROR_STR   "ERROR"
#define APP_LOGGER_WARNING_STR "WARNING"
#define APP_LOGGER_INFO_STR    "INFO"

#define APP_LOGGER_MAX_MSG_LEN	128U
#define APP_LOGGER_QUEUE_DEPTH 	16U
#define APP_LOGGER_TASK_PRIO	(tskIDLE_PRIORITY + 1U)
#define APP_LOGGER_TASK_STACK	512U

app_status_t app_logger_init(void);
uint32_t app_logger_get_dropped(void);


void AppLoggerHeader(const char *level, const char *name);

#define APP_LOG(level, name, ...) do {                                            \
    char app_log_buf[APP_LOGGER_MAX_MSG_LEN];                                        \
    int app_log_len = snprintf(app_log_buf, sizeof(app_log_buf),                   \
                               "[%8lu] [%s] %s - ",                              \
                               (unsigned long)xTaskGetTickCount(), level, name);  \
    int app_log_fmt = snprintf((app_log_len > 0) ?                                \
                               &app_log_buf[app_log_len] : app_log_buf,           \
                               (app_log_len > 0) ?                                 \
                               sizeof(app_log_buf) - (size_t)app_log_len : 0,     \
                               __VA_ARGS__);                                      \
    if (app_log_fmt >= 0) app_log_len += app_log_fmt;                              \
    if (app_log_len > (int)(APP_LOGGER_MAX_MSG_LEN - 2))                             \
        app_log_len = (int)(APP_LOGGER_MAX_MSG_LEN - 2);                              \
    if (app_log_len > 0)                                                           \
    {                                                                              \
        app_log_buf[app_log_len++] = '\r';                                         \
        app_log_buf[app_log_len++] = '\n';                                         \
        app_log_send(app_log_buf, (size_t)app_log_len);                           \
    }                                                                              \
} while (0)

void app_log_send(const char *msg, size_t len);

#define APP_LOG_ERROR(name, ...)   APP_LOG(APP_LOGGER_ERROR_STR,   name, __VA_ARGS__)
#define APP_LOG_WARNING(name, ...) APP_LOG(APP_LOGGER_WARNING_STR, name, __VA_ARGS__)
#define APP_LOG_INFO(name, ...)    APP_LOG(APP_LOGGER_INFO_STR,    name, __VA_ARGS__)



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_LOGGER_H */
