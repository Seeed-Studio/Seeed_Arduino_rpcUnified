#define TAG "WIFI API"
#include "rpc_wifi_api_utils.h"

#include <stdlib.h>
#include <string.h>
#include "../erpc_shim/erpc_shim_unified.h"
#include "rpc_wifi_api_hal.h"
#include "erpc_port.h"

//! @name rpc_wifi_drv
//@{
// int wifi_manager_init(void)
// {
//     RPC_FUN_RETURN_0(wifi_manager_init, int);
// }

int wifi_connect(
    char *ssid,
    rtw_security_t security_type,
    char *password,
    int ssid_len,
    int password_len,
    int key_id,
    void *semaphore)
{
    FUNC_ENTRY;

    binary_t b_ssid;
    binary_t b_pasword;

    b_ssid.dataLength = ssid_len;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len;
    b_pasword.data = (uint8_t *)password;

    int ret = 0;
    ret = rpc_wifi_connect(&b_ssid, &b_pasword, security_type, key_id, NULL);

    FUNC_EXIT;
    return ret;
}

int wifi_connect_bssid(
    unsigned char bssid[ETH_ALEN],
    char *ssid,
    rtw_security_t security_type,
    char *password,
    int bssid_len,
    int ssid_len,
    int password_len,
    int key_id,
    void *semaphore)
{
    FUNC_ENTRY
    binary_t b_bssid;
    binary_t b_ssid;
    binary_t b_pasword;

    b_bssid.dataLength = ETH_ALEN;
    b_bssid.data = bssid;

    b_ssid.dataLength = ssid_len;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len;
    b_pasword.data = (uint8_t *)password;

    int ret = 0;
    ret = rpc_wifi_connect_bssid(&b_ssid, &b_ssid, &b_pasword, security_type, key_id, NULL);

    FUNC_EXIT
    return ret;
}

int wifi_disconnect(void)
{
    RPC_FUN_RETURN_0(wifi_disconnect, int);
}

int wifi_is_connected_to_ap(void)
{
    RPC_FUN_RETURN_0(wifi_is_connected_to_ap, int);
}

int wifi_is_up(rtw_interface_t interface)
{
    RPC_FUN_RETURN_1(wifi_is_up, uint32_t(interface), int);
}

int wifi_is_ready_to_transceive(rtw_interface_t interface)
{
    RPC_FUN_RETURN_1(wifi_is_ready_to_transceive, uint32_t(interface), int);
}

int wifi_set_mac_address(char *mac)
{
    binary_t b_mac;
    b_mac.data = (uint8_t *)mac;
    b_mac.dataLength = strlen(mac) + 1;
    RPC_FUN_RETURN_1(wifi_set_mac_address, &b_mac, int);
}

int wifi_get_mac_address(char *mac)
{
    FUNC_ENTRY;
    uint8_t buf[18] = {};
    int ret = 0;
    ret = rpc_wifi_get_mac_address(buf);
    strcpy(mac, (char *)buf);
    FUNC_EXIT;
    return ret;
}

int wifi_enable_powersave(void)
{
    RPC_FUN_RETURN_0(wifi_enable_powersave, int);
}

int wifi_resume_powersave(void)
{
    RPC_FUN_RETURN_0(wifi_resume_powersave, int);
}

int wifi_disable_powersave(void)
{
    RPC_FUN_RETURN_0(wifi_disable_powersave, int);
}

void wifi_btcoex_set_bt_on(void)
{
    RPC_FUN_RETURN_VOID_0(wifi_btcoex_set_bt_on);
}

void wifi_btcoex_set_bt_off(void)
{
    RPC_FUN_RETURN_VOID_0(wifi_btcoex_set_bt_on);
}

#if 0 //Not ready
int wifi_get_txpower(int *poweridx)
{
    RPC_FUN_RETURN_1(wifi_get_txpower, poweridx, int);
}

int wifi_set_txpower(int poweridx)
{
    RPC_FUN_RETURN_1(wifi_set_txpower, poweridx, int);
}
#endif

int wifi_get_associated_client_list(void *client_list_buffer, unsigned short buffer_length)
{
    FUNC_ENTRY;
    int ret = 0;
    binary_t b_client_list_buffer;
    ret = rpc_wifi_get_associated_client_list(&b_client_list_buffer, buffer_length);
    if (RTW_SUCCESS == ret)
    {
        memcpy(client_list_buffer, b_client_list_buffer.data, buffer_length);
    }
    if (b_client_list_buffer.data != NULL)
    {
        erpc_free(b_client_list_buffer.data);
        b_client_list_buffer.data = NULL;
    }
    FUNC_EXIT;
    return ret;
}

int wifi_get_ap_bssid(unsigned char *bssid)
{
    RPC_FUN_RETURN_1(wifi_get_ap_bssid, bssid, int);
}

