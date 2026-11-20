#include "app_logger.h"
#include "string.h"

typedef struct
{
  size_t len;
  char buf[APP_LOGGER_MAX_MSG_LEN];
} app_log_msg_t;

static QueueHandle_t LogQueue = NULL;
static volatile uint32_t DroppedLogs = 0U;

static void LoggerTask(void *argument)
{
  (void)argument;
  app_log_msg_t msg;
  hal_uart_handle_t *p_usart2 = mx_usart2_uart_gethandle();

  for (;;)
  {
    if (xQueueReceive(LogQueue, &msg, portMAX_DELAY) == pdTRUE)
    {
      if (p_usart2 != NULL)
      {
        HAL_UART_Transmit(p_usart2, (uint8_t *)msg.buf, (uint16_t)msg.len, 100U);
      }
    }
  }
}

void app_log_send(const char *msg, size_t len)
{
  if (LogQueue == NULL)
  {
    return;
  }

  app_log_msg_t _msg;
  if (len > APP_LOGGER_MAX_MSG_LEN)
  {
    len = APP_LOGGER_MAX_MSG_LEN;
  }
  memcpy(_msg.buf, msg, len);
  _msg.len = len;

  if (xQueueSend(LogQueue, &_msg, 0U) != pdTRUE)
  {
    DroppedLogs++;
  }
}


uint32_t app_logger_get_dropped(void)
{
  return DroppedLogs;
}

app_status_t app_logger_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  LogQueue = xQueueCreate(APP_LOGGER_QUEUE_DEPTH, sizeof(app_log_msg_t));
  if (LogQueue == NULL)
  {
    return EXEC_STATUS_ERROR;
  }

  if (xTaskCreate(LoggerTask, "LoggerTask", APP_LOGGER_TASK_STACK,
                  NULL, APP_LOGGER_TASK_PRIO, NULL) != pdPASS)
  {
    vQueueDelete(LogQueue);
    LogQueue = NULL;
    return EXEC_STATUS_ERROR;
  }

  return return_status;
}
