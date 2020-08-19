#include <stdlib.h>
#include <string.h>
#include "rpc_ble_api_hal.h"
#include "rpc_ble_api.h"
#include "gap_adv.h"
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

extern P_FUN_LE_APP_CB _ble_gap_callback;

void le_register_app_cb(P_FUN_LE_APP_CB ble_gap_callback)
{
    _ble_gap_callback = ble_gap_callback;
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
