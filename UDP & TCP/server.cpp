#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include "helpers.h"
#include "data.h"
#include "list.h"
#include "queue.h"
#include "tcp_protocol_request.h"

using namespace std;

void usage(char *file)
{
	fprintf(stderr, "Usage: %s id_client ip_server port_server\n", file);
	exit(0);
}

int main(int argc, char *argv[]){
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    //Initializari
    int tcp_sockfd, udp_sockfd, newsockfd, portno, fdmax;
	char stdin_buffer[BUFLEN], udp_buffer[1600];
    char tcp_buffer[LENGTH];
    Queue_TCP* q = createQueue_tcp();
    memset(tcp_buffer, 0, LENGTH);
	struct sockaddr_in tcp_serv_addr, udp_serv_addr, cli_addr;
	int n, ret;
	fd_set read_fds, tmp_fds;
    
    if (argc < 2) {
		usage(argv[0]);
	}

    FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

    //Socket primire date protocol TCP
    tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(tcp_sockfd < 0, "tcp socket");

	memset((char *) &tcp_serv_addr, 0, sizeof(tcp_serv_addr));
	tcp_serv_addr.sin_family = AF_INET;
	tcp_serv_addr.sin_port = htons(portno);
	tcp_serv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(tcp_sockfd, (struct sockaddr *) &tcp_serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "tcp bind");

	ret = listen(tcp_sockfd, MAX_CLIENTS);
	DIE(ret < 0, "tcp listen");

    //Socket primire date protocol UDP
    udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	DIE(udp_sockfd < 0, "udp socket");

	memset((char *) &udp_serv_addr, 0, sizeof(udp_serv_addr));
	udp_serv_addr.sin_family = AF_INET;
	udp_serv_addr.sin_port = htons(portno);
	udp_serv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(udp_sockfd, (struct sockaddr *) &udp_serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "udp bind");

    FD_SET(udp_sockfd, &read_fds);
    FD_SET(tcp_sockfd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    int flag = 1;
    int result = setsockopt(tcp_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &flag, sizeof(int));
    DIE(result < 0, "error disable nagle");

    int enable = 1;
    result = setsockopt(tcp_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    DIE(result < 0, "port");
	
    int max = STDIN_FILENO;
    if(udp_sockfd > max) max = udp_sockfd;
    if(tcp_sockfd > max) max = tcp_sockfd;
    fdmax = max;

    //Lista de clienti conectati la server
    struct Client_List* clientList = NULL;

    while(1){
        tmp_fds = read_fds; 
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");

        //Daca primesc date de la STDIN
        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
		
			memset(stdin_buffer, 0, BUFLEN);
			fgets(stdin_buffer, BUFLEN - 1, stdin);

			DIE((strncmp(stdin_buffer, "exit", 4) != 0), "only exit"); 
			
            for(int i = 0; i <= fdmax; i++){
                close(i);
            }

            break;
        }
        for(int i = 0; i <= fdmax; i++){
            if(FD_ISSET(i, &tmp_fds)){
                if(i == udp_sockfd){
                    //Adaug datele primite intr-o structura de tip UDP_Response
                    memset(udp_buffer, 0, BUFLEN);
                    socklen_t len = sizeof(udp_serv_addr);
                    ret = recvfrom(udp_sockfd, &udp_buffer, BUFLEN, MSG_DONTWAIT, (struct sockaddr *)&udp_serv_addr, &len);
                    DIE(ret<0, "udp recv");

                    UDP_Response response;
                    memset(&response, 0, sizeof(UDP_Response));

                    char *pointer = udp_buffer;
                    memcpy(&response.topic, pointer, 50);
                    pointer += 50;
                    memcpy(&response.type, pointer, 1);
                    pointer += 1;
                    memcpy(&response.payload, pointer, 1500);
                    memcpy(&response.ip, inet_ntoa(udp_serv_addr.sin_addr), 20);
                    response.port = ntohs(udp_serv_addr.sin_port);

                    //Parcurg lista de clienti
                    struct Client_List* client = clientList;
                    while(client){
                        //Daca este abonat la topicul de pe care s-au primit mesaje
                        if(contains(client->client.topics, response.topic)){
                            struct Topic_List* topic = get_topic(client->client.topics, response.topic);
                            //Daca e activ trimti mesajul
                            if(client->client.status == 1){
                                n = send(client->client.socket, &response, sizeof(UDP_Response), 0);
                                DIE(n < 0, "send udp response");
                            }
                            //Daca nu e activ
                            if(client->client.status == 0){
                                //Dar are optiunea de store and forward pornita adaug mesajele intr-o coada
                                if(topic->topic.sf == 1){
                                    struct UDP_Message message;
                                    memcpy(&message.topic, &response.topic, 51);
                                    memcpy(&message.type, &response.type, 1);
                                    memcpy(&message.payload, &response.payload, 1501);
                                    enqueue(client->client.queue_message, message); 
                                }
                            }
                        }    
                        client = client->next;
                    }
                }
                else{
                    if(i == tcp_sockfd){
                        int event = 0;
                        
                        //Accept conexiunea
                        int len2 = sizeof(cli_addr);
                        newsockfd = accept(tcp_sockfd, (struct sockaddr*)&cli_addr, (socklen_t*)&len2);
                        DIE(newsockfd < 0, "tcp accept");

                        TCP_Command name;
                        memset(&name, 0, sizeof(TCP_Command));

                        protocol_tcp(tcp_buffer, newsockfd, q);
                        memcpy(&name, &q->front->item, sizeof(TCP_Command));
                        dequeue_tcp(q);

                        //Parcurg lista de clienti
                        struct Client_List* clients = clientList;
                        while(clients){
                            //Daca este un client ce s-a mai conectat cu acelasi id si este activ
                            if(strcmp(clients->client.id, name.topic) == 0 && clients->client.status == 1){
                                UDP_Response response;
                                memset(&response, 0, sizeof(UDP_Response));
                                memcpy(response.topic, "error", 5);
                                
                                n = send(newsockfd, &response, sizeof(UDP_Response), 0);
                                DIE(n < 0, "send error");

                                printf("Client %s already connected.\n", clients->client.id);
                                event = 1;
                                close(newsockfd);
                                break;
                            }
                            //Daca este un client ce s-a mai conectat cu acelasi id si nu este activ
                            if(strcmp(clients->client.id, name.topic) == 0 && clients->client.status == 0){
                                clients->client.status = 1;
                                clients->client.socket = newsockfd;
                                
                                printf("New client %s connected from %s:%d.\n", name.topic,
                                    inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

                                int result = setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &flag, sizeof(int));
                                DIE(result < 0, "error disable nagle");

                                struct Queue_Message *queue = clients->client.queue_message;
                                //Transmit mesajele salvate daca exista
                                if(queue != NULL){
                                    while(queue->front){
                                        UDP_Response response;
                                        memset(&response, 0, sizeof(UDP_Response));
                                        
                                        memcpy(&response.topic, &queue->front->item.topic, 51);
                                        memcpy(&response.type, &queue->front->item.type, 1);
                                        memcpy(&response.payload, &queue->front->item.payload, 1501);
                                        memcpy(&response.ip, inet_ntoa(udp_serv_addr.sin_addr), 20);
                                        response.port = ntohs(udp_serv_addr.sin_port);

                                        n = send(newsockfd, &response, sizeof(UDP_Response), 0);
                                        DIE(n < 0, "send error message");

                                        dequeue(queue);
                                    }
                                }

                                FD_SET(newsockfd, &read_fds);
                                if(newsockfd > fdmax){
                                    fdmax = newsockfd;
                                }
                                event = 1;    
                            }
                            clients = clients->next;
                        }
                        //Daca am trecut de cazurile precedente inseamna ca se conecteaza un client
                        //cu ID unic
                        if(event != 1){
                            
                            printf("New client %s connected from %s:%d.\n", name.topic, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                            
                            Client_Info client;
                            client.socket = newsockfd;
                            memset(client.id, 0, sizeof(client.id));
                            memcpy(client.id, name.topic, strlen(name.topic));
                            
                            client.status = 1;
                            client.queue_message = (Queue_Message*)malloc(sizeof(Queue_Message));
                            client.queue_message->front = NULL;
                            client.queue_message->rear = NULL;
                            client.topics = NULL;

                            //Adaug in lista de clienti
                            add_client(&clientList, client);
                            FD_SET(newsockfd, &read_fds);

                            int result = setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &flag, sizeof(int));
                            DIE(result < 0, "error disable nagle");

                            if(newsockfd > fdmax){
                                fdmax = newsockfd;
                            }
                        }  
                    }
                    else{
                        struct TCP_Command stdin_command;
                        memset(&stdin_command, 0, sizeof(TCP_Command));

                        protocol_tcp(tcp_buffer, newsockfd, q);
                        memcpy(&stdin_command, q->front, sizeof(TCP_Command));
                        dequeue_tcp(q);
                        
                        //Daca nu am primit date de la client acesta este deconectat
                        if(stdin_command.SF == 10 || stdin_command.type == 10){
                            struct Client_List* client = clientList;
                            while(client){
                                if(client->client.socket == i){
                                    client->client.status = 0;
                                    printf("Client %s disconnected.\n", client->client.id);
                                    client->client.socket = -1;
                                    break;
                                }
                                client = client->next;
                            }
                            FD_CLR(i, &read_fds);
                            close(i);
                            continue;
                        }
                        //Am primit comanda de subscribe
                        if(stdin_command.type == 1){
                            Client_List* cli = findClient(clientList, i);
                            Topic newtopic;
                            memset(&newtopic.name, 0, sizeof(newtopic.name));
                            newtopic.sf = stdin_command.SF;
                            memcpy(&newtopic.name, &stdin_command.topic, 50);
                            add_topic(&cli->client.topics, newtopic);
                        }
                        //Am primit comanda de unsubscribe
                        else{
                            Client_List* cli = findClient(clientList, i);
                            remove_topic(cli->client.topics, stdin_command.topic);
                        }
                        
                    }
                }
            }
        }
    }
    destroy_queue_tcp(q);
    freeClientList(clientList);
    close(tcp_sockfd);
    close(udp_sockfd);

    return 0;
}