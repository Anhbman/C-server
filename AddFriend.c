
#include "include/AddFriend.h"
#include "include/helper.h"

int Addfriend(int sockfd, PGconn *conn) {
	char buff[BUFF_SIZE];
	int bytes_sent,bytes_received;
	bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");	
    char* query = (char*)malloc(250*sizeof(char));
    char* username = (char*)malloc(250*sizeof(char));
    char* friendname = (char*)malloc(250*sizeof(char));
    char* query_check = (char*)malloc(250*sizeof(char));
    char* query_check1 = (char*)malloc(250*sizeof(char));

    buff[bytes_received] = '\0';
    printf("%s\n",buff);
    char* token;
    token = strtok(buff,"|");
    strcpy(username,token);
    token = strtok(NULL,"|");
    strcpy(friendname,token);
    int UserID = userID(username,conn);
    int FriendID = userID(friendname,conn);
    printf("%d\n",FriendID);
    if (FriendID == -1) {
        bytes_sent = send(sockfd,"151",3,0);
        free(query);
        free(username);
        free(friendname);
        free(query_check);
        free(query_check1);
        return 0;
    }
    sprintf(query,"INSERT INTO public.\"Friend\" VALUES (%d,%d,0)",UserID,FriendID);
    sprintf(query_check,"SELECT * FROM public.\"Friend\" WHERE user_id1 = %d AND user_id2 = %d",UserID,FriendID);
    sprintf(query_check1,"SELECT * FROM public.\"Friend\" WHERE user_id1 = %d AND user_id2 = %d",FriendID,UserID);
    printf("%s\n",query);
    PGresult *res = PQexec(conn,query_check);
    PGresult *res1 = PQexec(conn,query_check1);
    if (PQntuples(res) == 0 && PQntuples(res1) == 0) {
        res = PQexec(conn,query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Data no retrieved\n ");
            printf("%s\n",PQresultErrorMessage(res));
            bytes_sent = send(sockfd,"151",3,0);
            free(query);
            free(username);
            free(friendname);
            free(query_check);
            free(query_check1);
            return 0;
        }
        bytes_sent = send(sockfd,"150",3,0);
        free(query);
        free(username);
        free(friendname);
        free(query_check);
        free(query_check1);
            
        return 1;
    } else {
        printf("Data exsist\n");
        bytes_sent = send(sockfd,"151",3,0);
        free(query);
        free(username);
        free(friendname);
        free(query_check);
        free(query_check1);
            
        return 0;
    }
}
void showFriendList(int sockfd,PGconn *conn) {
    char buff[BUFF_SIZE];
    int bytes_sent,bytes_received;
    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n"); 
    buff[bytes_received] = '\0';
    printf("%s\n",buff);
    char* query = (char*)malloc(250*sizeof(char));
    char* query1 = (char*)malloc(250*sizeof(char));
    
    sprintf(query,"SELECT c.user_name FROM public.taikhoan a,public.\"Friend\" b,public.taikhoan c WHERE a.user_name = '%s' and a.user_id = b.user_id1 and b.user_id2 = c.user_id",buff);
    sprintf(query1,"SELECT c.user_name FROM public.taikhoan a,public.\"Friend\" b,public.taikhoan c WHERE a.user_name = '%s' and a.user_id = b.user_id2 and b.user_id1 = c.user_id",buff);

    PGresult *res = PQexec(conn,query);
    PGresult *res1 = PQexec(conn,query1);
    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQresultStatus(res1)!=PGRES_TUPLES_OK) {
        printf("data no retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        bytes_sent = send(sockfd,"161",3,0);
        return ;
    }
    char value[BUFF_SIZE] = {0};
    int count = PQntuples(res);
    for (int i=0; i<count ; i++) {
        strcat(value,PQgetvalue(res,i,0));
        strcat(value,"|");
    }
    count = PQntuples(res1);
    for (int i=0; i<count ; i++) {
        strcat(value,PQgetvalue(res1,i,0));
        strcat(value,"|");
    }

    printf("value: %s\n",value);
    bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
    bytes_sent = send(sockfd,SEND_END,BUFF_SIZE,0);
    if (bytes_sent < 0)
        perror("\nError: "); 
}
void showUserList(int sockfd,PGconn *conn) {
    char buff[BUFF_SIZE];
    int bytes_sent,bytes_received;
    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n"); 
    buff[bytes_received] = '\0';
    printf("%s\n",buff);

    char* query = (char*)malloc(250*sizeof(char));
    sprintf(query,"SELECT a.user_name FROM public.taikhoan a WHERE a.user_name != '%s'",buff);

    PGresult *res = PQexec(conn,query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("data no retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        bytes_sent = send(sockfd,"161",3,0);
        return ;
    }
    char value[BUFF_SIZE] = {0};
    int count = PQntuples(res);
    for (int i=0; i<count ; i++) {
        strcat(value,PQgetvalue(res,i,0));
        strcat(value,"|");
    }
    printf("value: %s\n",value);
    bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
    bytes_sent = send(sockfd,SEND_END,BUFF_SIZE,0);
    if (bytes_sent < 0)
        perror("\nError: "); 
}
void showFriendRequest(int sockfd,PGconn *conn) {
    char buff[BUFF_SIZE];
    int bytes_sent,bytes_received;
    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n"); 
    buff[bytes_received] = '\0';
    printf("%s\n",buff);

    char* query = (char*)malloc(250*sizeof(char));
    sprintf(query,"SELECT c.user_name FROM public.taikhoan a,public.\"Friend\" b,public.taikhoan c WHERE a.user_name = '%s' and a.user_id = b.user_id2 and b.user_id1 = c.user_id and b.accept = 0",buff);

    PGresult *res = PQexec(conn,query);
    //PGresult *res = PQexec(conn,query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("data no retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        bytes_sent = send(sockfd,"161",3,0);
        return ;
    }
    char value[BUFF_SIZE] = {0};
    int count = PQntuples(res);
    for (int i=0; i<count ; i++) {
        strcat(value,PQgetvalue(res,i,0));
        strcat(value,"|");
    }
    printf("value: %s\n",value);
    bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
    bytes_sent = send(sockfd,SEND_END,BUFF_SIZE,0);
    if (bytes_sent < 0)
        perror("\nError: "); 

}
