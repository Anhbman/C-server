#include"include/SharePlace.h"
#include "include/helper.h"

void showPlaceUser(int sockfd,PGconn *conn) {
   char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");

    char* query = (char*)malloc(250*sizeof(char));
    
    
    //strcpy(query,"INSERT INTO public.taikhoan(user_name,password) VALUES ");
    buff[bytes_received] = '\0';
    printf("%s",buff);
    

    sprintf(query,"select c.address from public.taikhoan a,public.\"FavoriteAddress\" b,public.\"Address\" c where a.user_name = '%s' and a.user_id = b.user_id and b.address_id = c.address_id",buff);
   // sprintf(query_check,"SELECT * FROM public.taikhoan where user_name = '%s'",username);
    PGresult *res = PQexec(conn, query);
    printf("%s\n",query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("data no retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        bytes_sent = send(sockfd,"161",BUFF_SIZE,0);
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
    free(query);
    return ;
}
void sharePlace(int sockfd,PGconn *conn) {
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");

    char* query = (char*)malloc(250*sizeof(char));
    char* friendname = (char*)malloc(250*sizeof(char));
    char* username = (char*)malloc(250*sizeof(char));
    char* placename = (char*)malloc(250*sizeof(char));
    char* query_check = (char*)malloc(250*sizeof(char));
    char* cate = (char*)malloc(250*sizeof(char));
    
    //strcpy(query,"INSERT INTO public.taikhoan(user_name,password) VALUES ");
    buff[bytes_received] = '\0';
    printf("%s",buff);
    char* token;
    token = strtok(buff,"|");
    strcpy(username,token);
    token = strtok(NULL,"|");
    strcpy(placename,token);
    token = strtok(NULL,"|");
    strcpy(cate,token);
    token = strtok(NULL,"|");
    strcpy(friendname,token);
    int UserID = userID(username,conn);
    int FriendID = userID(friendname,conn);
    int cateID = getCategoryID(conn, cate);
    int PlaceID = getPlaceID(sockfd,conn,placename, cateID);
    printf("friend id: %d\n",FriendID);

    sprintf(query,"insert into public.\"FavoriteAddress\" values (%d,%d,%d)",FriendID,PlaceID,UserID);
    sprintf(query_check,"select * from public.\"FavoriteAddress\" where user_id = %d and address_id = %d",FriendID,PlaceID);
    PGresult * res_check = PQexec(conn,query_check);
    if (PQntuples(res_check) != 0) {
        bytes_sent = send(sockfd,"202",BUFF_SIZE,0);
        free(query);
        free(query_check);
        free(friendname);
        free(placename);
        free(username);
        
        return;
    }
    PGresult *res = PQexec(conn,query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("No data retrieved1\n");
            printf("%s\n",PQresultErrorMessage(res));
            bytes_sent = send(sockfd,"201",BUFF_SIZE,0);
            return ;
    }
    bytes_sent = send(sockfd,"200",BUFF_SIZE,0);
    if (bytes_sent < 0)
    perror("\nError: ");
    free(query);
    free(username);
    free(friendname);
    free(placename);
    return ;
}
