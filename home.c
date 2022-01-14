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
    
    printf("Home\n");
    char* query = (char*) malloc(sizeof(char)*BUFF_SIZE);
    char* value = (char*) malloc(sizeof(char)*BUFF_SIZE);
    PGresult *res;

    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;
    printf("hihih\n");

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;

    printf("%s\n",buff);

    int userid = userID(buff,conn);

    printf("userID: %d nono\n",userid);

    int countCategory = getNumberCate(conn);

    printf("categoryID: %d\n",countCategory);


    for (int i = 0; i < countCategory; i++)
    {
        bzero(query,BUFF_SIZE);
        sprintf(query,"select distinct category_name,a.address, friend_id from public.\"FavoriteAddress\" f, public.\"Category\" c, public.\"Address\" a, taikhoan t where f.user_id = %d and f.address_id = a.address_id and a.category_id = c.category_id and c.category_id = %d",userid, i+1);

        res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");
        printf("%s\n",PQresultErrorMessage(res));

        send(sockfd, SEND_END, BUFF_SIZE, 0);
        return ;
    }
   
	int rec_count = PQntuples(res);

    printf("Count: %d\n", rec_count);
    if (rec_count == 0)
    {
        continue;
    }
    
    bzero(value,0);
    strcpy(value,PQgetvalue(res, 0, 0));
    for (int row=0; row<rec_count; row++) {

		strcat(value,"|");
        strcat(value, PQgetvalue(res, row, 1));
        // strcat(value, getuserName(sockfd, conn, PQgetvalue(res, row, 2)));
        
        char* friendID = PQgetvalue(res, row, 2);
        if (strncmp(friendID,"0",1) != 0)
        {
            printf("Friend: %s\n",friendID);
            char* name = getuserName(sockfd, conn, friendID);
            printf("NameUser: %s no\n",name);
            strcat(value, " <- ");
            strcat(value, name);
        }

	}
    printf("value: %s\n", value);
    int bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
    if (bytes_sent < 0)
        perror("\nError: ");
    }
    
    bytes_sent = send(sockfd, SEND_END, BUFF_SIZE, 0); /* echo to the client */
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

    printf("remove place");

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;
    
    char* username;
    char* place;
    char* cate;

    username = strtok(buff, "|");
    place = strtok(NULL,"|");
    cate = strtok(NULL, "|");


    
    int getUserid = userID(username, conn);
    int cateID = getCategoryID(conn,cate);

    printf("UserID: %d\n",getUserid);
    char* queryPlace = (char*)malloc(sizeof(char)*BUFF_SIZE);
    int placeID = getPlaceID(sockfd,conn,place, cateID);

    sprintf(queryPlace,"DELETE FROM public.\"FavoriteAddress\" WHERE user_id = %d and address_id = %d;", getUserid, placeID);

    printf("quety: %d", queryPlace);

    PGresult *res = PQexec(conn, queryPlace);

    free(queryPlace);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi delete pacle\n");
            //do_exit(conn, res);    
            send(sockfd, REMOVE_PLACE_ERROR,BUFF_SIZE,0);
    }
    else {
        send(sockfd, REMOVE_PLACE_OK,BUFF_SIZE,0);
    }

}

void backup (int sockfd, PGconn *conn) {
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;
    
    int userid = userID(buff, conn);

    char* query = (char*) malloc(sizeof(char)*BUFF_SIZE);

    sprintf(query,"DELETE FROM public.address_backup a WHERE a.user_id = %d", userid);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi delete backup pacle\n");
            //do_exit(conn, res);    
    }

    bzero(query,BUFF_SIZE);

    sprintf(query,"INSERT INTO public.address_backup SELECT * FROM public.\"FavoriteAddress\" f WHERE f.user_id = %d", userid);

    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi insert backup pacle\n");
    }

    free(query);
}

void restore (int sockfd, PGconn *conn) {
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;
    
    int userid = userID(buff, conn);

    char* query = (char*) malloc(sizeof(char)*BUFF_SIZE);

    sprintf(query,"INSERT INTO public.\"FavoriteAddress\" SELECT * FROM public.address_backup a WHERE a.user_id = %d and a.address_id not in ( SELECT f.address_id FROM public.\"FavoriteAddress\" f WHERE f.user_id = %d)", userid, userid);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi delete backup pacle\n");
            //do_exit(conn, res);    
    }

    bzero(query,BUFF_SIZE);

    free(query);
}