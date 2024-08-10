#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <math.h>
#include <netdb.h>
#include "helpers.h"
#include "data.h"
#include "tcp_protocol_response.h"

using namespace std;

void usage(char *file)
{
	fprintf(stderr, "Usage: %s id_client ip_server port_server\n", file);
	exit(0);
}


int main(int argc, char *argv[]){
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	//Initializari
    int sockfd, n, ret, fdmax;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];
	char udp_buffer[LENGTH2];
	Queue_UDP* q = createQueue_udp();
    memset(udp_buffer, 0, LENGTH2);
	int current_len = 0;
	fd_set read_fds, tmp_fds;

	if (argc < 4) {
		usage(argv[0]);
	}

    DIE(strlen(argv[1]) > 10, "invalid id"); 

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	FD_SET(STDIN_FILENO, &read_fds);
	FD_SET(sockfd, &read_fds);

    if(STDIN_FILENO > sockfd) fdmax = STDIN_FILENO;
    else fdmax = sockfd;

	int enable = 1;
    ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(int));
	DIE(ret < 0, "nagle error");
	
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    DIE(ret< 0, "port");
	
	//Trimit mesaj cu ID-ul subscriber-ului
	TCP_Command send_name;
	memset(&send_name, 0, sizeof(TCP_Command));
	send_name.SF = 2;
	send_name.type = 2;
	memcpy(&send_name.topic, argv[1], strlen(argv[1]));
	
	n = send(sockfd, &send_name, sizeof(TCP_Command), 0);
	DIE(n < 0, "id error");

    while(1){
		int break_point = 0;
        tmp_fds = read_fds;
		
		char* stdin_command = (char*)calloc(13, sizeof(char));
		char* topic = (char*)calloc(51, sizeof(char));
		char* sf = (char*)calloc(2, sizeof(char));
		stdin_command[12] = '\0';
		topic[50] = '\0';
		sf[1] = '\0';
		
        ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");
		
		//Daca se primesc date de la STDIN
		if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
		
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);
			//Opresc executia subscriber-ului
			if (strncmp(buffer, "exit", 4) == 0) {
                break;
			}

            //Trimit mesaj de subscribe
            if(strncmp(buffer, "subscribe", 9) == 0){
				TCP_Command command;
				memset(&command, 0, sizeof(TCP_Command));
				sscanf(buffer, "%9s %s %s", stdin_command, topic, sf);

				DIE(strlen(topic) > 50, "topic length");
				DIE(sf[0] != '0' && sf[0] != '1', "invalid sf");

				int a = atoi(sf);
				command.SF = (uint8_t)a;
				command.type = 1;
				memcpy(&command.topic, topic, strlen(topic));

				n = send(sockfd, &command, sizeof(TCP_Command), 0);
				DIE(n < 0, "subscribe failed");
				fprintf(stdout, "Subscribed to topic.\n");
			}

            //Trimit mesaj de unsubscribe
			if(strncmp(buffer, "unsubscribe", 11) == 0){
				TCP_Command command;
				memset(&command, 0, sizeof(TCP_Command));
				sscanf(buffer, "%11s %s", stdin_command, topic);
			
				command.SF = 1;
				command.type = 0;
				memcpy(&command.topic, topic, strlen(topic));

				n = send(sockfd, &command, sizeof(TCP_Command), 0);
				DIE(n < 0, "unsubscribe failed");
				fprintf(stdout, "Unsubscribed from topic.\n");
			}
		}
		free(stdin_command);
		free(topic);
		free(sf);

		//Primesc date de la server
		if (FD_ISSET(sockfd, &tmp_fds)) {
			//Primesc date si le organizez intr-o coada
			protocol_udp(udp_buffer, sockfd, q, &current_len);
			while(1){
				//Cat timp nu e goala coada
				if(size(q) == 0){
					break;
				}
				UDP_Response response;
				memset(&response, 0, sizeof(UDP_Response));
				memcpy(&response, &q->front->item, sizeof(UDP_Response));
				dequeue_udp(q);
				
				if(strncmp(response.topic, "error", 5) == 0){
					break;
				}

				//Daca primesc mesaj de inchidere
				if(response.type == 10){
					break_point = 1;
				}

				//Daca nu primesc mesaj de inchidere afisez mesajele primite
				if(response.type != 10){
					switch(response.type){
						case 0:
						{ 
						int int_number;
						uint32_t number = ntohl((*(uint32_t*)(response.payload + 1)));
						if(response.payload[0] == 1){
							int_number = -number;
						}
						else int_number = number;
						printf("%s - INT - %d\n", response.topic, int_number);
						}
						break;

						case 1:
						{
						double short_real_number;
						uint16_t numb = ntohs(*(uint16_t*)response.payload); 
						short_real_number = numb / (double)100;
						printf("%s - SHORT_REAL - %.2f\n", response.topic, short_real_number);
						}
						break;

						case 2:
						{
						float float_number;
						uint32_t number = ntohl((*(uint32_t*)(response.payload + 1)));
						uint8_t shift = *(uint8_t*)(response.payload + 5);
						float_number = number/pow(10,shift);
						if(response.payload[0] == 1){
							float_number *= -1;
						}
						printf("%s - FLOAT - %lf\n", response.topic, float_number);
						}
						break;

						case 3:
						{
						printf("%s - STRING - %s\n", response.topic, response.payload);
						}
						break;

						default:
						break;
					}
				}
				else{
					continue;
				}
			}	
		}
		if(break_point == 1){
			break;
		}
	}
	//Inchidere socket si dealocarea cozii
	destroy_queue_udp(q);
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    
	return 0;
}