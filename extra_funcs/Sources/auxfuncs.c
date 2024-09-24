#include "../../Includes/preprocessor.h"
#include "../Includes/auxfuncs.h"


int strs_are_strictly_equal(char* str1, char* str2){

	int max_size= max(strlen(str1),strlen(str2));
	if((strnlen(str1,max_size)-strnlen(str2,max_size))){

		return -2;

	}


	return strncmp(str1,str2,max_size);


}

int min(int first,int second){


	if(first<second){


		return first;
	}

	else{

		return second;
	}


}

void swap(void* first,void* second,int sz_bytes){

	void* tmp = malloc(sz_bytes);

	memcpy(tmp,first,sz_bytes);

	memcpy(first,second,sz_bytes);

	memcpy(second,tmp,sz_bytes);

}

int max(int first,int second){


	if(first>second){


		return first;
	}

	else{

		return second;
	}


}
