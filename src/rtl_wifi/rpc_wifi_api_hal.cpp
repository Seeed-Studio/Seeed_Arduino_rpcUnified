#define TAG "WIFI API"
#include "rpc_wifi_api_utils.h"

#include <stdlib.h>
#include <string.h>
#include "erpc/erpc_shim_unified.h"
#include "erpc/erpc_port.h"
#include "esp/esp_lib_unified.h"
#include "rpc_wifi_api_hal.h"

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

    b_ssid.dataLength = ssid_len + 1;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len + 1;
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

    b_ssid.dataLength = ssid_len + 1;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len + 1;
    b_pasword.data = (uint8_t *)password;

    int ret = 0;
    ret = rpc_wifi_connect_bssid(&b_bssid, &b_ssid, &b_pasword, security_type, key_id, NULL);

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
    b_mac.dataLength = strlen((char *)mac) + 1;
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

    b_ssid.dataLength = ssid_len + 1;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len + 1;
    b_pasword.data = (uint8_t *)password;
    RPC_INFO("password: %s\n\r", password);
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

    b_ssid.dataLength = ssid_len + 1;
    b_ssid.data = (uint8_t *)ssid;

    b_pasword.dataLength = password_len + 1;
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
    b_channel_list.dataLength = length * sizeof(uint32_t);
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

int wifi_get_reconnect_data(wlan_fast_reconnect_profile_t *wifi_info)
{
    FUNC_ENTRY;
    if (wifi_info == NULL)
    {
        RPC_DEBUG("wifi_info null");
        FUNC_EXIT;
        return 0;
    }

    binary_t data;
    int ret = rpc_wifi_get_reconnect_data(&data);
    RPC_DEBUG("rpc_wifi_get_reconnect_data ret %d", ret);
    memcpy(wifi_info, data.data, sizeof(wlan_fast_reconnect_profile_t));
    if (data.data)
    {
        erpc_free(data.data);
    }
    FUNC_EXIT;
    return ret;
}

int32_t wifi_scan_start()
{
    RPC_FUN_RETURN_0(wifi_scan_start, int32_t);
}

bool wifi_is_scaning()
{
    RPC_FUN_RETURN_0(wifi_is_scaning, bool);
}

uint16_t wifi_scan_get_ap_num()
{
    RPC_FUN_RETURN_0(wifi_scan_get_ap_num, uint16_t);
}

int32_t wifi_scan_get_ap_records(uint16_t number, wifi_ap_record_t *_scanResult)
{
    FUNC_ENTRY;
    int32_t ret = RTW_ERROR;
    binary_t scanResult;
    if (number > 60 || _scanResult == NULL)
    {
        return RTW_ERROR;
    }
    RPC_DEBUG("1");
    ret = rpc_wifi_scan_get_ap_records(number, &scanResult);
    if (ret != RTW_SUCCESS)
    {
        RPC_DEBUG("3");
        return RTW_ERROR;
    }
    RPC_DEBUG("2");
    rtw_scan_result_t *_rtw_scanResult = (rtw_scan_result_t *)scanResult.data;
    for (uint16_t i = 0; i < number; i++)
    {
        wifi_ap_record_t *item_network = &_scanResult[i];
        rtw_scan_result_t *item_scanResult = &_rtw_scanResult[i];
        memcpy(item_network->bssid, item_scanResult->BSSID.octet, 6);
        memcpy(item_network->ssid, item_scanResult->SSID.val, sizeof(item_scanResult->SSID.val));
        item_network->rssi = item_scanResult->signal_strength;
        item_network->primary = item_scanResult->channel;

        switch (item_scanResult->security)
        {
        case RTW_SECURITY_OPEN:
            item_network->authmode = WIFI_AUTH_OPEN;
            break;
        case RTW_SECURITY_WEP_PSK:
        case RTW_SECURITY_WEP_SHARED:
            item_network->authmode = WIFI_AUTH_WEP;
            break;
        case RTW_SECURITY_WPA_TKIP_PSK:
        case RTW_SECURITY_WPA_AES_PSK:
            item_network->authmode = WIFI_AUTH_WPA_PSK;
            break;
        case RTW_SECURITY_WPA2_AES_PSK:
        case RTW_SECURITY_WPA2_TKIP_PSK:
        case RTW_SECURITY_WPA2_AES_CMAC:
            item_network->authmode = WIFI_AUTH_WPA2_PSK;
            break;
        case RTW_SECURITY_WPA_WPA2_MIXED:
            item_network->authmode = WIFI_AUTH_WPA_WPA2_PSK;
            break;
        default:
            item_network->authmode = WIFI_AUTH_MAX;
            break;
        }
        item_network->wps = item_scanResult->wps_type == RTW_WPS_TYPE_NONE ? 0 : 1;
    }
    if (scanResult.data)
    {
        erpc_free(scanResult.data);
    }
    FUNC_EXIT_RC(ret);
}

