#include <stdlib.h>
#include <string.h>
#include "rpc_ble_api_hal.h"
#include "rpc_ble_api.h"
#include "gap_adv.h"
#include "profile_client.h"
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
extern P_FUN_HABDLE_GAP_MSG _handle_gap_msg;
void le_register_msg_handler(P_FUN_HABDLE_GAP_MSG handle_gap_msg)
{
    _handle_gap_msg = handle_gap_msg;
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

//! @name rpc_gap_adv
//@{
T_GAP_CAUSE le_adv_set_param(T_LE_ADV_PARAM_TYPE param, uint8_t len, void *p_value)
{
    RPC_FUN_GAP_SET_PARAM(le_adv_set_param, T_LE_ADV_PARAM_TYPE);
}

T_GAP_CAUSE le_adv_get_param(T_LE_ADV_PARAM_TYPE param, void *p_value)
{
    RPC_FUN_GAP_GET_PARAM(le_adv_get_param, T_LE_ADV_PARAM_TYPE);
}

T_GAP_CAUSE le_adv_start(void)
{
    RPC_FUN_GAP_VOID(le_adv_start);
}

T_GAP_CAUSE le_adv_stop(void)
{
    RPC_FUN_GAP_VOID(le_adv_stop);
}

T_GAP_CAUSE le_adv_update_param(void)
{
    RPC_FUN_GAP_VOID(le_adv_stop);
}
//@}

//! @name rpc_gap_scan
//@{
T_GAP_CAUSE le_scan_set_param(T_LE_SCAN_PARAM_TYPE param, uint8_t len, void *p_value)
{
    RPC_FUN_GAP_SET_PARAM(le_scan_set_param, T_LE_SCAN_PARAM_TYPE);
}

T_GAP_CAUSE le_scan_get_param(T_LE_SCAN_PARAM_TYPE param, void *p_value)
{
    RPC_FUN_GAP_GET_PARAM(le_scan_get_param, T_LE_SCAN_PARAM_TYPE);
}

T_GAP_CAUSE le_scan_start(void)
{
    RPC_FUN_GAP_VOID(le_scan_start);
}

T_GAP_CAUSE le_scan_stop(void)
{
    RPC_FUN_GAP_VOID(le_scan_stop);
}

bool le_scan_info_filter(bool enable, uint8_t offset, uint8_t len, uint8_t *p_filter)
{
    return rpc_le_scan_info_filter(enable, offset, len, p_filter);
}
//@}

//! @name rpc_gap_conn
//@{

T_GAP_CAUSE le_get_conn_param(T_LE_CONN_PARAM_TYPE param, void *p_value, uint8_t conn_id)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    binary_t value;
    ret = (T_GAP_CAUSE)rpc_le_get_conn_param((RPC_T_LE_CONN_PARAM_TYPE)param, &value, conn_id);
    if (GAP_SUCCESS == ret)
        memcpy(p_value, value.data, value.dataLength);
    erpc_free(value.data);
    return ret;
}

bool le_get_conn_info(uint8_t conn_id, T_GAP_CONN_INFO *p_conn_info)
{
    return rpc_le_get_conn_info(conn_id, (RPC_T_GAP_CONN_INFO *)p_conn_info);
}

bool le_get_conn_addr(uint8_t conn_id, uint8_t *bd_addr, uint8_t *bd_type)
{
    return rpc_le_get_conn_addr(conn_id, bd_addr, bd_type);
}

bool le_get_conn_id(uint8_t *bd_addr, uint8_t bd_type, uint8_t *p_conn_id)
{
    return rpc_le_get_conn_id(bd_addr, bd_type, p_conn_id);
}

uint8_t le_get_active_link_num(void)
{
    return rpc_le_get_active_link_num();
}

uint8_t le_get_idle_link_num(void)
{
    return rpc_le_get_idle_link_num();
}

T_GAP_CAUSE le_disconnect(uint8_t conn_id)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_disconnect(conn_id);
    return ret;
}

T_GAP_CAUSE le_read_rssi(uint8_t conn_id)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_read_rssi(conn_id);
    return ret;
}

T_GAP_CAUSE le_set_data_len(uint8_t conn_id, uint16_t tx_octets, uint16_t tx_time)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_set_data_len(conn_id, tx_octets, tx_time);
    return ret;
}

T_GAP_CAUSE le_set_phy(uint8_t conn_id, uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys,
                       T_GAP_PHYS_OPTIONS phy_options)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_set_phy(conn_id, all_phys, tx_phys, rx_phys, (RPC_T_GAP_PHYS_OPTIONS)phy_options);
    return ret;
}

T_GAP_CAUSE le_set_conn_param(T_GAP_CONN_PARAM_TYPE type,
                              T_GAP_LE_CONN_REQ_PARAM *p_conn_param)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_set_conn_param((RPC_T_GAP_CONN_PARAM_TYPE)type,
                                             (RPC_T_GAP_LE_CONN_REQ_PARAM *)p_conn_param);
    return ret;
}

