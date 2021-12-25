#include "include/helper.h"
#include "include/constant.h"

int userID (char* name, PGconn *conn) {
    char* query = (char *) malloc (sizeof(char));
    sprintf(query, "SELECT user_id FROM public.taikhoan WHERE user_name = '%s'", "batman");

    printf("getUserID: %s\n",query);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
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


void showPlaceUser(int sockfd, PGconn *conn) {

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

    char* name = (char*)malloc(sizeof(char)*BUFF_SIZE);
    strcpy(name,buff);
    //int getUserID = userID(name, conn);

    char* query = (char*)malloc(sizeof(char)*BUFF_SIZE);

    fprintf(query,"select DISTINCT f.address_id, a.address from public.\"FavoriteAddress\" f, public.\"Address\" a, t where f.address_id = a.address_id and f.user_id = %d",1);

    PGresult *res = PQexec(conn, query);
    int rec_count = PQntuples(res);
	
	char value[BUFF_SIZE] = {0};
	
    printf("bangi: %d\n",rec_count);
    for (int row=0; row<rec_count; row++) {
		
		// test dung
        printf("1\n");
		strcat(value,PQgetvalue(res, row, 1));
		strcat(value,"|");
	}

	printf("value: %s\n",value);
	int bytes_sent = send(sockfd, "hanoi|", BUFF_SIZE, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");
    
    free(buff);
    free(name);
    free(query);
    
}