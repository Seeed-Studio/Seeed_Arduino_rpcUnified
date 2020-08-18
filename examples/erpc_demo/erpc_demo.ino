#include "Seeed_erpcUnified.h"

void setup() {
  // put your setup code here, to run once:
   rpc_le_scan_start();
   delay(10000);
   rpc_le_scan_stop();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
