#include <stdlib.h>
#include "rpc_ble_api_hal.h"

P_FUN_LE_APP_CB _gap_callback = NULL;

void le_register_app_cb(P_FUN_LE_APP_CB gap_callback)
{
    _gap_callback = gap_callback;
}
