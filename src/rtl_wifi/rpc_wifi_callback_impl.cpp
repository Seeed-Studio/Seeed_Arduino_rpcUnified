#define TAG "WIFI CALLBACK"
#include "Arduino.h"
#include "rtl_wifi_unified.h"
#include "erpc/erpc_shim_unified.h"
#include "erpc/erpc_port.h"
#include "esp/esp_lib_unified.h"
#include "rpc_unified_log.h"
#include "lwip/dns.h"

system_event_cb_t ptr_wifi_event_callback = NULL;

void rpc_wifi_dns_found(const char *hostname, const binary_t *ipaddr, const binary_t *arg)
{
    FUNC_ENTRY;
    uint32_t *ptr_args;
    dns_found_callback ptr_found;

    memcpy(&ptr_found, arg->data, 4);
    memcpy(&ptr_args, arg->data + 4, 4);
    ptr_found(hostname, (ip_addr_t *)ipaddr->data, (void *)ptr_args);
    FUNC_EXIT;
}

void rpc_wifi_event_callback(const binary_t *event)
{
    FUNC_ENTRY;
    system_event_t *event_data = (system_event_t *)event->data;
    uint32_t err = RTW_SUCCESS;

    if (ptr_wifi_event_callback != NULL)
    {
        esp_err_t ret = ptr_wifi_event_callback(NULL, event_data);
        if (ret != ESP_OK)
        {
            err = RTW_ERROR;
        }
    }
    FUNC_EXIT;
}