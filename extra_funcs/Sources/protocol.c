#include "../../Includes/preprocessor.h"
#include "../Includes/auxfuncs.h"
#include "../Includes/protocol.h"
#include "../Includes/fileshit.h"


req_type str_to_req_type(char* str){

	
	printf("Recebemos conexap do tipo: |%s|\n",str);
	if(!strs_are_strictly_equal(str,"get")){

		return GET;

	}
	if(!strs_are_strictly_equal(str,"peek")){

		return PEEK;

	}

	return NA;

}
void req_type_to_str(req_type type,char buff[BUFFSIZE]){

	memset(buff,0,BUFFSIZE);
	switch(type){

		case GET:
			snprintf(buff, BUFFSIZE, "get");
		break;
		case PEEK:
			snprintf(buff, BUFFSIZE, "peek");
		break;
		default:
			snprintf(buff, BUFFSIZE, "NONE");
		break;
	


	}
}
