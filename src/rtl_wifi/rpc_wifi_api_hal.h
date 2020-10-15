#if !defined(_rpc_wifi_api__hal_h_)
#define _rpc_wifi_api__hal_h_

#include "Arduino.h"
#include "rtl_wifi_unified.h"
#include "./erpc_shim/erpc_shim_unified.h"
#include "./esp_lib/esp_lib_unified.h"
#include "rpc_unified_log.h"
#include "erpc_port.h"

#define IW_PASSPHRASE_MAX_SIZE 64
#define NDIS_802_11_LENGTH_SSID 32
#define A_SHA_DIGEST_LEN 20

typedef struct wlan_fast_reconnect
{
    unsigned char psk_essid[NDIS_802_11_LENGTH_SSID + 4];
    unsigned char psk_passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
    unsigned char wpa_global_PSK[A_SHA_DIGEST_LEN * 2];
    uint32_t channel;
    uint32_t security_type;
    uint32_t offer_ip;
} wlan_fast_reconnect_profile_t;

void system_event_callback_reg(system_event_cb_t system_event_cb);
int wifi_get_reconnect_data(wlan_fast_reconnect_profile_t *wifi_info);

#endif /* _rpc_wifi_api__hal_h_ */