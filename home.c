#include "include/home.h"

int getNumberCate(PGconn* conn) {
    char* query = (char*)malloc(sizeof(char)*BUFF_SIZE);

    sprintf(query, "select count(*) from public.\"Category\";");

    PGresult *res = PQexec(conn, query);

    free(query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        return 0;
    }

    return atoi(PQgetvalue(res, 0, 0));
}

char* getNameCate(int categoryID ,PGconn* conn) {
    char* query = (char*) malloc(sizeof(char)*BUFF_SIZE);

    sprintf(query,"select category_name from public.\"Category\" c where c.category_id = %d",categoryID);

    PGresult *res = PQexec(conn, query);

    free(query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        return NULL;
    }
    
    return (PQgetvalue(res, 0, 0));
}

void home(int sockfd, PGconn *conn) {
    
    
    char* query = (char*) malloc(sizeof(char)*BUFF_SIZE);
    char* value = (char*) malloc(sizeof(char)*BUFF_SIZE);

    sprintf(query,"select category_name,STRING_AGG (a.address, '|') address from public.\"FavoriteAddress\" f, public.\"Category\" c, public.\"Address\" a where f.user_id = 1 and f.address_id = a.address_id and a.category_id = c.category_id group by category_name;");

    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));
        return ;
    }
   
	int rec_count = PQntuples(res);
    for (int row=0; row<rec_count; row++) {
		
        bzero(value,0);
		strcpy(value,PQgetvalue(res, row, 0));
		strcat(value,"|");
        strcat(value, PQgetvalue(res, row, 1));
        int bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
        if (bytes_sent < 0)
            perror("\nError: ");
        printf("value: %s\n", value);
	}

    int bytes_sent = send(sockfd, SEND_END, BUFF_SIZE, 0); /* echo to the client */
    if (bytes_sent < 0)
        perror("\nError: ");
	//printf("value: %s\n",value);

    free(query);
    free(value);
}

void show_page_data (int sockfd, PGconn *conn) {
     char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;
    
    char* username = (char *)malloc(sizeof(char)*BUFF_SIZE);

    strcpy(username, buff);
    
    int getUserid = userID(username, conn);

    printf("UserID: %d\n",getUserid);
    char* queryPlace = (char*)malloc(sizeof(char)*BUFF_SIZE);

    sprintf(queryPlace,"select category_name,STRING_AGG (a.address, '|') address from public.\"Category\" c, public.\"Address\" a where a.category_id = c.category_id and a.address_id not in (select f.address_id from public.\"FavoriteAddress\" f where f.user_id = %d ) group by category_name;", getUserid);

    PGresult *res = PQexec(conn, queryPlace);

    //memset(Address,0,BUFF_SIZE);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    

    char* value = (char*) malloc(sizeof(char)*BUFF_SIZE);
    int rec_count = PQntuples(res);
    for (int row=0; row<rec_count; row++) {
		
        bzero(value,0);
		strcpy(value,PQgetvalue(res, row, 0));
		strcat(value,"|");
        strcat(value, PQgetvalue(res, row, 1));
        int bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
        if (bytes_sent < 0)
            perror("\nError: ");
        printf("value: %s\n", value);
	}

    bytes_sent = send(sockfd, SEND_END, BUFF_SIZE, 0); /* echo to the client */
    if (bytes_sent < 0)
        perror("\nError: ");

    free(queryPlace);
    free(value);
    free(username);
}

void remove_place (int sockfd, PGconn *conn ) {
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;
    
    char* username = (char *)malloc(sizeof(char)*BUFF_SIZE);

    strcpy(username, buff);

     bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;
    
    int getUserid = userID(username, conn);

    printf("UserID: %d\n",getUserid);
    char* queryPlace = (char*)malloc(sizeof(char)*BUFF_SIZE);
    int placeID = getPlaceID(sockfd,conn,buff);

    sprintf(queryPlace,"DELETE FROM public.\"FavoriteAddress\" WHERE user_id = %d and address_id = %d;", getUserid, placeID);

    printf("quety: %d", queryPlace);

    PGresult *res = PQexec(conn, queryPlace);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi delete pacle\n");
            //do_exit(conn, res);    
            send(sockfd, REMOVE_PLACE_ERROR,BUFF_SIZE,0);
    }
    else {
        send(sockfd, REMOVE_PLACE_OK,BUFF_SIZE,0);
    }

}