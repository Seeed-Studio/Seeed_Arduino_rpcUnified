#include "Arduino.h"
#include "seeed_rpcUnified.h"
#include "rtl_ble/ble_unified.h"

T_GAP_DEV_STATE ble_gap_dev_state = {0, 0, 0, 0, 0}; /**< GAP device state */
T_GAP_CONN_STATE ble_gap_conn_state = GAP_CONN_STATE_DISCONNECTED;
T_APP_LINK ble_clinet_link_table[BLE_LE_MAX_LINKS];
RPC_T_GAP_ROLE ble_dev_role = RPC_GAP_LINK_ROLE_SLAVE; // 0:close 1:server 2:client

/**
 * @brief    Handle msg GAP_MSG_LE_DEV_STATE_CHANGE
 * @note     All the gap device state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] new_state  New gap device state
 * @param[in] cause GAP device state change cause
 * @return   void
 */
void ble_dev_state_evt_handler(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    Serial.printf("ble_dev_state_evt_handler: init state %d, adv state %d, cause 0x%x\n\r", new_state.gap_init_state, new_state.gap_adv_state, cause);
    if (ble_gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            Serial.printf("GAP stack ready\n\r");
            // BLE stack is ready
            uint8_t bt_addr[6];
            gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
            Serial.printf("[BLE Device] Local BT addr: %2x:%2x:%2x:%2x:%2x:%2x\n\r", bt_addr[5], bt_addr[4], bt_addr[3], bt_addr[2], bt_addr[1], bt_addr[0]);
        }
    }

    // Assign different tasks according to different roles
    // As a Client
    if (ble_dev_role == RPC_GAP_LINK_ROLE_MASTER)
    {
        if (ble_gap_dev_state.gap_scan_state != new_state.gap_scan_state)
        {
            if (new_state.gap_scan_state == GAP_SCAN_STATE_IDLE)
            {
                Serial.printf("[BLE Device] GAP scan stop\n\r");
            }
            else if (new_state.gap_scan_state == GAP_SCAN_STATE_SCANNING)
            {
                Serial.printf("[BLE Device] GAP scan start\n\r");
            }
        }
    }
    // As a Server
    else
    {
        if (ble_gap_dev_state.gap_adv_state != new_state.gap_adv_state)
        {
            if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
            {
                if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
                {
                    Serial.printf("[BLE Device] GAP adv stopped: because connection created\n\r");
                }
                else
                {
                    Serial.printf("[BLE Device] GAP adv stopped\n\r");
                }
            }
            else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                Serial.printf("[BLE Device] GAP adv start\n\r");
            }
        }
    }

    ble_gap_dev_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_STATE_CHANGE
 * @note     All the gap conn state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New gap connection state
 * @param[in] disc_cause Use this cause when new_state is GAP_CONN_STATE_DISCONNECTED
 * @return   void
 */
