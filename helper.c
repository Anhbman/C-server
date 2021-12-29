#include "include/helper.h"
#include "include/constant.h"

int userID (char* name, PGconn *conn) {
    char* query = (char *) malloc (sizeof(char));
    sprintf(query, "SELECT user_id FROM public.taikhoan WHERE user_name = '%s'", name);

    printf("getUserID: %s\n",query);
    PGresult *res = PQexec(conn, query);
   if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    

    int rec_count = PQntuples(res);    

    if (rec_count != 0){
        free(query);
		return atoi(PQgetvalue(res, 0, 0));
    }
    
    return -1;
}


void showPlaceUser(int sockfd, char* name,PGconn *conn) {

    char* buff = (char*)malloc(sizeof(char)*BUFF_SIZE);;
    int bytes_received;

    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0){
        printf("Connection closed.\n");
        return;
    }

    buff[bytes_received] = 0;

    strcpy(name,buff);
    //int getUserID = userID(name, conn);

    char* query = (char*)malloc(sizeof(char)*BUFF_SIZE);

    sprintf(query,"select f.address_id, a.address from public.\"FavoriteAddress\" as f, public.\"Address\" as a where f.address_id = a.address_id and f.user_id = %d",1);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    
    int rec_count = PQntuples(res);
	
    printf("count: %d\n",rec_count);
	char value[BUFF_SIZE] = {0};
	
    for (int row=0; row<rec_count; row++) {
		
		// test dung
        printf("1\n");
		strcat(value,PQgetvalue(res, row, 1));
		strcat(value,"|");
	}

	printf("value: %s\n",value);
	int bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");
    
    free(buff);
    free(name);
    free(query);
    
}

void showFriend(int sockfd, char* name,PGconn *conn) {

    printf("show friend\n");
    char* query = (char*)malloc(sizeof(char)*BUFF_SIZE);
    
    sprintf(query,"select t.user_name from public.\"Friend\" as f, public.taikhoan as t where f.user_id1 = %d and f.user_id2 = t.user_id;",1);
    
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    
    int rec_count = PQntuples(res);

    char value[BUFF_SIZE] = {0};

    for (int row=0; row<rec_count; row++) {
		
		// test dung
        printf("1\n");
		strcat(value,PQgetvalue(res, row, 0));
		strcat(value,"|");
	}

    printf("value: %s\n",value);
	int bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");
    free(query);
}