T_GAP_CAUSE le_connect(uint8_t init_phys, uint8_t *remote_bd,
                       T_GAP_REMOTE_ADDR_TYPE remote_bd_type,
                       T_GAP_LOCAL_ADDR_TYPE local_bd_type, uint16_t scan_timeout)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_connect(init_phys, remote_bd,
                                      (RPC_T_GAP_REMOTE_ADDR_TYPE)remote_bd_type,
                                      (RPC_T_GAP_LOCAL_ADDR_TYPE)local_bd_type, scan_timeout);
    return ret;
}

T_GAP_CAUSE le_update_conn_param(uint8_t conn_id,
                                 uint16_t conn_interval_min,
                                 uint16_t conn_interval_max,
                                 uint16_t conn_latency,
                                 uint16_t supervision_timeout,
                                 uint16_t ce_length_min,
                                 uint16_t ce_length_max)
{
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;
    ret = (T_GAP_CAUSE)rpc_le_update_conn_param(conn_id,
                                                conn_interval_min,
                                                conn_interval_max,
                                                conn_latency,
                                                supervision_timeout,
                                                ce_length_min,
                                                ce_length_max);
    return ret;
}
//@}

//! @name rpc_gatt_client
//@{
    
extern P_FUN_GENERAL_APP_CB _ble_gattc_callback;
void le_register_gattc_cb(P_FUN_GENERAL_APP_CB ble_gattc_callback)
{
    _ble_gattc_callback = ble_gattc_callback;
}

bool ble_client_init(uint8_t num)
{
    return rpc_ble_client_init(num);
}

uint8_t ble_add_client(uint8_t app_id, uint8_t link_num)
{
    return rpc_ble_add_client(app_id, link_num);
}

T_GAP_CAUSE client_all_primary_srv_discovery(uint8_t conn_id, T_CLIENT_ID client_id)
{
    RPC_FUN_RETURN_CAUSE_2(client_all_primary_srv_discovery, conn_id, client_id);
}

T_GAP_CAUSE client_by_uuid_srv_discovery(uint8_t conn_id, T_CLIENT_ID client_id, uint16_t uuid16)
{
    RPC_FUN_RETURN_CAUSE_3(client_by_uuid_srv_discovery, conn_id, client_id, uuid16);
}

T_GAP_CAUSE client_by_uuid128_srv_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                            uint8_t *p_uuid128)
{
    RPC_FUN_RETURN_CAUSE_3(client_by_uuid128_srv_discovery, conn_id, client_id, p_uuid128);
}
T_GAP_CAUSE client_relationship_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                          uint16_t start_handle, uint16_t end_handle)
{
    RPC_FUN_RETURN_CAUSE_4(client_relationship_discovery, conn_id, client_id, start_handle, end_handle);
}
T_GAP_CAUSE client_all_char_discovery(uint8_t conn_id, T_CLIENT_ID client_id, uint16_t start_handle,
                                      uint16_t end_handle)
{
    RPC_FUN_RETURN_CAUSE_4(client_all_char_discovery, conn_id, client_id, start_handle, end_handle);
}
T_GAP_CAUSE client_by_uuid_char_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                          uint16_t start_handle,
                                          uint16_t end_handle, uint16_t uuid16)
{
    RPC_FUN_RETURN_CAUSE_5(client_by_uuid_char_discovery, conn_id, client_id, start_handle, end_handle, uuid16);
}
T_GAP_CAUSE client_by_uuid128_char_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                             uint16_t start_handle,
                                             uint16_t end_handle, uint8_t *p_uuid128)
{
    RPC_FUN_RETURN_CAUSE_5(client_by_uuid128_char_discovery, conn_id, client_id, start_handle, end_handle, p_uuid128);
}
T_GAP_CAUSE client_all_char_descriptor_discovery(uint8_t conn_id, T_CLIENT_ID client_id,
                                                 uint16_t start_handle, uint16_t end_handle)
{
    RPC_FUN_RETURN_CAUSE_4(client_all_char_descriptor_discovery, conn_id, client_id, start_handle, end_handle);
}

T_GAP_CAUSE client_attr_read(uint8_t conn_id, T_CLIENT_ID client_id, uint16_t handle)
{
    RPC_FUN_RETURN_CAUSE_3(client_attr_read, conn_id, client_id, handle);
}

T_GAP_CAUSE client_attr_read_using_uuid(uint8_t conn_id, T_CLIENT_ID client_id,
                                        uint16_t start_handle,
                                        uint16_t end_handle, uint16_t uuid16, uint8_t *p_uuid128)
{
    RPC_FUN_RETURN_CAUSE_6(client_attr_read_using_uuid, conn_id, client_id, start_handle, end_handle, uuid16, p_uuid128);
}
T_GAP_CAUSE client_attr_write(uint8_t conn_id, T_CLIENT_ID client_id,
                              T_GATT_WRITE_TYPE write_type,
                              uint16_t handle, uint16_t length, uint8_t *p_data)
{
    binary_t data;
    data.dataLength = length;
    data.data = p_data;
    RPC_FUN_RETURN_CAUSE_5(client_attr_write, conn_id, client_id, (RPC_T_GATT_WRITE_TYPE)write_type, handle, &data);
}
T_GAP_CAUSE client_attr_ind_confirm(uint8_t conn_id)
{
    RPC_FUN_RETURN_CAUSE_1(client_attr_ind_confirm, conn_id);
}
//@}
