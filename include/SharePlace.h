#ifndef C_SERVER_SHAREPLACE_H
#define C_SERVER_SHAREPLACE_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "FuncConstant.h"
#include "constant.h"

void showPlaceUser(int sockfd,PGconn *conn);
void sharePlace(int sockfd,PGconn *conn);
#endif //C_SERVER_LOGIN_H