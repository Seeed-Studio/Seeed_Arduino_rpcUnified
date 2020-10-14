#define TAG "WIFI CALLBACK"
#include "Arduino.h"
#include "rtl_wifi_unified.h"
#include "../erpc_shim/erpc_shim_unified.h"
#include "./esp_lib/esp_lib_unified.h"
#include "rpc_unified_log.h"
#include "erpc_port.h"
system_event_cb_t ptr_wifi_event_callback = NULL;

void rpc_wifi_event_callback(const binary_t *event)
{
    FUNC_ENTRY;
    system_event_t *event_data = (system_event_t *)event->data;
    uint32_t err = RTW_SUCCESS;

    if (ptr_wifi_event_callback != NULL)
    {
        esp_err_t ret = ptr_wifi_event_callback(NULL, event_data);
        if(ret != ESP_OK)
        {
            err = RTW_ERROR;
        }
    }
    FUNC_EXIT;
}