void ble_conn_state_evt_handler(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{

    Serial.printf("ble_conn_state_evt_handler: conn_id %d old_state %d new_state %d, disc_cause 0x%x\n\r", conn_id, ble_gap_conn_state, new_state, disc_cause);

    if (ble_dev_role == RPC_GAP_LINK_ROLE_MASTER)
    {

        if (conn_id >= BLE_LE_MAX_LINKS)
        {
            return;
        }

        ble_clinet_link_table[conn_id].conn_state = new_state;

        switch (new_state)
        {
        case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE)) && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                Serial.printf("connection lost, conn_id %d, cause 0x%x\n\r", conn_id, disc_cause);
            }
            Serial.printf("[BLE Device] Disconnected conn_id %d\n\r", conn_id);
            memset(&ble_clinet_link_table[conn_id], 0, sizeof(T_APP_LINK));
            break;
        }
        case GAP_CONN_STATE_CONNECTED:
        {
            le_get_conn_addr(conn_id, ble_clinet_link_table[conn_id].bd_addr, (uint8_t *)&ble_clinet_link_table[conn_id].bd_type);
            Serial.printf("[BLE Device] Connected conn_id %d\n\r", conn_id);
            {
                uint8_t tx_phy;
                uint8_t rx_phy;
                le_get_conn_param(GAP_PARAM_CONN_TX_PHY_TYPE, &tx_phy, conn_id);
                le_get_conn_param(GAP_PARAM_CONN_RX_PHY_TYPE, &rx_phy, conn_id);
                Serial.printf("GAP_CONN_STATE_CONNECTED: tx_phy %d, rx_phy %d\n\r", tx_phy, rx_phy);
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        switch (new_state)
        {
        case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE)) && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {

                Serial.printf("connection lost cause 0x%x\n\r", disc_cause);
            }
            Serial.printf("[BLE Device] BT Disconnected, start ADV\n\r\n\r");
            le_adv_start();
            break;
        }
        case GAP_CONN_STATE_CONNECTED:
        {
            uint16_t conn_interval;
            uint16_t conn_latency;
            uint16_t conn_supervision_timeout;
            uint8_t remote_bd[6];
            T_GAP_REMOTE_ADDR_TYPE remote_bd_type;

            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
            le_get_conn_addr(conn_id, remote_bd, (uint8_t *)&remote_bd_type);

            Serial.printf("GAP_CONN_STATE_CONNECTED:remote_bd %x:%x:%x:%x:%x:%x, remote_addr_type %d, conn_interval 0x%x, conn_latency 0x%x, conn_supervision_timeout 0x%x\n\r", remote_bd[0], remote_bd[1], remote_bd[2], remote_bd[3], remote_bd[4], remote_bd[5], remote_bd_type, conn_interval, conn_latency, conn_supervision_timeout);
            Serial.printf("[BLE Device] BT Connected\n\r\n\r");
            break;
        }
        default:
            break;
        }
    }
    ble_gap_conn_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_PARAM_UPDATE
 * @note     All the connection parameter update change  events are pre-handled in this function.
 * @param[in] conn_id Connection ID
 * @param[in] status  New update state
 * @param[in] cause Use this cause when status is GAP_CONN_PARAM_UPDATE_STATUS_FAIL
 * @return   void
 */
