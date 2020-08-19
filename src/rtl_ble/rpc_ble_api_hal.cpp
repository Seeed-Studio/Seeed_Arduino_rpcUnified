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

//! @name rpc_gap_le
//@{
bool le_gap_init(uint8_t link_num)
{
    return rpc_le_gap_init(link_num);
}
void le_gap_msg_info_way(bool use_msg)
{
    return rpc_le_gap_msg_info_way(use_msg);
}
uint8_t le_get_max_link_num(void)
{
    return le_get_max_link_num();
}
extern P_FUN_LE_APP_CB _ble_gap_callback;
void le_register_app_cb(P_FUN_LE_APP_CB ble_gap_callback)
{
    _ble_gap_callback = ble_gap_callback;
}

T_GAP_CAUSE le_set_gap_param(T_GAP_LE_PARAM_TYPE param, uint8_t len, void *p_value)
{
    RPC_FUN_GAP_GET_PARAM(le_set_gap_param, T_GAP_LE_PARAM_TYPE);
}
T_GAP_CAUSE le_get_gap_param(T_GAP_LE_PARAM_TYPE param, void *p_value)
{
    RPC_FUN_GAP_GET_PARAM(le_get_gap_param, T_GAP_LE_PARAM_TYPE);
}
T_GAP_CAUSE le_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                                 T_GAP_REMOTE_ADDR_TYPE bd_type)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_modify_white_list((RPC_T_GAP_WHITE_LIST_OP)operation, bd_addr, (RPC_T_GAP_REMOTE_ADDR_TYPE)bd_type);
    return ret;
}
T_GAP_CAUSE le_gen_rand_addr(T_GAP_RAND_ADDR_TYPE rand_addr_type, uint8_t *random_bd)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_gen_rand_addr((RPC_T_GAP_RAND_ADDR_TYPE)rand_addr_type, random_bd);
    return ret;
}
T_GAP_CAUSE le_set_rand_addr(uint8_t *random_bd)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_set_rand_addr(random_bd);
    return ret;
}
T_GAP_CAUSE le_cfg_local_identity_address(uint8_t *addr, T_GAP_IDENT_ADDR_TYPE type)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_cfg_local_identity_address(addr, (RPC_T_GAP_IDENT_ADDR_TYPE)type);
    return ret;
}
T_GAP_CAUSE le_set_host_chann_classif(uint8_t *p_channel_map)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_set_host_chann_classif(*p_channel_map);
    return ret;
}
T_GAP_CAUSE le_write_default_data_len(uint16_t tx_octets, uint16_t tx_time)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_write_default_data_len(tx_octets, tx_time);
    return ret;
}
//@}

//! @name rpc_gap_config
//@{
void gap_config_cccd_not_check(T_GAP_CONFIG_GATT_CCCD_NOT_CHECK cccd_not_check_flag)
{
    return rpc_gap_config_cccd_not_check((RPC_T_GAP_CONFIG_GATT_CCCD_NOT_CHECK)cccd_not_check_flag);
}

void gap_config_ccc_bits_count(uint8_t gatt_server_ccc_bits_count,
                               uint8_t gatt_storage_ccc_bits_count)
{
    return rpc_gap_config_ccc_bits_count(gatt_server_ccc_bits_count, gatt_storage_ccc_bits_count);
}

void gap_config_max_attribute_table_count(uint8_t gatt_max_attribute_table_count)
{
    return rpc_gap_config_max_attribute_table_count(gatt_max_attribute_table_count);
}

void gap_config_max_mtu_size(uint16_t att_max_mtu_size)
{
    return rpc_gap_config_max_mtu_size(att_max_mtu_size);
}

void gap_config_bte_pool_size(uint8_t bte_pool_size)
{
    return rpc_gap_config_bte_pool_size(bte_pool_size);
}

void gap_config_bt_report_buf_num(uint8_t bt_report_buf_num)
{
    return rpc_gap_config_bt_report_buf_num(bt_report_buf_num);
}

void gap_config_le_key_storage_flag(uint16_t le_key_storage_flag)
{
    return rpc_gap_config_le_key_storage_flag(le_key_storage_flag);
}

void gap_config_max_le_paired_device(uint8_t max_le_paired_device)
{
    return rpc_gap_config_max_le_paired_device(max_le_paired_device);
}

void gap_config_max_le_link_num(uint8_t le_link_num)
{
    return rpc_gap_config_max_le_link_num(le_link_num);
}
//@}
