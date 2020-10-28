#include "seeed_rpcUnified.h"
#include "rtl_ble/ble_unified.h"

T_APP_RESULT ble_gap_callback(uint8_t cb_type, void *p_cb_data)
{
  T_APP_RESULT result = APP_RESULT_SUCCESS;
  T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;
  switch (cb_type)
  {
  case GAP_MSG_LE_SCAN_INFO:
  {
    Serial.printf("GAP_MSG_LE_SCAN_INFO:adv_type 0x%x\n\r", p_data->p_le_scan_info->adv_type);
    Serial.printf("bd_addr %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                  (p_data->p_le_scan_info->bd_addr)[5],
                  (p_data->p_le_scan_info->bd_addr)[4],
                  (p_data->p_le_scan_info->bd_addr)[3],
                  (p_data->p_le_scan_info->bd_addr)[2],
                  (p_data->p_le_scan_info->bd_addr)[1],
                  (p_data->p_le_scan_info->bd_addr)[0]);
    Serial.printf("remote_addr_type %d, rssi %d, data_len %d\n\r",
                  p_data->p_le_scan_info->remote_addr_type,
                  p_data->p_le_scan_info->rssi,
                  p_data->p_le_scan_info->data_len);
    break;
  }
  case GAP_MSG_LE_SCAN_CMPL:
  {
    Serial.printf("GAP_MSG_LE_SCAN_CMPL:\n\r");
    break;
  }
  }
  return result;
}
extern void ble_handle_gap_msg(T_IO_MSG *p_gap_msg);
extern T_APP_RESULT ble_gatt_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

uint8_t conn_id = 0xff;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
  {
  };
  delay(2000);
  Serial.printf("ble_init\n\r");
  ble_init();

  le_register_app_cb(ble_gap_callback);
  le_register_msg_handler(ble_handle_gap_msg);
  le_register_gattc_cb(ble_gatt_client_callback);
  ble_client_init(BLE_CLIENT_MAX_APPS);
  uint16_t _scanInterval = 0x520;
  le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(_scanInterval), &_scanInterval);
  Serial.printf("ble_start\n\r");
  ble_start();
  delay(1000);

  T_CLIENT_ID client_id = ble_add_client(0, BLE_LE_MAX_LINKS);

  delay(1000);
  Serial.printf("rpc_le_scan_start\n\r");
  le_scan_timer_start(10000);
  delay(4000);
  le_scan_stop();
  delay(1000);
  le_scan_timer_start(10000);
  // delay(2000);
  // Serial.printf("connecting...\n\r");
  //  uint8_t bd_addr[6] = {0x7d, 0x18, 0x1b, 0xf1, 0xf7, 0x2c};

  // T_GAP_LE_CONN_REQ_PARAM conn_req_param;
  // conn_req_param.scan_interval = 0x10;
  // conn_req_param.scan_window = 0x10;
  // conn_req_param.conn_interval_min = 80;
  // conn_req_param.conn_interval_max = 80;
  // conn_req_param.conn_latency = 0;
  // conn_req_param.supv_tout = 1000;
  // conn_req_param.ce_len_min = 2 * (conn_req_param.conn_interval_min - 1);
  // conn_req_param.ce_len_max = 2 * (conn_req_param.conn_interval_max - 1);

  // le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param);

  // printf("cmd_con, DestAddr: 0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X\r\n",
  // 	   bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
  // T_GAP_CAUSE result = le_connect(0, bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC, GAP_LOCAL_ADDR_LE_PUBLIC, 1000);

  // if (result == GAP_CAUSE_SUCCESS)
  // {

  // 	Serial.printf("Connect successful\r\n");
  // }
  // else
  // {
  // 	Serial.printf("Connect failed:%d\r\n", result);
  // 	while (1)
  // 	{
  // 	}

  // }
  // delay(2000);
  // le_get_conn_id(bd_addr, GAP_REMOTE_ADDR_LE_PUBLIC, &conn_id);
  // //Serial.printf("conn_id: %d\r\n", result);
  // delay(3000);
  // //T_GAP_CONN_INFO pConnInfo;
  // //le_get_conn_info(conn_id, &pConnInfo);
  // client_all_primary_srv_discovery(conn_id, client_id);
}

void loop()
{
  delay(1000);
  // rpc_le_scan_start();
  // delay(10000);
  // rpc_le_scan_stop();
  // delay(5000);
}
