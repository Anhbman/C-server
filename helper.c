#include "include/helper.h"
#include "include/constant.h"

int userID (char* name, PGconn *conn) {
    char* query = (char *) malloc (sizeof(char));
    sprintf(query, "SELECT user_id FROM public.taikhoan WHERE user_name = '%s'", name);

    printf("getUserID: %s\n",query);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("Loi select helper UserID\n");
        //do_exit(conn, res);    
	}

    int rec_count = PQntuples(res);    

    if (rec_count != 0){
        free(query);
		return atoi(PQgetvalue(res, 0, 0));
    }
    
    return -1;
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

int getPlaceID (int sockfd,PGconn *conn, char* namePlace) {
    char* Address = (char*)malloc(BUFF_SIZE*sizeof(char)); 

    sprintf(Address,"SELECT \"address_id\" FROM public.\"Address\" where address = '%s'",namePlace);
	printf("query: %s\n",Address);

	PGresult *res = PQexec(conn, Address);

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
    }    

    int rec_count = PQntuples(res);

    printf("count: %d\n", rec_count);
	
    free(Address);
    return atoi(PQgetvalue(res, 0, 0));
}