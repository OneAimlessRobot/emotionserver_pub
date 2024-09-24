#include "../../Includes/preprocessor.h"
#include "../../extra_funcs/Includes/auxfuncs.h"
#include "../Includes/connection.h"
#include "../Includes/load_html.h"
#include "../../extra_funcs/Includes/protocol.h"
#include "../../extra_funcs/Includes/fileshit.h"
#include "../../extra_funcs/Includes/sockio.h"
#include "../Includes/engine.h"

server_state state;


static void sigint_handler(int useless){

	printf("Interrupção no modulo do server!!!! sinal %d\n",useless);
	serverStop();
}

static void con_accepting_loop(void){

	while(state.server_is_on) {
	
		
		int iResult,
			client_sock,
			pid;

		struct sockaddr_in con_from_addr;

		socklen_t socklength= sizeof(state.server_address);


                struct timeval tv;
                FD_ZERO(&state.rdfds);
                FD_SET(state.server_socket,&state.rdfds);
                tv.tv_sec=SERVER_TIMEOUT_CON_SEC;
                tv.tv_usec=SERVER_TIMEOUT_CON_USEC;
                iResult=select(state.server_socket+1,&state.rdfds,(fd_set*)0,(fd_set*)0,&tv);
                
		if(iResult>0){
                

		client_sock=accept(state.server_socket,(struct sockaddr*)(&con_from_addr),&(socklength));


                /*long flags= fcntl(client_sock,F_GETFL);
                flags |= O_NONBLOCK;
                fcntl(client_sock,F_SETFL,flags);
		*/
                printf("Accepted connection from %s\n",inet_ntoa(con_from_addr.sin_addr));
                	
		pid=fork();

		if(!pid){
			
			char in_file_name[PATHSIZE]={0},
				req_type_in_buff[BUFFSIZE]={0},
				file_path[PATHSIZE]={0};
				
			char* dir_listing_str=NULL;
			//Lendo o request do cliente
			readsome(client_sock,req_type_in_buff,BUFFSIZE,SERVER_DATA_TIMES_PAIR);
			req_type recvd_type= str_to_req_type(req_type_in_buff);
			
			//Se depois deste send...
			sendsome(client_sock,req_type_in_buff,BUFFSIZE,SERVER_DATA_TIMES_PAIR);
			//este read suceder, n importa o que recebe, o cliente recebeu autorizaçao para nos dar o filename
			readsome(client_sock,in_file_name,PATHSIZE-1,SERVER_DATA_TIMES_PAIR);
			//(Quis ler o request e o filename em transferencias diferentes)
			switch(recvd_type){

				case GET:
					snprintf(file_path,3*PATHSIZE-1,"%s%s",curr_dir,in_file_name);
					break;
				case PEEK:
					dir_listing_str=generateDirListing();
					snprintf(file_path,strlen(dir_listing_str)+1,"%s",dir_listing_str);
					free(dir_listing_str);
					break;
				default:
					break;
			}
					
			int fp;
			struct stat file_info;
			if((fp=open(file_path,O_RDONLY,0777))<0){

                        	long flags2= fcntl(fp,F_GETFL);
                        	flags2 |= O_NONBLOCK;
                        	fcntl(fp,F_SETFL,flags2);

                        	printf("Accepted connection from %s, mas ficheiro %s e invalido. Conexao sera largada...\n",inet_ntoa(con_from_addr.sin_addr),file_path);
                       		perror("Nao foi possivel abrir nada!!!!\n");
                        	
                	}
			else{
			
				char file_size_buff_out[DEF_DATASIZE+1]={0};
				//char file_size_buff_back[DEF_DATASIZE+1]={0};;
				fstat(fp,&file_info);
				
				snprintf(file_size_buff_out,DEF_DATASIZE,"%ld",file_info.st_size);
				//enviando datasize
				sendsome(client_sock,file_size_buff_out,DEF_DATASIZE,SERVER_DATA_TIMES_PAIR);
				//Se este read suceder, n importa o que recebe, significa que o cliente recebeu o size
				readsome(client_sock,file_size_buff_out,DEF_DATASIZE,SERVER_DATA_TIMES_PAIR);
				//Partiu pra bola eeeeeeeeeeeeeeee reemmaaaataaaa
				con_go(file_path,client_sock,fp);
				
				if(recvd_type==PEEK){
					deleteDirListingFile();
				}
				close(fp);
			
			}
			
			exit(-1);



		}
		else{

			if(pid<0){

				perror("Erro no fork no loop de conexoes do server!!!\n");
				exit(-1);
			}
			else{

				printf("Nova conexão!");
			
			}

		}
                }
                else{
                        printf("Timed out!!!!!( more that %ds waiting). Trying again...\n",SERVER_TIMEOUT_CON_SEC);
                     
                }
		
	}

}



void serverStop(void){

	close(state.server_socket);
	state.server_is_on=0;

}
void serverInit(u_int64_t port){


//bind the socket to the specified IP and port
	logging=1;
	logstream=stderr;
	
	memset(&state,0,sizeof(server_state));

        state.server_socket= socket(AF_INET,SOCK_STREAM,0);
        int ptr=1;
        setsockopt(state.server_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&ptr,sizeof(ptr));
        if(state.server_socket==-1){
                return;

        }

        //especificar socket;
        //fcntl(state.server_socket,F_SETFL,O_ASYNC|O_NONBLOCK);
/*      ioctl(state.server_socket,FIOASYNC,&(int){1});
        long flags= fcntl(state.server_socket,F_GETFL);
        flags |= O_NONBLOCK|O_ASYNC;
        fcntl(state.server_socket,F_SETFL,flags);
*/
        signal(SIGINT,sigint_handler);
        signal(SIGPIPE,sigint_handler);
        state.server_address.sin_family=AF_INET;
        state.server_address.sin_port= htons(port);
        //state.server_address.sin_addr.s_addr=inet_addr("192.168.18.95");
        state.server_address.sin_addr.s_addr = INADDR_ANY;
        socklen_t socklength=sizeof(state.server_address);
        bind(state.server_socket,(struct sockaddr*)(&state.server_address),socklength);

        listen(state.server_socket,MAX_CLIENTS_HARD_LIMIT);

        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&state.server_address;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        inet_ntop( AF_INET, &ipAddr, state.address_str, INET_ADDRSTRLEN );
	state.server_is_on=1;
	con_accepting_loop();
	printf("Server morreu!!!!! (Saida por SIGINT)\n");
	
}


