#ifndef C_SERVER_HELPER_H
#define C_SERVER_HELPER_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include "stdlib.h"
#include "FuncConstant.h"
#include <sys/socket.h>


int userID (char* name, PGconn *conn);
void showFriend(int sockfd, char* name,PGconn *conn);
int getPlaceID (int sockfd,PGconn *conn, char* namePlace, int cate);
int getCategoryID (PGconn *conn, char* cate);
char* getuserName (int sockfd,PGconn *conn, char* name);

#endif //C_SERVER_HELPER_H