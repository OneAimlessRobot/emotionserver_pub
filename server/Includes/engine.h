#ifndef ENGINE_H
#define ENGINE_H


#define SERVER_TIMEOUT_CON_SEC 1
#define SERVER_TIMEOUT_CON_USEC 0

#define SERVER_TIMEOUT_DATA_SEC 1
#define SERVER_TIMEOUT_DATA_USEC 0
#define SERVER_DATA_TIMES_PAIR ((int_pair){SERVER_TIMEOUT_DATA_SEC,SERVER_TIMEOUT_DATA_USEC})


typedef struct server_state{

	char address_str[INET_ADDRSTRLEN];
        struct sockaddr_in server_address;
        u_int64_t server_is_on,port;
        int server_socket;
   	fd_set rdfds;
	u_int64_t dataSize;
	/*u_int64_t totalSent;
        u_int64_t timeActive;
        double trafficRate;
	*/
}server_state;



void serverInit(u_int64_t port);

void serverStop(void);



#endif
