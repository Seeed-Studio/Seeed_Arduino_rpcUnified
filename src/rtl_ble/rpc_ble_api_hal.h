#if !defined(_rpc_ble_api__hal_h_)
#define _rpc_ble_api__hal_h_

#include "bt_types.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_config.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "Arduino.h"

#define RPC_FUN_GAP_SET_PARAM(FUN, PARAM_TYPE)                     \
    Serial.printf("%s called\n\r", #FUN);                          \
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;                           \
    binary_t value;                                                \
    value.dataLength = len;                                        \
    value.data = (uint8_t *)p_value;                               \
    ret = (T_GAP_CAUSE)rpc_##FUN((RPC_##PARAM_TYPE)param, &value); \
    return ret

#define RPC_FUN_GAP_GET_PARAM(FUN, PARAM_TYPE)                     \
    Serial.printf("%s called\n\r", #FUN);                          \
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;                           \
    binary_t value;                                                \
    ret = (T_GAP_CAUSE)rpc_##FUN((RPC_##PARAM_TYPE)param, &value); \
    if (GAP_SUCCESS == ret)                                        \
        memcpy(p_value, value.data, value.dataLength);             \
    erpc_free(value.data);                                         \
    return ret

#define RPC_FUN_GAP_VOID(FUN)             \
    Serial.printf("%s called\n\r", #FUN); \
    T_GAP_CAUSE ret = GAP_CAUSE_SUCCESS;  \
    ret = (T_GAP_CAUSE)rpc_##FUN();             \
    return ret

void le_register_app_cb(P_FUN_LE_APP_CB gap_callback);

#endif /* _rpc_ble_api__hal_h_ */