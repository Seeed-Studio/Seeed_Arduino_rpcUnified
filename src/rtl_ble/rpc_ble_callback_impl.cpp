#include "Arduino.h"
#include "rtl_ble_unified.h"

//typedef void(*P_FUN_HABDLE_GAP_MSG)(T_IO_MSG *p_gap_msg);
P_FUN_LE_APP_CB _ble_gap_callback = NULL;
// P_FUN_HABDLE_GAP_MSG _handle_gap_msg = NULL;

//void ble_handle_gap_msg(T_IO_MSG *p_gap_msg)

void rpc_ble_handle_gap_msg(const binary_t *gap_msg)
{
    // Serial.printf("rpc_ble_handle_gap_msg\n\r");
    // for (int i = 0; i < gap_msg->dataLength; i++)
    // {
    //     Serial.printf("%02x, ", gap_msg->data[i]);
    // }
    // Serial.printf("\n\r");
    // if(_handle_gap_msg)
    // {
    //     _handle_gap_msg(gap_msg->data);
    // }

}

void rpc_ble_gap_callback(uint8_t cb_type, const binary_t *cb_data)
{
    // T_LE_SCAN_INFO *p_data = (T_LE_SCAN_INFO *)cb_data->data;
    // Serial.printf("rpc_ble_gap_callback cb_type:%d\n\r", cb_type);
    // Serial.printf("bt_addr: %02x:%02x:%02x:%02x:%02x:%02x\n\r",
    //               (p_data->bd_addr)[5],
    //               (p_data->bd_addr)[4],
    //               (p_data->bd_addr)[3],
    //               (p_data->bd_addr)[2],
    //               (p_data->bd_addr)[1],
    //               (p_data->bd_addr)[0]);
    // for (int i = 0; i < cb_data->dataLength; i++)
    // {
    //     Serial.printf("%0x, ", cb_data->data[i]);
    // }
    // Serial.printf("\n\r");
    T_LE_CB_DATA *p_cb_data = (T_LE_CB_DATA *)malloc(sizeof(T_LE_CB_DATA));
    p_cb_data->p_le_scan_info = (T_LE_SCAN_INFO *)cb_data->data;
    if(_ble_gap_callback != NULL)
        _ble_gap_callback(cb_type, p_cb_data);
    free(p_cb_data);
}
