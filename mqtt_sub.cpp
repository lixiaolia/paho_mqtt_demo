// MQTT 订阅者
#include "config.h"
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <unistd.h>

#ifdef WITH_SSL
#define ADDRESS "ssl://localhost:8883"
#else
#define ADDRESS "tcp://localhost:1883"
#endif

#define CLIENTID "hello_sub"
#define TOPIC "Hello"
#define QOS 1
#define TIMEOUT 10000L
#define USERNAME "hello_sub"
#define PASSWORD "123456"
#define DISCONNECT "hello_sub is died"

int CONNECT = 1;
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf("Message arrived\n");
    printf(" topic: %s\n", topicName);
    printf(" message: ");

    payloadptr = (char*)message->payload;
    if(strcmp(payloadptr, DISCONNECT) == 0)
    {
        printf(" \n out!!");
        CONNECT = 0;
    }

    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    printf("\n");

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf(" cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
#ifdef WITH_SSL
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    conn_opts.ssl = &ssl_opts;
    conn_opts.ssl->trustStore = "/home/lxl/Develop/myCA/ca.crt";
    conn_opts.ssl->keyStore = "/home/lxl/Develop/myCA/client.crt";
    conn_opts.ssl->privateKey = "/home/lxl/Develop/myCA/client.key";
    conn_opts.ssl->enableServerCertAuth = true; // 双向认证
#endif

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
    "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    MQTTClient_unsubscribe(client, TOPIC);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}