//
// Created by bumman on 21/12/2021.
//

#include "include/Login.h"

int Login(int sockfd, PGconn *conn) {

    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");

    char* username = (char*)malloc(250*sizeof(char));

    // strcpy(username,"SELECT * FROM public.taikhoan where ");
    buff[bytes_received] = '\0';

    printf("recv: %s\n",buff);

    char* tk = strtok(buff,"|");
    char* mk = strtok(NULL,"|");
    // strcat(username, buff);

    sprintf(username,"SELECT * FROM public.taikhoan where user_name = '%s' and password = '%s' ",tk, mk);
    PGresult *res = PQexec(conn, username);
    printf("buff: %s\n",username);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        bytes_sent = send(sockfd, "s1", 2, 0); /* echo to the client */
        return 0;
        // PQclear(res);
        // do_exit(conn);
    }

    int rec_count = PQntuples(res);
    printf("count: %d\n",rec_count);
    if (rec_count == 0)
    {
        bytes_sent = send(sockfd, "s1", BUFF_SIZE, 0); /* echo to the client */
        return 0;
    }

    bytes_sent = send(sockfd, "s0", BUFF_SIZE, 0); /* echo to the client */
    if (bytes_sent < 0)
        perror("\nError: ");
    return 1;
}