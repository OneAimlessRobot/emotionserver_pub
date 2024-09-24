#include "../../Includes/preprocessor.h"
#include "../../extra_funcs/Includes/auxfuncs.h"
#include "../../extra_funcs/Includes/fileshit.h"
#include "../../extra_funcs/Includes/sockio.h"
#include "../Includes/connection.h"
#include "../Includes/engine.h"

static connection con_obj;

//retorna 0 se o objecto tava vazio. 1 se n tava


static void fill_con(char* file_path,int sockfd,int fp){

	memset(con_obj.file_path,0,PATHSIZE);
        strncpy(con_obj.file_path,file_path,min(PATHSIZE, strlen(file_path)));
        con_obj.is_on=1;
	con_obj.sockfd=sockfd;
	con_obj.data_size=DEF_DATASIZE;
	con_obj.fp=fp;


}

static void sigint_handler(int useless){
	
	printf("Recebemos sinal dentro de um processo de conexão. Sinal: %d\n",useless);
	con_obj.is_on=0;
	close(con_obj.sockfd);

}


static void con_loop(void){


	sendallfd(con_obj.sockfd,con_obj.fp,SERVER_DATA_TIMES_PAIR);




}


void con_go(char* file_path,int sockfd,int fp){
	
	fill_con(file_path,sockfd,fp);
	
	signal(SIGINT, sigint_handler);
	signal(SIGPIPE, sigint_handler);
	con_loop();
}

