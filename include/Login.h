//
// Created by bumman on 21/12/2021.
//

#ifndef C_SERVER_LOGIN_H
#define C_SERVER_LOGIN_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "FuncConstant.h"
#include "constant.h"

int Login(int sockfd, PGconn *conn);

#endif //C_SERVER_LOGIN_H
