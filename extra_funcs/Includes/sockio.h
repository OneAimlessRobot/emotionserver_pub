#ifndef SOCKIO_H
#define SOCKIO_H


typedef const int int_pair[2];

int readall(int sock,char* buff,int_pair times);

int sendsome(int sd,char buff[],u_int64_t size,int_pair times);

int sendallfd(int sock,int fd,int_pair times);

int readsome(int sd,char buff[],u_int64_t size,int_pair times);

int readalltofd(int sock,int fd,int_pair times);
#define SERVER_DATA_TIMES_PAIR ((int_pair){SERVER_TIMEOUT_DATA_SEC,SERVER_TIMEOUT_DATA_USEC})
#define CLIENT_DATA_TIMES_PAIR ((int_pair){CLIENT_TIMEOUT_DATA_SEC,CLIENT_TIMEOUT_DATA_USEC})




#endif