//@}

//! @name rpc_wifi_tcpip
//@{

void tcpip_adapter_init(void)
{
    RPC_FUN_RETURN_VOID_0(tcpip_adapter_init);
}

esp_err_t tcpip_adapter_eth_start(uint8_t *mac, tcpip_adapter_ip_info_t *ip_info)
{
    return ESP_OK;
}

esp_err_t tcpip_adapter_sta_start(uint8_t *mac, tcpip_adapter_ip_info_t *ip_info)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (mac == NULL || ip_info == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _mac;
    binary_t _ip_info;
    _mac.dataLength = strlen((char *)mac) + 1;
    _mac.data = mac;
    _ip_info.dataLength = sizeof(tcpip_adapter_ip_info_t);
    _ip_info.data = (uint8_t *)ip_info;
    ret = (esp_err_t)rpc_tcpip_adapter_sta_start(&_mac, &_ip_info);
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_ap_start(uint8_t *mac, tcpip_adapter_ip_info_t *ip_info)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (mac == NULL || ip_info == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _mac;
    binary_t _ip_info;
    _mac.dataLength = strlen((char *)mac) + 1;
    _mac.data = mac;
    _ip_info.dataLength = sizeof(tcpip_adapter_ip_info_t);
    _ip_info.data = (uint8_t *)ip_info;
    ret = (esp_err_t)rpc_tcpip_adapter_ap_start(&_mac, &_ip_info);
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_stop(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_stop, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_up(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_up, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_down(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_down, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_get_ip_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_ip_info_t *ip_info)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (ip_info == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _ip_info;
    ret = (esp_err_t)rpc_tcpip_adapter_get_ip_info((uint32_t)tcpip_if, &_ip_info);
    if (ret == ESP_OK)
    {
        memcpy(ip_info, _ip_info.data, sizeof(tcpip_adapter_ip_info_t));
    }
    tcpip_adapter_ip_info_t *temp = (tcpip_adapter_ip_info_t *)_ip_info.data;
    // RPC_DEBUG("tcpip_if:%d ip_addr:%d netmask:%d, gw:%d", tcpip_if, ip_info->ip, ip_info->netmask, ip_info->gw);
    // RPC_DEBUG("tcpip_if:%d ip_addr:%d netmask:%d, gw:%d", tcpip_if, temp->ip, temp->netmask, temp->gw);
    if (_ip_info.data != NULL)
    {
        erpc_free(_ip_info.data);
        _ip_info.data = NULL;
    }
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_set_ip_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_ip_info_t *ip_info)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (ip_info == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _ip_info;
    _ip_info.dataLength = sizeof(tcpip_adapter_ip_info_t);
    _ip_info.data = (uint8_t *)ip_info;
    ret = (esp_err_t)rpc_tcpip_adapter_set_ip_info((uint32_t)tcpip_if, &_ip_info);
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_set_dns_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_dns_type_t type, tcpip_adapter_dns_info_t *dns)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (dns == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _dns;
    _dns.dataLength = sizeof(tcpip_adapter_dns_info_t);
    _dns.data = (uint8_t *)dns;
    ret = (esp_err_t)rpc_tcpip_adapter_set_dns_info((uint32_t)tcpip_if, (uint32_t)type, &_dns);
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_get_dns_info(tcpip_adapter_if_t tcpip_if, tcpip_adapter_dns_type_t type, tcpip_adapter_dns_info_t *dns)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (dns == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _dns;
    ret = (esp_err_t)rpc_tcpip_adapter_get_dns_info((uint32_t)tcpip_if, (uint32_t)type, &_dns);
    if (ret == ESP_OK)
    {
        memcpy(dns, _dns.data, sizeof(tcpip_adapter_dns_info_t));
    }
    if (_dns.data != NULL)
    {
        erpc_free(_dns.data);
        _dns.data = NULL;
    }
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_get_mac(tcpip_adapter_if_t tcpip_if, uint8_t *mac)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (mac == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _mac;
    ret = (esp_err_t)rpc_tcpip_adapter_get_mac((uint32_t)tcpip_if, &_mac);
    if (ret == ESP_OK)
    {
        memcpy(mac, _mac.data, _mac.dataLength);
    }
    if (_mac.data != NULL)
    {
        erpc_free(_mac.data);
        _mac.data = NULL;
    }
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_set_mac(tcpip_adapter_if_t tcpip_if, uint8_t *mac)
{
    FUNC_ENTRY;
    esp_err_t ret = ESP_OK;
    if (mac == NULL)
    {
        return ESP_ERR_TCPIP_ADAPTER_INVALID_PARAMS;
    }
    binary_t _mac;
    _mac.dataLength = strlen((char *)mac) + 1;
    _mac.data = (uint8_t *)mac;
    ret = (esp_err_t)rpc_tcpip_adapter_set_mac((uint32_t)tcpip_if, &_mac);
    FUNC_EXIT;
    return ret;
}

esp_err_t tcpip_adapter_dhcps_start(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_dhcps_start, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_dhcps_stop(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_dhcps_stop, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_dhcpc_start(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_dhcpc_start, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_dhcpc_stop(tcpip_adapter_if_t tcpip_if)
{
    RPC_FUN_RETURN_1(tcpip_adapter_dhcpc_stop, (uint32_t)tcpip_if, esp_err_t);
}

esp_err_t tcpip_adapter_set_hostname(tcpip_adapter_if_t tcpip_if, const char *hostname)
{
    FUNC_ENTRY;
    FUNC_EXIT;
    return ESP_OK;
}

esp_err_t tcpip_adapter_get_hostname(tcpip_adapter_if_t tcpip_if, const char **hostname)
{
    FUNC_ENTRY;
    FUNC_EXIT;
    static char host[] = "Wio Terminal";
    *hostname = host;
    return ESP_OK;
}

esp_err_t tcpip_adapter_dhcps_option(tcpip_adapter_option_mode_t opt_op, tcpip_adapter_option_id_t opt_id,
                                     void *opt_val, uint32_t opt_len)

{
    FUNC_ENTRY;
    FUNC_EXIT;
    return ESP_OK;
}

esp_err_t tcpip_adapter_create_ip6_linklocal(tcpip_adapter_if_t tcpip_if)
{
    FUNC_ENTRY;
    FUNC_EXIT;
    return ESP_FAIL;
}

esp_err_t tcpip_adapter_get_ip6_linklocal(tcpip_adapter_if_t tcpip_if, ip6_addr_t *if_ip6)
{
    FUNC_ENTRY;
    FUNC_EXIT;
    return ESP_FAIL;
}

//@}

//! @name rpc_wifi_callback
//@{
extern system_event_cb_t ptr_wifi_event_callback;
void system_event_callback_reg(system_event_cb_t system_event_cb)
{
    FUNC_ENTRY;
    ptr_wifi_event_callback = system_event_cb;
    FUNC_EXIT;
}
//@}