#include "seeed_rpcUnified.h"
#include "rtl_ble/ble_unified.h"

extern void ble_handle_gap_msg(T_IO_MSG *p_gap_msg);
extern T_APP_RESULT ble_gatt_server_callback(T_SERVER_ID service_id, void *p_para);

uint8_t conn_id = 0xff;
bool isNotify = false;

/** @brief  GAP - scan response data (max size = 31 bytes) */
static const uint8_t scan_rsp_data[] =
    {
        0x03,                  /* length */
        GAP_ADTYPE_APPEARANCE, /* type="Appearance" */
        LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
        HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
};

/** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
static const uint8_t adv_data[] =
    {
        /* Flags */
        0x02,             /* length */
        GAP_ADTYPE_FLAGS, /* type="Flags" */
        GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
        /* Service */
        0x03, /* length */
        GAP_ADTYPE_16BIT_COMPLETE,
        LO_WORD(0x180F),
        HI_WORD(0x180F),
        /* Local name */
        0x0F, /* length */
        GAP_ADTYPE_LOCAL_NAME_COMPLETE,
        'B',
        'L',
        'E',
        '_',
        'P',
        'E',
        'R',
        'I',
        'P',
        'H',
        'E',
        'R',
        'A',
        'L',
};
char v1_user_descr[] = "V1 read characteristic";
uint16_t default_value = 0x00;
uint8_t srcv_app_id = 0;
uint8_t srcv_handle = 0;
uint8_t char_handle = 0;
uint8_t desc_handle1 = 0;
uint8_t desc_handle2  = 0;

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
  ble_server_init(BLE_SERVER_MAX_APPS);
  le_register_msg_handler(ble_handle_gap_msg);
  le_register_gatts_cb(ble_gatt_server_callback);

  le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(adv_data), (void *)adv_data);
  le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, sizeof(scan_rsp_data), (void *)scan_rsp_data);

  ble_service_t srcv;

  srcv.uuid_length = UUID_16BIT_SIZE;
  uint16_t srcv_uuid = 0x180F;
  memcpy(&(srcv.uuid), &srcv_uuid, 2);
  srcv.is_primary = true;

  srcv_app_id = ble_create_service(srcv);
  Serial.printf("srcv_app_id: %d\n\r", srcv_app_id);

  ble_char_t CHAR;
  CHAR.uuid_length = UUID_16BIT_SIZE;
  uint16_t CHAR_uuid = 0x2A19;
  memcpy(&(CHAR.uuid), &CHAR_uuid, 2);
  CHAR.properties = (GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY);
  CHAR.permissions = GATT_PERM_READ | GATT_PERM_WRITE;
  char_handle = ble_create_char(srcv_app_id, CHAR);
  Serial.printf("char_handle: %d\n\r", char_handle);
  ble_desc_t desc;

  desc.flags =  ATTRIB_FLAG_CCCD_APPL;
  desc.uuid_length = UUID_16BIT_SIZE;
  uint16_t desc_uuid = 0x2902;
  uint16_t default_vlaue = 0x0000;
  memcpy(&(desc.uuid), &desc_uuid, 2);
  memcpy(&(desc.uuid[2]), &default_vlaue, 2);
  desc.p_value = (uint8_t *)&default_value;
  desc.vlaue_length = 2;
  desc.permissions = (GATT_PERM_READ | GATT_PERM_WRITE);
  desc_handle1 = ble_create_desc(srcv_app_id, char_handle, desc);
  Serial.printf("desc_handle1: %d\n\r", desc_handle1);

  ble_desc_t desc2;
  desc2.flags = ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z;
  desc2.uuid_length = UUID_16BIT_SIZE;
  uint16_t desc2_uuid = 0x2901;
  memcpy(&(desc2.uuid), &desc2_uuid, 2);
  desc2.p_value = (uint8_t *)v1_user_descr;
  desc2.vlaue_length = (sizeof(v1_user_descr) - 1);
  desc2.permissions = (GATT_PERM_READ | GATT_PERM_WRITE);
  desc_handle2 = ble_create_desc(srcv_app_id, char_handle, desc2);
  Serial.printf("desc_handle2: %d\n\r", desc_handle2);

  srcv_handle = ble_service_start(srcv_app_id);

  ble_start();

  delay(2000);
  le_adv_start();
}
uint8_t battlevel = 0;
uint8_t temp[100];

void loop()
{
 
  if (isNotify)
  {
    battlevel = (battlevel + 1)%100;
    server_send_data(0, srcv_handle, char_handle, &battlevel, 1, GATT_PDU_TYPE_ANY);
    Serial.println(battlevel);
  }
  uint16_t value_length = ble_server_get_attr_value(srcv_app_id,  desc_handle2, temp);
  // Serial.println("desc_handle2 value: ");
  // for(uint16_t i = 0; i < value_length; i++)
  // {
  //     Serial.printf("%02x ", temp[i]);
  // }
  // Serial.println(" ");
  delay(1000);
  // rpc_le_scan_start();
  // delay(10000);
  // rpc_le_scan_stop();
  // delay(5000);
}
