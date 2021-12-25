//
// Created by bumman on 21/12/2021.
//

#ifndef C_SERVER_ADDPLACE_H
#define C_SERVER_ADDPLACE_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include "stdlib.h"
#include "FuncConstant.h"
#include "helper.h"
#include <sys/socket.h>


int addAddress (int sockfd, PGconn *conn);
void addPlace(int sockfd, PGconn *conn);
void showPlace (int sockfd, PGconn *conn);

#endif //C_SERVER_ADDPLACE_H
