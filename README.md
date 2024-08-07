__EVA<sup style="font-size: .8rem">EVENT LIB</sup>__
====================================================

High performance event loop written c.

__Supported OS__

| LINUX |
| ----- |
| O     |

> We are preparing to operate on Windows and Mac.

__Benchmark #1: Local echo server: send & receive 1024 bytes random bytes.__

As a result of transmitting 1 million local echo 1024 bytes, approximately 300,000 can be transmitted per second.

| RESPONSE | ELAPSE (NANO) |
| -------- | ------------- |
|  100000  | 0.321793360   |
|  200000  | 0.658478196   |
|  300000  | 0.986696707   |
|  400000  | 1.306567191   |
|  500000  | 1.683766227   |
|  600000  | 2.024611389   |
|  700000  | 2.330603947   |
|  800000  | 2.637404270   |
|  900000  | 2.954828010   |
| 1000000  | 3.267341360   |

__Example Code: Server__

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <snorlax/eva.h>
#include <snorlax/socket/server.h>
#include <snorlax/socket/session.h>
#include <snorlax/deserializer.h>
#include <snorlax/serializer.h>

socket_server_event_subscription_t * subscription = nil;
socket_server_t * server = nil;

static void cancel(const event_engine_t * engine);
static void serverOn(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);
static void sessionOn(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    socket_server_event_subscription_handler_t callback[descriptor_event_type_max] = {
        serverOn,
        serverOn,
        serverOn,
        serverOn,
        serverOn,
        serverOn
    };

    socket_session_event_subscription_handler_t handler[descriptor_event_type_max] = {
        sessionOn,
        sessionOn,
        sessionOn,
        sessionOn,
        sessionOn,
        sessionOn
    };

    server = socket_server_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
    subscription = snorlax_eva_socket_server_sub(server, handler, callback);
    return snorlax_eva_run();
}

static void serverOn(___notnull socket_server_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
}

static int count = 0;

static void sessionOn(___notnull socket_session_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_open) {
        count = count + 1;
    } else if(type == descriptor_event_type_close) {
        count = count - 1;
    } else if(type == descriptor_event_type_read) {
        buffer_t * in = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        buffer_t * out = snorlax_eva_descriptor_buffer_out_get((descriptor_event_subscription_t *) subscription);
        deserialize(in, out);
    }
}

static void cancel(const event_engine_t * engine) {
    subscription = (socket_server_event_subscription_t * ) object_rem((object_t *) subscription);
    socket_close(server);
    server = (socket_server_t *) object_rem((object_t *) server);
}
```

__Example Code: Client Pool__

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <snorlax/eva.h>
#include <snorlax/socket/client.h>
#include <snorlax/nanosecond.h>

const int total = 100;

const int size = 1024;
const int repeat = 16;
const int request = 1000000;
int response = 0;

nanosecond_t start = { 0, };
nanosecond_t current = { 0, };
nanosecond_t end = { 0, };

socket_client_event_subscription_t * subscriptions[100] = { 0, };
socket_client_t * clients[100] = { 0, };

char * data = nil;

static void init(void);
static void cancel(const event_engine_t * engine);
static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    init();
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    socket_client_event_subscription_handler_t handler[descriptor_event_type_max] = {
        on,
        on,
        on,
        on,
        on,
        on
    };

    for(int i = 0; i < total; i++) {
        clients[i] = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
        subscriptions[i] = snorlax_eva_socket_client_sub(clients[i], handler);
    }


    return snorlax_eva_run();
}

static int count = 0;

static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_open) {
        count = count + 1;
        if(count == 1) {
            nanosecond_get(&start);
        }

        snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, data, size * repeat);

    } else if(type == descriptor_event_type_read) {
        buffer_t * buffer = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        uint64_t n = buffer_length(buffer);
        uint64_t res = n / size;
        buffer_position_set(buffer, buffer_position_get(buffer) + res * size);
        // printf("response => %d\n", response);
        for(int i = 0; i < res && response < request; i++) {
            response = response + 1;

            if(response <= request) {
                if(response == request) {
                    nanosecond_get(&end);
                    nanosecond_t elapse;
                    nanosecond_elapse(&start, &end, &elapse);
                    printf("| %d | %ld.%09ld |\n", response, elapse.second, elapse.nano);
                    for(int i = 0; i < total; i++) {
                        snorlax_eva_descriptor_close((descriptor_event_subscription_t *) subscriptions[i]);
                    }
                    
                } else {
                    if(response % 10000 == 0) {
                        nanosecond_get(&current);
                        nanosecond_t elapse;
                        nanosecond_elapse(&start, &current, &elapse);
                        printf("| %d | %ld.%09ld |\n", response, elapse.second, elapse.nano);
                    }
                    snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, data, size);
                }
                break;
            }
        }

    } else if(type == descriptor_event_type_close) {
        if(count > 0) {
            count = count - 1;
            if(count == 0) {
                printf("cancel\n");
                snorlax_eva_off(cancel);
            }
        }
        
    }
}

static void cancel(const event_engine_t * engine) {
    for(int i = 0; i < total; i++) {
        subscriptions[i] = (socket_client_event_subscription_t * ) object_rem((object_t *) subscriptions[i]);
        clients[i] = (socket_client_t *) object_rem((object_t *) clients[i]);
    }
    free(data);
    printf("response => %d\n", response);
}

static void init(void) {
    data = malloc(size * repeat);
    char * origin = malloc(size);
    for(int i = 0; i < size; i++) {
        origin[i] = (char) i;
    }
    origin[1023] = 0;
    for(int i = 0; i < repeat; i++) {
        memcpy(&data[i], origin, 1024);
    }
    free(origin);
}
```