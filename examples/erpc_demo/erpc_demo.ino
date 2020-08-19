#include "Seeed_erpcUnified.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
  {
  };

  Serial.printf("rpc_ble_init\n\r");
  rpc_ble_init();
  Serial.printf("rpc_gap_config_max_le_link_num\n\r");
  rpc_gap_config_max_le_link_num(3);
  Serial.printf("rpc_le_gap_init\n\r");
  rpc_le_gap_init(3);
  Serial.printf("rpc_ble_start\n\r");
  rpc_ble_start();

  // rpc_le_scan_start();
  // delay(10000);
  // rpc_le_scan_stop();
}

void loop()
{
  // rpc_le_scan_start();
  // delay(10000);
  // rpc_le_scan_stop();
  // delay(5000);
}
