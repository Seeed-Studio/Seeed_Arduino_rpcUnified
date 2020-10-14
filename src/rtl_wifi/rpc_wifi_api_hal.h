#if !defined(_rpc_wifi_api__hal_h_)
#define _rpc_wifi_api__hal_h_

#include "Arduino.h"
#include "rtl_wifi_unified.h"
#include "./erpc_shim/erpc_shim_unified.h"
#include "./esp_lib/esp_lib_unified.h"
#include "rpc_unified_log.h"
#include "erpc_port.h"


void system_event_callback_reg(system_event_cb_t system_event_cb);

#endif /* _rpc_wifi_api__hal_h_ */