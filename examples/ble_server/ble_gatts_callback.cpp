#include "Arduino.h"
#include "seeed_rpcUnified.h"
#include "rtl_ble/ble_unified.h"
uint32_t value = 50;
extern bool isNotify; 
T_APP_RESULT onRead(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    Serial.printf("onRead: service_id: %d conn_id: %d; attrib_index: %d offset: %d\n\r", service_id, conn_id, attrib_index, offset);
    *p_length = sizeof(uint32_t);
    *pp_value = (uint8_t*)&value;
    return result;
}

T_APP_RESULT onWrite(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, const uint8_t *p_value)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    Serial.printf("onWrite: service_id: %d conn_id: %d; attrib_index: %d write_type: %d length:%d data: ", service_id, conn_id, attrib_index, write_type, length);
    for (int i = 0; i < length; i++)
    {
        Serial.printf("%02x ", p_value[i]);
    }
    Serial.printf("\n\r");
    return result;
}

T_APP_RESULT onStatus(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t cccbits)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    Serial.printf("onStatus: service_id: %d conn_id: %d; attrib_index: %d cccbits: %d\n\r", service_id, conn_id, index, cccbits);
    if(cccbits == 1)
    {
        isNotify = true;
    }else{
        isNotify = false;
    }
    return result;
}

T_APP_RESULT ble_gatt_server_callback(T_SERVER_ID service_id, void *p_para)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    ble_service_cb_data_t *cb_data = (ble_service_cb_data_t *)p_para;
    Serial.println("ble_gatt_server_callback");
    switch (cb_data->event)
    {
    case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
    {
        result = onStatus(cb_data->conn_id, service_id, cb_data->attrib_handle, cb_data->cb_data_context.cccd_update_data.cccbits);
        break;
    }
    case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE:
    {
        result = onRead(cb_data->conn_id, service_id, cb_data->attrib_handle, cb_data->cb_data_context.read_data.offset, &(cb_data->cb_data_context.read_data.length), &(cb_data->cb_data_context.read_data.p_value));
        break;
    }
    case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
    {
        result = onWrite(cb_data->conn_id, service_id, cb_data->attrib_handle, cb_data->cb_data_context.write_data.write_type, cb_data->cb_data_context.write_data.length, cb_data->cb_data_context.write_data.p_value);
        break;
    }
    default:
        break;
    }

    return result;
}