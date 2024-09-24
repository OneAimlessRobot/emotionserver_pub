#ifndef PROTOCOL_H
#define PROTOCOL_H


typedef enum req_type{GET,PEEK,NA}req_type;

//todas as strings sao terminadas a 0
req_type str_to_req_type(char* str);


//conteudos sao completamente reescritos
void req_type_to_str(req_type type,char buff[BUFFSIZE]);







#endif
