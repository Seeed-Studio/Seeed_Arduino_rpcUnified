
/*
    The MIT License (MIT)
    Copyright (C) 2020  Seeed Technology Co.,Ltd.
*/

#include "erpc/erpc_arduino_uart_transport.h"
#include "erpc/erpc_basic_codec.h"
#include "erpc/erpc_arbitrated_client_manager.h"
#include "erpc/erpc_threading.h"
#include "erpc/erpc_simple_server.h"
#include "erpc/erpc_transport_arbitrator.h"
#include "erpc/erpc_port.h"
#include "erpc/erpc_shim_unified.h"

using namespace erpc;

#include "rpc_unified_config.h"

class MyMessageBufferFactory : public MessageBufferFactory
{
public:
    virtual MessageBuffer create()
    {
        uint8_t *buf = new uint8_t[RPC_BUFFER_SIZE];
        return MessageBuffer(buf, RPC_BUFFER_SIZE);
    }

    virtual void dispose(MessageBuffer *buf)
    {
        if (*buf)
        {
            delete[] buf->get();
        }
    }
};


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
    server->addService(static_cast<erpc::Service *>(create_rpc_wifi_callback_service()));
}

void runClient(void *arg)
{
    (void)arg;
    delay(100);
    _real_body();
}

void runServer(void *arg)
{
    (void)arg;
    /* run server */
    while (true)
    {
        g_server.poll();
        delay(20);
    }
}

Thread serverThread(&runServer, configMAX_PRIORITIES, RPC_SERVER_STACK_SIZE, "runServer");
Thread clientThread(&runClient, configMAX_PRIORITIES - 2, RPC_CLIENT_STACK_SIZE, "runClient");

void erpc_init()
{
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
    RTL8720_RESET();
    erpc_init();

    vTaskStartScheduler();

    while (1)
        ;

    return;
}
