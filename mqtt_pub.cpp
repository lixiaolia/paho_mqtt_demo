// MQTT 发布者
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <unistd.h>

#define NUM_THREADS 2
#define ADDRESS "tcp://localhost:1883"
#define CLIENTID "hello_pub"
#define TOPIC "Hello"
#define PAYLOAD "Hello, World!"
#define QOS 1
#define TIMEOUT 10000L
#define USERNAME "hello_pub"
#define PASSWORD "123456"
#define DISCONNECT "hello_pub is died"

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
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    pubmsg.payload = const_cast<char *>(PAYLOAD);
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    while(CONNECT)
    {
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",(int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
        usleep(3000000L);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}
