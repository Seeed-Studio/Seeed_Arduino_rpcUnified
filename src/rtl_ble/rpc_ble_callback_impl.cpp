#include "Arduino.h"
#include "rtl_ble_unified.h"

P_FUN_LE_APP_CB _ble_gap_callback = NULL;
P_FUN_HABDLE_GAP_MSG _handle_gap_msg = NULL;

void rpc_ble_handle_gap_msg(const binary_t *gap_msg)
{
    // Serial.printf("rpc_ble_handle_gap_msg\n\r");
    // for (int i = 0; i < gap_msg->dataLength; i++)
    // {
    //     Serial.printf("%02x, ", gap_msg->data[i]);
    // }
    // Serial.printf("\n\r");

    // ble_handle_gap_msg((T_IO_MSG *)gap_msg->data);
    if (_handle_gap_msg)
    {
        _handle_gap_msg((T_IO_MSG *)gap_msg->data);
    }
}

RPC_T_APP_RESULT rpc_ble_gap_callback(uint8_t cb_type, const binary_t *cb_data)
{
    RPC_T_APP_RESULT result = RPC_APP_RESULT_SUCCESS;

    T_LE_CB_DATA *p_cb_data = (T_LE_CB_DATA *)malloc(sizeof(T_LE_CB_DATA));

    switch (cb_type)
    {
    case GAP_MSG_LE_MODIFY_WHITE_LIST:
    {
        p_cb_data->p_le_modify_white_list_rsp = (T_LE_MODIFY_WHITE_LIST_RSP *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_CONN_UPDATE_IND:
    {
        p_cb_data->p_le_conn_update_ind = (T_LE_CONN_UPDATE_IND *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_PHY_UPDATE_INFO:
    {
        p_cb_data->p_le_phy_update_info = (T_LE_PHY_UPDATE_INFO *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_REMOTE_FEATS_INFO:
    {
        p_cb_data->p_le_remote_feats_info = (T_LE_REMOTE_FEATS_INFO *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_SCAN_INFO:
    {
        p_cb_data->p_le_scan_info = (T_LE_SCAN_INFO *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_ADV_UPDATE_PARAM:
    {
        p_cb_data->p_le_adv_update_param_rsp = (T_LE_ADV_UPDATE_PARAM_RSP *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_CREATE_CONN_IND:
    {
        p_cb_data->p_le_create_conn_ind = (T_LE_CREATE_CONN_IND *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_SET_RAND_ADDR:
    {
        p_cb_data->p_le_set_rand_addr_rsp = (T_LE_SET_RAND_ADDR_RSP *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_SET_HOST_CHANN_CLASSIF:
    {
        p_cb_data->p_le_set_host_chann_classif_rsp = (T_LE_SET_HOST_CHANN_CLASSIF_RSP *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_READ_RSSI:
    {
        p_cb_data->p_le_read_rssi_rsp = (T_LE_READ_RSSI_RSP *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_SET_DATA_LEN:
    {
        p_cb_data->p_le_set_data_len_rsp = (T_LE_SET_DATA_LEN_RSP *)cb_data->data;
        break;
    }
    case GAP_MSG_LE_DATA_LEN_CHANGE_INFO:
    {
        p_cb_data->p_le_data_len_change_info = (T_LE_DATA_LEN_CHANGE_INFO *)cb_data->data;
        break;
    }
    default:
    {
        break;
    }
    }

    if (_ble_gap_callback != NULL)
        result = (RPC_T_APP_RESULT)_ble_gap_callback(cb_type, p_cb_data);

    free(p_cb_data);

    return result;
}
