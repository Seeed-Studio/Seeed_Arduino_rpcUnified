#include <stdlib.h>
#include "rpc_ble_api_hal.h"
#include "rpc_ble_api.h"
#include "gap_adv.h"

extern P_FUN_LE_APP_CB _ble_gap_callback;


void le_register_app_cb(P_FUN_LE_APP_CB ble_gap_callback)
{
    _ble_gap_callback = _ble_gap_callback;
}

T_GAP_CAUSE le_adv_set_param(T_LE_ADV_PARAM_TYPE param, uint8_t len, void *p_value)
{
    binary_t value;
    value.dataLength = len;
    value.data = (uint8_t *)p_value;
    return (T_GAP_CAUSE)rpc_le_adv_set_param((RPC_T_LE_ADV_PARAM_TYPE)param, &value);
}

T_GAP_CAUSE le_adv_get_param(T_LE_ADV_PARAM_TYPE param, void *p_value)
{
    binary_t value;
    value.dataLength = 0;
    value.data = (uint8_t *)p_value;
    return (T_GAP_CAUSE)rpc_le_adv_get_param((RPC_T_LE_ADV_PARAM_TYPE)param, &value);
}

T_GAP_CAUSE le_adv_start(void)
{
    return (T_GAP_CAUSE)rpc_le_adv_start();
}

T_GAP_CAUSE le_adv_stop(void)
{
    return (T_GAP_CAUSE)le_adv_stop();
}

T_GAP_CAUSE le_adv_update_param(void)
{
    return (T_GAP_CAUSE)le_adv_update_param();
}
bool le_gap_init(uint8_t link_num)
{
    return rpc_le_gap_init(link_num);
}

void le_gap_msg_info_way(bool use_msg)
{
    return le_gap_msg_info_way(use_msg);
}

uint8_t le_get_max_link_num(void)
{
    return rpc_le_get_max_link_num();
}

T_GAP_CAUSE le_set_gap_param(T_GAP_LE_PARAM_TYPE param, uint8_t len, void *p_value)
{
    binary_t value;
    value.dataLength = len;
    value.data = (uint8_t *)p_value;
    return (T_GAP_CAUSE)rpc_le_set_gap_param((RPC_T_GAP_LE_PARAM_TYPE)param, &value);
}

T_GAP_CAUSE le_get_gap_param(T_GAP_LE_PARAM_TYPE param, void *p_value)
{
    binary_t value;
    value.dataLength = 0;
    value.data = (uint8_t *)p_value;
    return (T_GAP_CAUSE)rpc_le_set_gap_param((RPC_T_GAP_LE_PARAM_TYPE)param, &value);
}
T_GAP_CAUSE le_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                                 T_GAP_REMOTE_ADDR_TYPE bd_type)
{
    return (T_GAP_CAUSE)rpc_le_modify_white_list((RPC_T_GAP_WHITE_LIST_OP)operation, bd_addr, (RPC_T_GAP_REMOTE_ADDR_TYPE)bd_type);
}
T_GAP_CAUSE le_gen_rand_addr(T_GAP_RAND_ADDR_TYPE rand_addr_type, uint8_t *random_bd)
{
    return (T_GAP_CAUSE)rpc_le_gen_rand_addr((RPC_T_GAP_RAND_ADDR_TYPE)rand_addr_type, random_bd);
}

T_GAP_CAUSE le_set_rand_addr(uint8_t *random_bd)
{
    return (T_GAP_CAUSE)rpc_le_set_rand_addr(random_bd);
}



T_GAP_CAUSE le_scan_set_param(T_LE_SCAN_PARAM_TYPE param, uint8_t len, void *p_value)
{
    binary_t value;
    value.dataLength = len;
    value.data = (uint8_t *)p_value;
    return (T_GAP_CAUSE)rpc_le_scan_set_param((RPC_T_LE_SCAN_PARAM_TYPE)param, &value);
}

T_GAP_CAUSE le_scan_get_param(T_LE_SCAN_PARAM_TYPE param, void *p_value)
{
    binary_t value;
    value.dataLength = 0;
    value.data = (uint8_t *)p_value;
    return (T_GAP_CAUSE)rpc_le_scan_get_param((RPC_T_LE_SCAN_PARAM_TYPE)param, &value);
}

T_GAP_CAUSE le_scan_start(void)
{
    return (T_GAP_CAUSE)rpc_le_scan_start();
}

T_GAP_CAUSE le_scan_stop(void)
{
    return (T_GAP_CAUSE)rpc_le_scan_stop();
}

// bool le_scan_info_filter(bool enable, uint8_t offset, uint8_t len, uint8_t *p_filter)
// {
   
// }