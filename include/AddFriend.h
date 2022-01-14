#ifndef C_SERVER_ADDFRIEND_H
#define C_SERVER_ADDFRIEND_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "FuncConstant.h"
#include "constant.h"

int Addfriend(int sockfd, PGconn *conn);
void showFriendList(int sockfd,PGconn *conn);
void showUserList(int sockfd,PGconn *conn);
void showFriendRequest(int sockfd,PGconn *conn);
void acceptFriend(int sockfd,PGconn *conn);
#endif //C_SERVER_LOGIN_H