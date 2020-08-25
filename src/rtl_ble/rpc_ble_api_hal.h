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
#include "app_msg.h"
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
    ret = (T_GAP_CAUSE)rpc_##FUN();       \
    return ret

#define RPC_FUN_RETURN_CAUSE_0(FUN) \
    return (T_GAP_CAUSE)rpc_##FUN()

#define RPC_FUN_RETURN_CAUSE_1(FUN, PARAM0) \
    return (T_GAP_CAUSE)rpc_##FUN(PARAM0)

#define RPC_FUN_RETURN_CAUSE_2(FUN, PARAM0, PARAM1) \
    return (T_GAP_CAUSE)rpc_##FUN(PARAM0, PARAM1)

#define RPC_FUN_RETURN_CAUSE_3(FUN, PARAM0, PARAM1, PARAM2) \
    return (T_GAP_CAUSE)rpc_##FUN(PARAM0, PARAM1, PARAM2)

#define RPC_FUN_RETURN_CAUSE_4(FUN, PARAM0, PARAM1, PARAM2, PARAM3) \
    return (T_GAP_CAUSE)rpc_##FUN(PARAM0, PARAM1, PARAM2, PARAM3)

#define RPC_FUN_RETURN_CAUSE_5(FUN, PARAM0, PARAM1, PARAM2, PARAM3, PARAM4) \
    return (T_GAP_CAUSE)rpc_##FUN(PARAM0, PARAM1, PARAM2, PARAM3, PARAM4)

#define RPC_FUN_RETURN_CAUSE_6(FUN, PARAM0, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5) \
    return (T_GAP_CAUSE)rpc_##FUN(PARAM0, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5)


typedef void (*P_FUN_HABDLE_GAP_MSG)(T_IO_MSG *p_gap_msg);

void le_register_app_cb(P_FUN_LE_APP_CB gap_callback);
void le_register_msg_handler(P_FUN_HABDLE_GAP_MSG handle_gap_msg);

#endif /* _rpc_ble_api__hal_h_ */