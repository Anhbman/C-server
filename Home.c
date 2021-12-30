#include "include/Home.h"

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
    
    
    int numberCate = getNumberCate(conn);
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
		
		strcpy(value,PQgetvalue(res, row, 0));
		strcat(value,"|");
        strcat(value, PQgetvalue(res, row, 1));
        int bytes_sent = send(sockfd, value, BUFF_SIZE, 0); /* echo to the client */
        if (bytes_sent < 0)
            perror("\nError: ");
        bzero(value,0);
	}

	//printf("value: %s\n",value);
	int bytes_sent = send(sockfd, SEND_END, BUFF_SIZE, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");

    free(query);
    free(value);
}