int wifi_get_ap_info(rtw_bss_info_t *ap_info, rtw_security_t *security)
{
    FUNC_ENTRY;
    int ret = 0;
    binary_t b_ap_info;
    ret = rpc_wifi_get_ap_info(&b_ap_info, (uint32_t *)security);
    if (RTW_SUCCESS == ret)
    {
        memcpy(ap_info, b_ap_info.data, b_ap_info.dataLength);
    }
    if (b_ap_info.data != NULL)
    {
        erpc_free(b_ap_info.data);
        b_ap_info.data = NULL;
    }
    FUNC_EXIT;
    return ret;
}

int wifi_set_country(rtw_country_code_t country_code)
{
    RPC_FUN_RETURN_1(wifi_set_country, (uint32_t)country_code, int);
}

int wifi_get_sta_max_data_rate(__u8 *inidata_rate)
{
    RPC_FUN_RETURN_1(wifi_get_sta_max_data_rate, inidata_rate, int);
}

int wifi_get_rssi(int *pRSSI)
{
    RPC_FUN_RETURN_1(wifi_get_rssi, (int32_t *)pRSSI, int);
}

int wifi_set_channel(int channel)
{
    RPC_FUN_RETURN_1(wifi_set_channel, channel, int);
}

int wifi_get_channel(int *channel)
{
    RPC_FUN_RETURN_1(wifi_get_channel, (int32_t *)channel, int);
}

int wifi_change_channel_plan(uint8_t channel_plan)
{
    RPC_FUN_RETURN_1(wifi_change_channel_plan, channel_plan, int);
}

int wifi_register_multicast_address(rtw_mac_t *mac)
{
    RPC_FUN_RETURN_1(wifi_register_multicast_address, (uint8_t *)mac->octet, int);
}

int wifi_unregister_multicast_address(rtw_mac_t *mac)
{
    RPC_FUN_RETURN_1(wifi_unregister_multicast_address, (uint8_t *)mac->octet, int);
}

int wifi_rf_on(void)
{
    RPC_FUN_RETURN_0(wifi_rf_on, int);
}

int wifi_rf_off(void)
{
    RPC_FUN_RETURN_0(wifi_rf_off, int);
}

int wifi_on(rtw_mode_t mode)
{
    RPC_FUN_RETURN_1(wifi_on, (uint32_t)mode, int);
}

int wifi_off(void)
{
    RPC_FUN_RETURN_0(wifi_off, int);
}

int wifi_set_mode(rtw_mode_t mode)
{
    RPC_FUN_RETURN_1(wifi_set_mode, (uint32_t)mode, int);
}

int wifi_off_fastly(void)
{
    RPC_FUN_RETURN_0(wifi_off_fastly, int);
}

int wifi_set_power_mode(unsigned char ips_mode, unsigned char lps_mode)
{
    RPC_FUN_RETURN_2(wifi_set_power_mode, (uint8_t)ips_mode, (uint8_t)lps_mode, int);
}

int wifi_set_tdma_param(unsigned char slot_period, unsigned char rfon_period_len_1, unsigned char rfon_period_len_2, unsigned char rfon_period_len_3)
{
    RPC_FUN_RETURN_4(wifi_set_tdma_param, (uint8_t)slot_period, (uint8_t)rfon_period_len_1, (uint8_t)rfon_period_len_2, (uint8_t)rfon_period_len_3, int);
}

int wifi_set_lps_dtim(unsigned char dtim)
{
    RPC_FUN_RETURN_1(wifi_set_lps_dtim, (uint8_t)dtim, int);
}

int wifi_get_lps_dtim(unsigned char *dtim)
{
    RPC_FUN_RETURN_1(wifi_get_lps_dtim, (uint8_t *)dtim, int);
}

int wifi_set_lps_thresh(rtw_lps_thresh_t mode)
{
    RPC_FUN_RETURN_1(wifi_set_lps_thresh, (uint8_t)mode, int);
}

int wifi_set_lps_level(unsigned char lps_level)
{
    RPC_FUN_RETURN_1(wifi_set_lps_level, (uint8_t)lps_level, int);
}

int wifi_set_mfp_support(unsigned char value)
{
    RPC_FUN_RETURN_1(wifi_set_mfp_support, (uint8_t)value, int);
}

int wifi_start_ap(
    char *ssid,
    rtw_security_t security_type,
    char *password,
    int ssid_len,
    int password_len,
    int channel)
{
    FUNC_ENTRY;

    binary_t b_ssid;
    binary_t b_pasword;

    b_ssid.dataLength = ssid_len;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len;
    b_pasword.data = (uint8_t *)password;

    int ret = 0;
    ret = rpc_wifi_start_ap(&b_ssid, &b_pasword, security_type, channel);
    FUNC_EXIT;
    return ret;
}