void ble_param_update_evt_handler(uint8_t conn_id, uint8_t status, uint16_t cause)
{
    switch (status)
    {
    case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS:
    {
        uint16_t conn_interval;
        uint16_t conn_slave_latency;
        uint16_t conn_supervision_timeout;

        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_slave_latency, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
        Serial.printf("connParamUpdateEvtHandlerDefault update success:conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x\n\r", conn_interval, conn_slave_latency, conn_supervision_timeout);
        break;
    }
    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
    {
        Serial.printf("connParamUpdateEvtHandlerDefault update failed: cause 0x%x\n\r", cause);
        break;
    }
    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
    {
        Serial.printf("connParamUpdateEvtHandlerDefault update pending: conn_id %d\n\r", conn_id);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_AUTHEN_STATE_CHANGE
 * @note     All the gap authentication state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New authentication state
 * @param[in] cause Use this cause when new_state is GAP_AUTHEN_STATE_COMPLETE
 * @return   void
 */
void ble_authen_state_evt_handler(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    Serial.printf("app_handle_authen_state_evt:conn_id %d, cause 0x%x\n\r", conn_id, cause);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
    {
        Serial.printf("app_handle_authen_state_evt: GAP_AUTHEN_STATE_STARTED\n\r");
    }
    break;

    case GAP_AUTHEN_STATE_COMPLETE:
    {
        if (cause == GAP_SUCCESS)
        {
            Serial.printf("Pair success\n\r");
            Serial.printf("app_handle_authen_state_evt: GAP_AUTHEN_STATE_COMPLETE pair success\n\r");
        }
        else
        {
            Serial.printf("Pair failed: cause 0x%x\n\r", cause);
            Serial.printf("app_handle_authen_state_evt: GAP_AUTHEN_STATE_COMPLETE pair failed\n\r");
        }
    }
    break;

    default:
    {
        Serial.printf("app_handle_authen_state_evt: unknown newstate %d\n\r", new_state);
    }
    break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_MTU_INFO
 * @note     This msg is used to inform APP that exchange mtu procedure is completed.
 * @param[in] conn_id Connection ID
 * @param[in] mtu_size  New mtu size
 * @return   void
 */
void ble_mtu_info_evt_handler(uint8_t conn_id, uint16_t mtu_size)
{
    Serial.printf("app_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d\n\r", conn_id, mtu_size);
}

/**
 * @brief    All the BT GAP MSG are pre-handled in this function.
 * @note     Then the event handling function shall be called according to the
 *           subtype of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void ble_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    Serial.printf("ble_central_app_handle_gap_msg: subtype %d\n\r", p_gap_msg->subtype);

    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
    {
        Serial.printf("GAP_MSG_LE_DEV_STATE_CHANGE\n\r");
        ble_dev_state_evt_handler(gap_msg.msg_data.gap_dev_state_change.new_state,
                                  gap_msg.msg_data.gap_dev_state_change.cause);
    }
    break;
    case GAP_MSG_LE_CONN_STATE_CHANGE:
    {
        Serial.printf("GAP_MSG_LE_CONN_STATE_CHANGE\n\r");
        ble_conn_state_evt_handler(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                   (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                   gap_msg.msg_data.gap_conn_state_change.disc_cause);
    }
    break;
    case GAP_MSG_LE_CONN_PARAM_UPDATE:
    {
        Serial.printf("GAP_MSG_LE_CONN_PARAM_UPDATE\n\r");
        ble_param_update_evt_handler(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                     gap_msg.msg_data.gap_conn_param_update.status,
                                     gap_msg.msg_data.gap_conn_param_update.cause);
    }
    break;
    case GAP_MSG_LE_CONN_MTU_INFO:
    {
        Serial.printf("GAP_MSG_LE_CONN_MTU_INFO\n\r");
        ble_mtu_info_evt_handler(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                 gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
    }
    break;
    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
    {
        Serial.printf("GAP_MSG_LE_AUTHEN_STATE_CHANGE\n\r");
        ble_authen_state_evt_handler(gap_msg.msg_data.gap_authen_state.conn_id,
                                     gap_msg.msg_data.gap_authen_state.new_state,
                                     gap_msg.msg_data.gap_authen_state.status);
    }
    break;
    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
    {
        Serial.printf("GAP_MSG_LE_BOND_PASSKEY_DISPLAY\n\r");
        conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
        le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;
    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
    {
        Serial.printf("GAP_MSG_LE_BOND_PASSKEY_INPUT\n\r");
        uint32_t display_value = 0;
        conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
        le_bond_get_display_key(conn_id, &display_value);
        Serial.printf("GAP_MSG_LE_BOND_PASSKEY_DISPLAY:passkey %d\n\r", display_value);
        le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        Serial.printf("GAP_MSG_LE_BOND_PASSKEY_DISPLAY:passkey %d\n\r", display_value);
    }
    break;
    case GAP_MSG_LE_BOND_OOB_INPUT:
    {
        Serial.printf("GAP_MSG_LE_BOND_OOB_INPUT\n\r");
        uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
        Serial.printf("GAP_MSG_LE_BOND_OOB_INPUT\r\n\n\r");
        le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
        le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;
    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
    {
        Serial.printf("GAP_MSG_LE_BOND_USER_CONFIRMATION\n\r");
        uint32_t display_value = 0;
        conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
        le_bond_get_display_key(conn_id, &display_value);
        Serial.printf("GAP_MSG_LE_BOND_USER_CONFIRMATION: passkey %ld\r\n\n\r", display_value);
        le_bond_user_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;
    case GAP_MSG_LE_BOND_JUST_WORK:
    {
        Serial.printf("GAP_MSG_LE_BOND_JUST_WORK\n\r");
        conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
        le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        Serial.printf("GAP_MSG_LE_BOND_JUST_WORK\r\n\n\r");
    }
    break;
    default:
        Serial.printf("gapMsgHandlerDefault: unknown subtype %d\n\r", p_gap_msg->subtype);
        break;
    }
}
