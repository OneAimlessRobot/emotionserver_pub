#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEFAULT_OK_STRING "bring_it"
#define MUSIC_CLIENT_OUTPUT_PATH "/songs_out/"
#define MUSIC_SERVER_INPUT_PATH "/songs_in/"
#define MAX_CLIENTS_HARD_LIMIT 1000
#define SIG_STOPSERVER 19199
#define PATHSIZE 20000
#define DEF_DATASIZE 1024
#define MAX_DATASIZE 100000
#define MAX_TRIES 10
#define BUFFSIZE 1024
