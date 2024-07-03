#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <snorlax/eva.h>
#include <snorlax/socket/client.h>
#include <snorlax/nanosecond.h>

const int total = 100;

int size = 1024;
const int repeat = 16;
const int request = 10000000;
int response = 0;

nanosecond_t start = { 0, };
nanosecond_t current = { 0, };
nanosecond_t end = { 0, };


socket_client_event_subscription_t * subscriptions[2][100] = { 0, };
socket_client_t * clients[2][100] = { 0, };

char * data = nil;
char * ping = nil;

static void init(void);
static void cancel(const event_engine_t * engine);
static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node);

int main(int argc, char ** argv) {
    init();
    snorlax_eva_on();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6379);
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
        clients[0][i] = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
        subscriptions[0][i] = snorlax_eva_socket_client_sub(clients[0][i], handler);
    }

    // addr.sin_port = htons(6378);
    // for(int i = 0; i < total; i++) {
    //     clients[1][i] = socket_client_gen(AF_INET, SOCK_STREAM, IPPROTO_TCP, (struct sockaddr *) &addr, addrlen);
    //     subscriptions[1][i] = snorlax_eva_socket_client_sub(clients[1][i], handler);
    // }


    return snorlax_eva_run();
}

static int count = 0;

static void on(___notnull socket_client_event_subscription_t * subscription, uint32_t type, event_subscription_event_t * node) {
    if(type == descriptor_event_type_open) {
        count = count + 1;
        if(count == 1) {
            nanosecond_get(&start);
        }
        for(int i = 0; i < repeat; i++) {
            snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, ping, size);
        }
        

    } else if(type == descriptor_event_type_read) {
        buffer_t * buffer = snorlax_eva_descriptor_buffer_in_get((descriptor_event_subscription_t *) subscription);
        uint64_t n = buffer_length(buffer);
        uint64_t res = n / 1033;
        buffer_position_set(buffer, buffer_position_get(buffer) + res * 1033);
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
                        snorlax_eva_descriptor_close((descriptor_event_subscription_t *) subscriptions[0][i]);
                    }
                    
                } else {
                    if(response % 100000 == 0) {
                        nanosecond_get(&current);
                        nanosecond_t elapse;
                        nanosecond_elapse(&start, &current, &elapse);
                        printf("| %d | %ld.%09ld |\n", response, elapse.second, elapse.nano);
                    }
                    snorlax_eva_descriptor_write((descriptor_event_subscription_t *) subscription, ping, size);
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
        subscriptions[0][i] = (socket_client_event_subscription_t * ) object_rem((object_t *) subscriptions[0][i]);
        clients[0][i] = (socket_client_t *) object_rem((object_t *) clients[0][i]);
    }
    // for(int i = 0; i < total; i++) {
    //     subscriptions[1][i] = (socket_client_event_subscription_t * ) object_rem((object_t *) subscriptions[1][i]);
    //     clients[1][i] = (socket_client_t *) object_rem((object_t *) clients[1][i]);
    // }
    free(data);
    free(ping);
    printf("response => %d\n", response);
}


static void init(void) {
    data = malloc(1024+8);
    ping = malloc(64);
    int n = sprintf(ping, "*2\r\n$4\r\nPING\r\n$%d\r\n", 1024);
    ping = realloc(ping, n + 1024 + 8);
    for(int i = 0; i < 1024; i++) {
        data[i] = (char) ((i % 60) + 64);
    }
    data[1024] = 0;

    int m = sprintf(&ping[n], "%s\r\n", data);

    size = n + m;
}
