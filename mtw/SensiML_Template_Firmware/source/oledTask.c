#include "class_queue.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "GUI.h"
#include "mtb_ssd1306.h"
#include "mtb_ssd1306_i2c.h"
#include "cy8ckit_028_sense.h"
#include "FreeRTOS.h"
#include "task.h"
#include "oledTask.h"
#include "mqtt_client_config.h"
#include "publisher_task.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* I2C bus speed */
#define I2C_SPEED         (400000)
#define THRESOLD_MILLIS   (500)

extern QueueHandle_t xQueue;
cyhal_lptimer_t lptimer_obj;
cyhal_lptimer_info_t lptimer_obj_info;

void send_alert_message()
{
      publisher_data_t publisher_q_data;

      /* Assign the publish command to be sent to the publisher task. */
      publisher_q_data.cmd = PUBLISH_MQTT_MSG;
      publisher_q_data.data = (char *)MQTT_DEVICE_ON_MESSAGE;

      /* Send the command and data to publisher task over the queue */
      if (xQueueSend(publisher_task_q, &publisher_q_data, ( TickType_t ) 10)  != pdPASS) {
   	     printf("publisher_task_q: no queue space\r\n");
      }
}


void oled_task(void *arg)
{
    cy_rslt_t result;
    cyhal_i2c_t i2c_obj;

    /* Configuration to initialize the I2C block */
    cyhal_i2c_cfg_t i2c_config = {
        .is_slave = false,
        .address = OLED_I2C_ADDRESS,
        .frequencyhal_hz = I2C_SPEED
    };

    /* Initialize and configure the I2C to use with the OLED display */
    result = cyhal_i2c_init( &i2c_obj, CY8CKIT_028_SENSE_PIN_I2C_SDA,
                             CY8CKIT_028_SENSE_PIN_I2C_SCL, NULL);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    result = cyhal_i2c_configure(&i2c_obj, &i2c_config);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize the OLED display */
    result = mtb_ssd1306_init_i2c(&i2c_obj);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize the LPTIMER */
    result = cyhal_lptimer_init(&lptimer_obj);
    CY_ASSERT(result == CY_RSLT_SUCCESS);
    cyhal_lptimer_get_info(&lptimer_obj, &lptimer_obj_info);


    /* Initialize emWin GUI */
    GUI_Init();
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetTextAlign(GUI_TA_HCENTER);

    uint32_t continous_faucet_running_time = 0;
    uint32_t prev_millis = 0;
    int prev_cls_index = 0;
    int n = 1;

    for (;;)
    {
        class_t cls;
		uint32_t millis = (cyhal_lptimer_read(&lptimer_obj) * 1000) / lptimer_obj_info.frequency_hz;

        if (xQueueReceive(xQueue, &cls, portMAX_DELAY) != pdPASS) {
            //printf("queue read failed\r\n");
        } else {
        	if (cls.index >= 0) {
				if (cls.index == 0) {
					GUI_Clear();
					GUI_DispStringAt("UNKNOWN", 4, 20);
				} else if (cls.index == 1) {
					GUI_Clear();
					GUI_DispStringAt("FAUCET", 4, 20);
					if (prev_cls_index == 1) {
						continous_faucet_running_time += (millis - prev_millis);
						//printf("continous_faucet_running_time = %ld\r\n", continous_faucet_running_time);
						if (continous_faucet_running_time > THRESOLD_MILLIS * n) {
							n += 1;
							send_alert_message();
							printf("Faucet running time: %ld\r\n", continous_faucet_running_time);
						}
					} else {
						n = 1;
						continous_faucet_running_time = 0;
						printf("Time reinitialized\r\n");
					}

				} else if (cls.index == 2) {
					GUI_Clear();
					GUI_DispStringAt("NOISE", 4, 20);
				}
				prev_cls_index = cls.index;
        	}
        }
        prev_millis = millis;
     }
}



