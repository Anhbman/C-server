//
// Created by bumman on 21/12/2021.
//

#include "include/Register.h"

int Register(int sockfd, PGconn *conn) {

    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");

    char* query = (char*)malloc(250*sizeof(char));
    char* username = (char*)malloc(250*sizeof(char));
    char* password = (char*)malloc(250*sizeof(char));
    char* query_check = (char*)malloc(250*sizeof(char));
    
    //strcpy(query,"INSERT INTO public.taikhoan(user_name,password) VALUES ");
    buff[bytes_received] = '\0';
    printf("%s",buff);
    char* token;
    token = strtok(buff,"|");
    strcpy(username,token);
    token = strtok(NULL,"|");
    strcpy(password,token);
    printf("recv: %s\n",buff);
    strcat(query, buff);

    sprintf(query,"INSERT INTO public.taikhoan VALUES (nextval('user_id'),'%s','%s')",username,password);
    sprintf(query_check,"SELECT * FROM public.taikhoan where user_name = '%s'",username);
    PGresult *res = PQexec(conn, query_check);
    printf("query: %s\n",query_check);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        bytes_sent = send(sockfd, REGISTER_FAIL, BUFF_SIZE, 0); /* echo to the client */
        return 0;
        // PQclear(res);
        // do_exit(conn);
    }
    if (PQntuples(res)==0) {
        res = PQexec(conn,query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("No data retrieved1\n");
            printf("%s\n",PQresultErrorMessage(res));
            bytes_sent = send(sockfd,REGISTER_FAIL,BUFF_SIZE,0);
            return 0;
        }
        bytes_sent = send(sockfd,REGISTER_SUCCESS,BUFF_SIZE,0);
        if (bytes_sent < 0)
        perror("\nError: ");
        return 1;
    }

    
    
    bytes_sent = send(sockfd, USER_EXIST, BUFF_SIZE, 0); /* echo to the client */
    return 0;
}