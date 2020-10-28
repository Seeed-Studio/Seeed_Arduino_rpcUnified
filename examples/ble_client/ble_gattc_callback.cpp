#include "Arduino.h"
#include "seeed_rpcUnified.h"
#include "rtl_ble/ble_unified.h"
T_APP_RESULT ble_gatt_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_BLE_CLIENT_CB_DATA *p_ble_client_cb_data = (T_BLE_CLIENT_CB_DATA *)p_data;
    Serial.printf("ble_gatt_client_callback conn_id:%d, client_id: %d, app_id:%d, cb_type:%d\n\r", conn_id, client_id, p_ble_client_cb_data->app_id, p_ble_client_cb_data->cb_type);

    switch (p_ble_client_cb_data->cb_type)
    {
    case BLE_CLIENT_CB_TYPE_DISCOVERY_STATE:
        Serial.printf("discov_state:%d\n\r", p_ble_client_cb_data->cb_content.discov_state.state);
        break;
    case BLE_CLIENT_CB_TYPE_DISCOVERY_RESULT:
    {
        T_DISCOVERY_RESULT_TYPE discov_type = p_ble_client_cb_data->cb_content.discov_result.discov_type;
        switch (discov_type)
        {
            Serial.printf("discov_type:%d\n\r", discov_type);
        case DISC_RESULT_ALL_SRV_UUID16:
        {
            T_GATT_SERVICE_ELEM16 *disc_data = (T_GATT_SERVICE_ELEM16 *)&(p_ble_client_cb_data->cb_content.discov_result.result.srv_uuid16_disc_data);
            Serial.printf("start_handle:%d, end handle:%d UUID16:%02x%02x\n\r",
                          disc_data->att_handle, disc_data->end_group_handle, (uint8_t)(disc_data->uuid16 >> 8), disc_data->uuid16 & 0xFF);
            break;
        }
        case DISC_RESULT_ALL_SRV_UUID128:
        {
            T_GATT_SERVICE_ELEM128 *disc_data = (T_GATT_SERVICE_ELEM128 *)&(p_ble_client_cb_data->cb_content.discov_result.result.srv_uuid128_disc_data);
            Serial.printf("start_handle:%d, end handle:%d UUID128:%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x\n\r",
                          disc_data->att_handle, disc_data->end_group_handle,
                          disc_data->uuid128[15], disc_data->uuid128[14], disc_data->uuid128[13], disc_data->uuid128[12],
                          disc_data->uuid128[11], disc_data->uuid128[10], disc_data->uuid128[9], disc_data->uuid128[8],
                          disc_data->uuid128[7], disc_data->uuid128[6], disc_data->uuid128[5], disc_data->uuid128[4],
                          disc_data->uuid128[3], disc_data->uuid128[2], disc_data->uuid128[1], disc_data->uuid128[0]);
            break;
        }
        case DISC_RESULT_SRV_DATA:
        {
            T_GATT_SERVICE_BY_UUID_ELEM *disc_data = (T_GATT_SERVICE_BY_UUID_ELEM *)&(p_ble_client_cb_data->cb_content.discov_result.result.srv_disc_data);
            Serial.printf("start_handle:%d, end handle:%d\n\r", disc_data->att_handle, disc_data->end_group_handle);
            break;
        }
        case DISC_RESULT_CHAR_UUID16:
        {
            T_GATT_CHARACT_ELEM16 *disc_data = (T_GATT_CHARACT_ELEM16 *)&(p_ble_client_cb_data->cb_content.discov_result.result.char_uuid16_disc_data);
            Serial.printf("decl_handle:%d, properties:%d value_handle:%d UUID16:%02x02x\n\r", disc_data->decl_handle, disc_data->properties, disc_data->value_handle, (uint8_t)(disc_data->uuid16 >> 8), disc_data->uuid16 & 0xFF);
            break;
        }
        case DISC_RESULT_CHAR_UUID128:
        {
            T_GATT_CHARACT_ELEM128 *disc_data = (T_GATT_CHARACT_ELEM128 *)&(p_ble_client_cb_data->cb_content.discov_result.result.char_uuid128_disc_data);
            Serial.printf("decl_handle:%d, properties:%d value_handle:%d UUID128:%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x\n\r", disc_data->decl_handle, disc_data->properties, disc_data->value_handle,
                          disc_data->uuid128[15], disc_data->uuid128[14], disc_data->uuid128[13], disc_data->uuid128[12],
                          disc_data->uuid128[11], disc_data->uuid128[10], disc_data->uuid128[9], disc_data->uuid128[8],
                          disc_data->uuid128[7], disc_data->uuid128[6], disc_data->uuid128[5], disc_data->uuid128[4],
                          disc_data->uuid128[3], disc_data->uuid128[2], disc_data->uuid128[1], disc_data->uuid128[0]);
            break;
        }
        case DISC_RESULT_CHAR_DESC_UUID16:
        {
            T_GATT_CHARACT_DESC_ELEM16 *disc_data = (T_GATT_CHARACT_DESC_ELEM16 *)&(p_ble_client_cb_data->cb_content.discov_result.result.char_desc_uuid16_disc_data);
            Serial.printf("handle:%d, UUID16:%02x02x\n\r", disc_data->handle, (uint8_t)(disc_data->uuid16 >> 8), disc_data->uuid16 & 0xFF);
            break;
        }
        case DISC_RESULT_CHAR_DESC_UUID128:
        {
            T_GATT_CHARACT_DESC_ELEM128 *disc_data = (T_GATT_CHARACT_DESC_ELEM128 *)&(p_ble_client_cb_data->cb_content.discov_result.result.char_desc_uuid128_disc_data);
            Serial.printf("handle:%d, UUID128:%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x\n\r", disc_data->handle,
                          disc_data->uuid128[15], disc_data->uuid128[14], disc_data->uuid128[13], disc_data->uuid128[12],
                          disc_data->uuid128[11], disc_data->uuid128[10], disc_data->uuid128[9], disc_data->uuid128[8],
                          disc_data->uuid128[7], disc_data->uuid128[6], disc_data->uuid128[5], disc_data->uuid128[4],
                          disc_data->uuid128[3], disc_data->uuid128[2], disc_data->uuid128[1], disc_data->uuid128[0]);
            break;
        }
        default:
            break;
        }
        break;
    }
    case BLE_CLIENT_CB_TYPE_READ_RESULT:
        break;
    case BLE_CLIENT_CB_TYPE_WRITE_RESULT:
        break;
    case BLE_CLIENT_CB_TYPE_NOTIF_IND:
        break;
    case BLE_CLIENT_CB_TYPE_DISCONNECT_RESULT:
        break;
    default:
        break;
    }
    return result;
}