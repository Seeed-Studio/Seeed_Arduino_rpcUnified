
/*
    The MIT License (MIT)
    Copyright (C) 2020  Seeed Technology Co.,Ltd.
*/


#include "rpc_ble_api.h"
#include "rpc_ble_callback.h"
#include "rpc_ble_callback_server.h"
#include "erpc_arduino_uart_transport.h"
#include "erpc_basic_codec.h"
#include "erpc_arbitrated_client_manager.h"
#include "erpc_threading.h"
#include "erpc_simple_server.h"
#include "erpc_transport_arbitrator.h"
#include "erpc_port.h"
#include "gap_scan.h"
#include "rpc_ble_api_hal.h"


using namespace erpc;

class MyMessageBufferFactory : public MessageBufferFactory
{
public:
    virtual MessageBuffer create()
    {
        uint8_t *buf = new uint8_t[1024];
        return MessageBuffer(buf, 1024);
    }

    virtual void dispose(MessageBuffer *buf)
    {
        if (*buf)
        {
            delete[] buf->get();
        }
    }
};

UartTransport g_transport(&Serial1, 1843200);
MyMessageBufferFactory g_msgFactory;
BasicCodecFactory g_basicCodecFactory;
ArbitratedClientManager *g_client;
TransportArbitrator g_arbitrator;
SimpleServer g_server;
Crc16 g_crc16;

// setup and loop code block
extern void _real_body();

/**
 * @brief  Initialize erpc server task
 * @return void
 */
void add_services(erpc::SimpleServer *server)
{
    server->addService(static_cast<erpc::Service *>(create_rpc_ble_callback_service()));
}

void runClient(void *arg)
{
   _real_body();
}
void rpc_ble_handle_gap_msg(const binary_t *gap_msg)
{
    Serial.printf("rpc_ble_handle_gap_msg\n\r");
    for (int i = 0; i < gap_msg->dataLength; i++)
    {
        Serial.printf("%02x, ", gap_msg->data[i]);
    }
    Serial.printf("\n\r");
}

void rpc_ble_gap_callback(uint8_t cb_type, const binary_t *cb_data)
{
    T_LE_SCAN_INFO *p_data = (T_LE_SCAN_INFO *)cb_data->data;
    Serial.printf("rpc_ble_gap_callback cb_type:%d\n\r", cb_type);
    Serial.printf("bt_addr: %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                  (p_data->bd_addr)[5],
                  (p_data->bd_addr)[4],
                  (p_data->bd_addr)[3],
                  (p_data->bd_addr)[2],
                  (p_data->bd_addr)[1],
                  (p_data->bd_addr)[0]);
    // for (int i = 0; i < cb_data->dataLength; i++)
    // {
    //     Serial.printf("%0x, ", cb_data->data[i]);
    // }
    // Serial.printf("\n\r");

    if(_gap_callback != NULL)
        _gap_callback(cb_type, cb_data->data);
    
}

void runServer(void *arg)
{
    Serial.println("start erpc server");
    /* run server */
    while (true)
    {
        g_server.poll();
        delay(20);
    }
}

Thread serverThread(&runServer, configMAX_PRIORITIES - 2, 1024, "runServer");
Thread clientThread(&runClient, configMAX_PRIORITIES - 1, 2048, "runClient");

void erpc_init()
{
    Serial.printf("erpc init\n\r");
    g_transport.init();
    g_arbitrator.setSharedTransport(&g_transport);
    g_arbitrator.setCodec(g_basicCodecFactory.create());

    g_client = new ArbitratedClientManager();
    g_client->setArbitrator(&g_arbitrator);
    g_client->setCodecFactory(&g_basicCodecFactory);
    g_client->setMessageBufferFactory(&g_msgFactory);

    g_arbitrator.setCrc16(&g_crc16);

    g_server.setTransport(&g_arbitrator);
    g_server.setCodecFactory(&g_basicCodecFactory);
    g_server.setMessageBufferFactory(&g_msgFactory);

    add_services(&g_server);

    serverThread.start();
    clientThread.start();

    g_client->setServer(&g_server);
    g_client->setServerThreadId(serverThread.getThreadId());
}

void _wrap_body()
{
    Serial.begin(115200);

    erpc_init();
    
    vTaskStartScheduler();

    while (1)
        ;

    return;
}
