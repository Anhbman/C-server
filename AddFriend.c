
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
        return 0;
    }
    sprintf(query,"INSERT INTO public.\"Friend\" VALUES (%d,%d,0)",UserID,FriendID);
    sprintf(query_check,"SELECT * FROM public.\"Friend\" WHERE user_id1 = %d AND user_id2 = %d",UserID,FriendID);
    
    printf("%s\n",query);
    PGresult *res = PQexec(conn,query_check);
    if (PQntuples(res) == 0) {
        res = PQexec(conn,query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Data no retrieved\n ");
            printf("%s\n",PQresultErrorMessage(res));
            bytes_sent = send(sockfd,"151",3,0);
            return 0;
        }
        bytes_sent = send(sockfd,"150",3,0);
        return 1;
    } else {
        printf("Data exsist\n");
        bytes_sent = send(sockfd,"151",3,0);
        return 0;
    }

}
