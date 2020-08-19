#include <stdlib.h>
#include <string.h>
#include "rpc_ble_api_hal.h"
#include "rpc_ble_api.h"
#include "gap_adv.h"
#include "erpc_port.h"

//! @brief Function to free space allocated inside struct binary_t
static void free_binary_t_struct(binary_t *data);

// Free space allocated inside struct binary_t function implementation
static void free_binary_t_struct(binary_t *data)
{
    if (data->data)
    {
        erpc_free(data->data);
    }
}

extern P_FUN_LE_APP_CB _ble_gap_callback;

void le_register_app_cb(P_FUN_LE_APP_CB ble_gap_callback)
{
    _ble_gap_callback = ble_gap_callback;
}

//! @name rpc_gap
//@{
T_GAP_CAUSE gap_set_param(T_GAP_PARAM_TYPE param, uint8_t len, void *p_value)
{
    RPC_FUN_GAP_SET_PARAM(gap_set_param, T_GAP_PARAM_TYPE);
}

T_GAP_CAUSE gap_get_param(T_GAP_PARAM_TYPE param, void *p_value)
{
    RPC_FUN_GAP_GET_PARAM(gap_get_param, T_GAP_PARAM_TYPE);
}

T_GAP_CAUSE gap_set_pairable_mode(void)
{
    RPC_FUN_GAP_VOID(gap_set_pairable_mode);
}
//@}
//! @name rpc_gap_bone
//@{
T_GAP_CAUSE le_bond_set_param(T_LE_BOND_PARAM_TYPE param, uint8_t len, void *p_value)
{
    RPC_FUN_GAP_SET_PARAM(le_bond_set_param, T_LE_BOND_PARAM_TYPE);
}
T_GAP_CAUSE le_bond_pair(uint8_t conn_id)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_pair(conn_id);
    return ret;
}
T_GAP_CAUSE le_bond_get_display_key(uint8_t conn_id, uint32_t *p_key)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_get_display_key(conn_id, p_key);
    return ret;
}
T_GAP_CAUSE le_bond_passkey_input_confirm(uint8_t conn_id, uint32_t passcode,
                                          T_GAP_CFM_CAUSE cause)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_passkey_input_confirm(conn_id, passcode, (RPC_T_GAP_CFM_CAUSE)cause);
    return ret;
}
T_GAP_CAUSE le_bond_oob_input_confirm(uint8_t conn_id, T_GAP_CFM_CAUSE cause)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_oob_input_confirm(conn_id, (RPC_T_GAP_CFM_CAUSE)cause);
    return ret;
}
T_GAP_CAUSE le_bond_just_work_confirm(uint8_t conn_id, T_GAP_CFM_CAUSE cause)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_just_work_confirm(conn_id, (RPC_T_GAP_CFM_CAUSE)cause);
    return ret;
}
T_GAP_CAUSE le_bond_passkey_display_confirm(uint8_t conn_id, T_GAP_CFM_CAUSE cause)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_passkey_display_confirm(conn_id, (RPC_T_GAP_CFM_CAUSE)cause);
    return ret;
}
T_GAP_CAUSE le_bond_user_confirm(uint8_t conn_id, T_GAP_CFM_CAUSE cause)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_user_confirm(conn_id, (RPC_T_GAP_CFM_CAUSE)cause);
    return ret;
}
T_GAP_CAUSE le_bond_cfg_local_key_distribute(uint8_t init_dist, uint8_t rsp_dist)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_cfg_local_key_distribute(init_dist, rsp_dist);
    return ret;
}
void le_bond_clear_all_keys(void)
{
    return rpc_le_bond_clear_all_keys();
}
T_GAP_CAUSE le_bond_delete_by_idx(uint8_t idx)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_delete_by_idx(idx);
    return ret;
}
T_GAP_CAUSE le_bond_delete_by_bd(uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE bd_type)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_delete_by_bd(bd_addr, (RPC_T_GAP_REMOTE_ADDR_TYPE)bd_type);
    return ret;
}
T_GAP_CAUSE le_bond_get_sec_level(uint8_t conn_id, T_GAP_SEC_LEVEL *p_type)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_bond_get_sec_level(conn_id, (RPC_T_GAP_SEC_LEVEL *)p_type);
    return ret;
}
//@}
