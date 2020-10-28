#include "seeed_rpcUnified.h"
#include "rtl_wifi/wifi_unified.h"

const char *ssid = "TE_TEST";
const char *password = "123456789";

esp_err_t wifi_event_callback(void *ctx, system_event_t *event)
{
    Serial.printf("[WiFi-event] event: %d\n\r", event->event_id);
    
    switch (event->event_id)
    {
    case SYSTEM_EVENT_WIFI_READY:
        Serial.printf("WiFi interface ready\n\r");
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        Serial.printf("Completed scan for access points\n\r");
        break;
    case SYSTEM_EVENT_STA_START:
        Serial.printf("WiFi client started\n\r");
        break;
    case SYSTEM_EVENT_STA_STOP:
        Serial.printf("WiFi clients stopped\n\r");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        Serial.printf("Connected to access point\n\r");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.printf("Disconnected from WiFi access point\n\r");
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        Serial.printf("Authentication mode of access point has changed\n\r");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.printf("Obtained IP address: \n\r");
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        Serial.printf("Lost IP address and IP address is reset to 0\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        Serial.printf("WiFi Protected Setup (WPS): succeeded in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        Serial.printf("WiFi Protected Setup (WPS): failed in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        Serial.printf("WiFi Protected Setup (WPS): timeout in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        Serial.printf("WiFi Protected Setup (WPS): pin code in enrollee mode\n\r");
        break;
    case SYSTEM_EVENT_AP_START:
        Serial.printf("WiFi access point started\n\r");
        break;
    case SYSTEM_EVENT_AP_STOP:
        Serial.printf("WiFi access point  stopped\n\r");
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        Serial.printf("Client connected\n\r");
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        Serial.printf("Client disconnected\n\r");
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        Serial.printf("Assigned IP address to client\n\r");
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        Serial.printf("Received probe request\n\r");
        break;
    case SYSTEM_EVENT_GOT_IP6:
        Serial.printf("IPv6 is preferred\n\r");
        break;
    case SYSTEM_EVENT_ETH_START:
        Serial.printf("Ethernet started\n\r");
        break;
    case SYSTEM_EVENT_ETH_STOP:
        Serial.printf("Ethernet stopped\n\r");
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        Serial.printf("Ethernet connected\n\r");
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        Serial.printf("Ethernet disconnected\n\r");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        Serial.printf("Obtained IP address\n\r");
        break;
    default:
        break;
    }

    return ESP_OK;
}
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    };
    // String mac("01:02:03:04:05:06");
    // uint8_t mac_s[6];
    // Serial.println(mac);
    // int index = 0;
    // for(int i = 0; i < 6; i ++)
    // {
    //     mac_s[i] = (uint8_t)mac.substring(index, index+2).toInt();
    //     Serial.println(mac.substring(index, index+2));
    //     index+=3;
    // }
    // for(int i = 0; i < 6; i++)
    // {
    //     Serial.println(mac_s[i]);
    // }
    system_event_callback_reg(wifi_event_callback);
    tcpip_adapter_init();
    wifi_off();
    wifi_on(RTW_MODE_AP);
    wifi_start_ap((char *)ssid, RTW_SECURITY_WPA2_AES_PSK, (char *)password, strlen(ssid), strlen(password), 11);
    tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
    tcpip_adapter_ip_info_t info;
    info.ip.addr = 16885952;
    info.gw.addr = 16885952;
    info.netmask.addr = 16777215;
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info);
    tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);
}

void loop()
{
}
