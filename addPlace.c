#include "include/addPlace.h"


#define BUFF_SIZE 1024

int addAddress (int sockfd, PGconn *conn){

	char* Address = (char*)malloc(BUFF_SIZE*sizeof(char)); 
    char name[20];

    strcpy(name,"Ha Noi");
    sprintf(Address,"SELECT \"AddressID\" FROM public.\"Address\" where address = '%s'",name);
	printf("query: %s\n",Address);

	PGresult *res = PQexec(conn, Address);

    //memset(Address,0,BUFF_SIZE);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Loi select Address\n");
        //do_exit(conn, res);    
	}

    int rec_count = PQntuples(res);

	if (rec_count != 0){
        free(Address);
		return atoi(PQgetvalue(res, 0, 0));
	} else {
        char* insertAddress = (char*)malloc(BUFF_SIZE*sizeof(char)); 

        sprintf(insertAddress,"INSERT INTO public.\"Address\" (\"AddressID\", \"Address\", \"CategoryID\") VALUES (nextval(\'Address_id\'), '%s', %d)",name, 1);
        PGresult *res = PQexec(conn, insertAddress);
        free(insertAddress);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi select Address 1\n");
            //do_exit(conn, res);    
        }

         res = PQexec(conn, Address);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Loi select Address 2\n");
            //do_exit(conn, res);    
	    }
        int rec_count = PQntuples(res);
        if (rec_count != 0){
            free(Address);
            return atoi(PQgetvalue(res, 0, 0));
        } 
    }
    return -1;
}
void addPlace(int sockfd, PGconn *conn) {

	char* place = (char*)malloc(BUFF_SIZE*sizeof(char));

    int AdressID = addAddress(sockfd, conn);

    printf("%d\n",AdressID);
	PGresult *res = PQexec(conn, place);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        do_exit(conn);    
	}
	printf("Add palce\n");
	free(place); 
}

void showPlace (int sockfd, PGconn *conn) {

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

    sprintf(queryPlace, "select * from public.\"FavoriteAddress\" as f where f.user_id = %d", 1);

    printf("queryPlace: %s\n",queryPlace);
    PGresult *res = PQexec(conn, queryPlace);

    //memset(Address,0,BUFF_SIZE);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Loi select Address\n");
        //send(sockfd, "Loi", 3, 0);
        //do_exit(conn, res);    
	}

    int rec_count = PQntuples(res);

    printf ("count: %d\n", rec_count);
    char value[BUFF_SIZE] = {0};

    for (int row=0; row<rec_count; row++) {
		
		// test dung
		strcat(value,PQgetvalue(res, row, 1));
		strcat(value,"|");
	}

	printf("value: %s\n",value);
	bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");

    free(queryPlace);
    free(username);
}