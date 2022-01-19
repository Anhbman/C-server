#include "include/addPlace.h"


#define BUFF_SIZE 1024

int addAddress (int sockfd,char* namePlace,char* category,PGconn *conn){

	char* Address = (char*)malloc(BUFF_SIZE*sizeof(char)); 

    int cateID = getCategoryID(conn,category);

    sprintf(Address,"SELECT \"address_id\" FROM public.\"Address\" where address = '%s' and category_id = %d ",namePlace, cateID);
	printf("query: %s\n",Address);

	PGresult *res = PQexec(conn, Address);

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
    }    

    int rec_count = PQntuples(res);

    printf("count: %d\n", rec_count);
	
    if (rec_count != 0){
        free(Address);
		return atoi(PQgetvalue(res, 0, 0));
	} else {
        char* insertAddress = (char*)malloc(BUFF_SIZE*sizeof(char)); 

        sprintf(insertAddress,"INSERT INTO public.\"Address\" (\"address_id\", \"address\", \"category_id\") VALUES (nextval(\'Address_id\'), '%s', %d)",namePlace, cateID);
       
        PGresult *res = PQexec(conn, insertAddress);

        free(insertAddress);
        
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi select Address 1\n");
            //do_exit(conn, res);    
        }

        res = PQexec(conn, Address);

        free(Address);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("No data retrieved\n");        
            PQclear(res);
            do_exit(conn);
        }    

        int rec_count = PQntuples(res);
        if (rec_count != 0){
            return atoi(PQgetvalue(res, 0, 0));
        } 
    }
    return -1;
}

void addPlace(int sockfd, PGconn *conn) {

	char* place = (char*)malloc(BUFF_SIZE*sizeof(char));

    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking

    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.\n");
    buff[bytes_received] = 0;

    printf("recv: %s\n",buff);

    char* user = strtok(buff, "|");
    char* namePlace = strtok(NULL, "|");
    char* category = strtok(NULL, "|");

    int AdressID = addAddress(sockfd, namePlace, category, conn);
    int getUser = userID(user, conn);

    printf("UserID: %d\n", getUser);
    printf("addressID: %d\n", AdressID);
    sprintf(place, "INSERT INTO public.\"FavoriteAddress\" (user_id, address_id, friend_id) VALUES (%d, %d, %d);",getUser, AdressID, 0);

	PGresult *res = PQexec(conn, place);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("ERROR query addPlace\n");
        bytes_sent = send(sockfd, ADD_PLACE_ERR, BUFF_SIZE, 0);
        if (bytes_sent <= 0)
        {
            printf("ERROR add Place");
        }
	} else {
        bytes_sent = send(sockfd, ADD_PLACE_OK, BUFF_SIZE, 0);
        if (bytes_sent <= 0)
        {
            printf("ERROR add Place");
        }
    }

    
	free(place); 
}