int wifi_start_ap_with_hidden_ssid(
    char *ssid,
    rtw_security_t security_type,
    char *password,
    int ssid_len,
    int password_len,
    int channel)
{
    FUNC_ENTRY;

    binary_t b_ssid;
    binary_t b_pasword;

    b_ssid.dataLength = ssid_len;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len;
    b_pasword.data = (uint8_t *)password;

    int ret = 0;
    ret = rpc_wifi_start_ap_with_hidden_ssid(&b_ssid, &b_pasword, security_type, channel);
    FUNC_EXIT;
    return ret;
}

int wifi_set_pscan_chan(__u8 *channel_list, __u8 *pscan_config, __u8 length)
{
    FUNC_ENTRY;
    binary_t b_channel_list;
    b_channel_list.data = (uint8_t *)channel_list;
    b_channel_list.dataLength = length;
    int ret = 0;
    ret = rpc_wifi_set_pscan_chan(&b_channel_list, (uint8_t)*pscan_config);
    FUNC_EXIT;
    return ret;
}

int wifi_get_setting(const char *ifname, rtw_wifi_setting_t *pSetting)
{
    FUNC_ENTRY;
    int ret = 0;
    binary_t b_pSetting;
    ret = rpc_wifi_get_setting(ifname, &b_pSetting);
    if (ret = RTW_SUCCESS)
    {
        memcpy(pSetting, b_pSetting.data, sizeof(rtw_wifi_setting_t));
    }
    if (b_pSetting.data != NULL)
    {
        erpc_free(b_pSetting.data);
        b_pSetting.data = NULL;
    }
    FUNC_EXIT;
    return ret;
}

int wifi_set_network_mode(rtw_network_mode_t mode)
{
    RPC_FUN_RETURN_1(wifi_set_network_mode, (uint32_t)mode, int);
}

int wifi_get_network_mode(rtw_network_mode_t *pmode)
{
    RPC_FUN_RETURN_1(wifi_get_network_mode, (uint32_t *)pmode, int);
}

int wifi_set_wps_phase(unsigned char is_trigger_wps)
{
    RPC_FUN_RETURN_1(wifi_set_wps_phase, (uint8_t)is_trigger_wps, int);
}

int wifi_restart_ap(
    unsigned char *ssid,
    rtw_security_t security_type,
    unsigned char *password,
    int ssid_len,
    int password_len,
    int channel)
{
    FUNC_ENTRY;

    binary_t b_ssid;
    binary_t b_pasword;

    b_ssid.dataLength = ssid_len;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len;
    b_pasword.data = (uint8_t *)password;

    int ret = 0;
    ret = rpc_wifi_restart_ap(&b_ssid, &b_pasword, security_type, channel);
    FUNC_EXIT;
    return ret;
}

int wifi_config_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout)
{
    RPC_FUN_RETURN_3(wifi_config_autoreconnect, (uint8_t)mode, (uint8_t)retry_times, (uint16_t)timeout, int);
}

int wifi_set_autoreconnect(__u8 mode)
{
    RPC_FUN_RETURN_1(wifi_set_autoreconnect, (uint8_t)mode, int);
}

int wifi_get_autoreconnect(__u8 *mode)
{
    RPC_FUN_RETURN_1(wifi_get_autoreconnect, (uint8_t *)mode, int);
}

int wifi_get_last_error(void)
{
    RPC_FUN_RETURN_0(wifi_get_last_error, int);
}

int wifi_add_custom_ie(void *cus_ie, int ie_num);

// int wifi_update_custom_ie(void *cus_ie, int ie_index)
// {

// }

int wifi_del_custom_ie(void)
{
    RPC_FUN_RETURN_0(wifi_del_custom_ie, int);
}

int wifi_get_drv_ability(uint32_t *ability)
{
    RPC_FUN_RETURN_1(wifi_get_drv_ability, ability, int);
}

int wifi_set_channel_plan(uint8_t channel_plan)
{
    RPC_FUN_RETURN_1(wifi_set_channel_plan, channel_plan, int);
}

int wifi_get_channel_plan(uint8_t *channel_plan)
{
    RPC_FUN_RETURN_1(wifi_get_channel_plan, channel_plan, int);
}

int wifi_enable_forwarding(void)
{
    RPC_FUN_RETURN_0(wifi_enable_forwarding, int);
}

int wifi_disable_forwarding(void)
{
    RPC_FUN_RETURN_0(wifi_disable_forwarding, int);
}

int wifi_set_ch_deauth(__u8 enable)
{
    RPC_FUN_RETURN_1(wifi_set_ch_deauth, (uint8_t)enable, int);
}

WL_BAND_TYPE wifi_get_band_type(void)
{
    RPC_FUN_RETURN_0(wifi_get_band_type, WL_BAND_TYPE);
}

int wifi_set_tx_pause_data(unsigned int NewState)
{
    RPC_FUN_RETURN_1(wifi_set_tx_pause_data, (uint32_t)NewState, int);
}
//@}
