#include "../../Includes/preprocessor.h"
#include "../Includes/sockio.h"
#include "../../client/Includes/client.h"
#include "../Includes/fileshit.h"


int sendsome(int sd,char buff[],u_int64_t size,int_pair times){
                int iResult;
                struct timeval tv;
                fd_set wfds;
                FD_ZERO(&wfds);
                FD_SET(sd,&wfds);
                tv.tv_sec=times[0];
                tv.tv_usec=times[1];
                iResult=select(sd+1,(fd_set*)0,&wfds,(fd_set*)0,&tv);
                if(iResult>0){

                return send(sd,buff,size,0);
                }
		else if(!iResult){
               	return -2;
		}
		else{
		if(logging){

		fprintf(logstream, "SELECT ERROR!!!!! SEND\n");
		}
		return -1;
		}
}


int sendallfd(int sock,int fd,int_pair times){

char buff[BUFFSIZE];
int numread;
int sent=0;
while ((numread = read(fd,buff,BUFFSIZE)) > 0) {
    
    int totalsent = 0;
    while (totalsent < numread) {
        errno=0;
	sent = sendsome(sock, buff + totalsent,  numread - totalsent,times);
	if(sent==-2){

		if(logging){
		fprintf(logstream,"Timeout no sending!!!!: %s\nsocket %d\n",strerror(errno),sock);
                }
		continue;
	}
	if(sent<0){
	
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                if(logging){
		fprintf(logstream,"Block no sending!!!!: %s\nsocket %d\n",strerror(errno),sock);
                }
		break;

        }
	else if(errno==EPIPE){

		if(logging){
		fprintf(logstream,"Pipe partido!!! A socket e %d\n",sock);
		}
		raise(SIGINT);
		return -1;
	}
        else if(errno == ECONNRESET){
		if(logging){
                fprintf(logstream,"Conexão largada!!\nSIGPIPE!!!!!: %s\n",strerror(errno));
                }
		raise(SIGINT);
		return -1;
	}
	else {
		if(logging){
                fprintf(logstream,"Outro erro qualquer!!!!!: %d %s\n",errno,strerror(errno));
                }
	
		break;
	}
        }
	else{
	if(logging){
	fprintf(logstream,"send de %d bytes feito!!!!!\n",sent);
	}
	totalsent += sent;
    	}
	}
}

return 0;
}


int readsome(int sd,char buff[],u_int64_t size,int_pair times){
		int iResult;
                struct timeval tv;
                fd_set rfds;
                FD_ZERO(&rfds);
                FD_SET(sd,&rfds);
                tv.tv_sec=times[0];
                tv.tv_usec=times[1];
                iResult=select(sd+1,&rfds,(fd_set*)0,(fd_set*)0,&tv);
                if(iResult>0){

                return recv(sd,buff,size,0);

                }
		else if(!iResult){
               	return -2;
		}
		else{
		if(logging){

		fprintf(logstream, "SELECT ERROR!!!!! READ\n");
		}
		return -1;
		}
}


int readall(int sock,char buff[],int_pair times){
        int64_t len=1;

while(1){
        len=readsome(sock,buff,DEF_DATASIZE,times);
	if(len<=0){
	
                break;
	
	}

}
	
	if(len<=0){
	if (errno == EAGAIN || errno == EWOULDBLOCK) {
        	if(logging){
		fprintf(logstream,"readall bem sucedido!! A socket e %d\n",sock);
		}
	}
	else if(errno==EPIPE){

		if(logging){
		fprintf(logstream,"Pipe partido!!! A socket e %d\n",sock);
		}
		return -2;
	}
	else if(errno==ENOTCONN){
		if(logging){
		fprintf(logstream,"readall saiu com erro!!!!!:\nAvisando server para desconectar!\n%s\n",strerror(errno));
		}
		
		return -2;
	}
	else if(len!=-2){
		if(logging){
		fprintf(logstream,"readall saiu com erro!!!!!:\n%s\n",strerror(errno));
		}
	}
	
	}
        
        return 0;

}

int readalltofd(int sock,int fd,int_pair times){
        int64_t len=1;
	char buff[BUFFSIZE];
	int64_t buffsize= sizeof(buff);
	while(1){
		len=readsome(sock,buff,DEF_DATASIZE,times);
		if(len<=0){
			break;	
		}

		write(fd,buff,len);
		memset(buff,0,buffsize);
	}
	if(len<0){
	if (errno == EAGAIN || errno == EWOULDBLOCK) {
        	if(logging){
		fprintf(logstream,"readall bem sucedido!! A socket e %d\n",sock);
		}
	}
	else if(errno==EPIPE){

		if(logging){
		fprintf(logstream,"Pipe partido!!! A socket e %d\n",sock);
		}
		return -2;
	}
	else if(errno==ENOTCONN){
		if(logging){
		fprintf(logstream,"readall saiu com erro!!!!!:\nAvisando server para desconectar!\n%s\n",strerror(errno));
		}
		
		return -2;
	}
	else if(len!=-2){
		if(logging){
		fprintf(logstream,"readall saiu com erro!!!!!:\n%s\n",strerror(errno));
		}
	}
	
	}
	
        return 0;

}


