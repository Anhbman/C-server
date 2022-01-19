#ifndef C_SERVER_HOME_H
#define C_SERVER_HOME_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "FuncConstant.h"
#include "constant.h"
#include "helper.h"

void home(int sockfd, PGconn *conn);
void show_page_data(int sockfd, PGconn *conn);
void remove_place (int sockfd, PGconn *conn );
void restore (int sockfd, PGconn *conn);
void backup (int sockfd, PGconn *conn);

#endif //C_SERVER_HOME_H