#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>   // stat
#include <stdbool.h>  
#include <libgen.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <postgresql/libpq-fe.h>
#include "include/Login.h"
#include "include/constant.h"
#include "include/addPlace.h"
#include "include/FuncConstant.h"
#include "include/helper.h"
#include "include/home.h"
#include "include/AddFriend.h"
#include "include/Register.h"
#include "include/SharePlace.h"


// #define PORT 5500


/* Handler process signal*/
void sig_chld(int signo);

/*
* Receive and echo message to client
* [IN] sockfd: socket descriptor that connects to client 	
*/
void echo(int sockfd);



void controller (int sockfd){

	PGconn *conn = PQconnectdb("user=postgres host=localhost password=1304 dbname=LTM");
	if (PQstatus(conn) == CONNECTION_BAD) {   
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        do_exit(conn); 
    }
	
	while (1)
	{
		char buff[BUFF_SIZE];
		int bytes_received;

		bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
		if (bytes_received < 0)
			perror("\nError: ");
		else if (bytes_received == 0){
			printf("Connection closed.\n");
			return;
		}
		buff[bytes_received] = 0;
		int chon = atoi(buff);

		switch (chon)
		{
		case 1:
			printf("Login\n");
			Login(sockfd, conn);
			break;
		case 2:
			home(sockfd, conn);
			break;
		case 3:
			printf("showPlace\n");
			show_page_data(sockfd, conn);
			break;
		case 5:
			printf("show_user_place\n");
			showPlaceUser(sockfd,conn);
			break;
		case 7:
			printf("register\n");
			Register(sockfd,conn);
			break;
		case 8:
			printf("Add place\n");
			addPlace(sockfd, conn);
			break;
		case 9:
			printf("delete\n");
			remove_place(sockfd,conn);
			break;

		case 10:
			backup(sockfd, conn);
			break;
		case 11:
			printf("restore\n");
			restore(sockfd,conn);
			break;
		case 15 :
			printf("Add Friend\n");
			Addfriend(sockfd,conn);
			break;
		case 16:
			printf("Add Friend\n");
			showFriendList(sockfd,conn);
			
			break;
		case 17:
			printf("Show User List\n");
			showUserList(sockfd,conn);
			break;
		case 18:
			printf("Show Friend Request\n");
			showFriendRequest(sockfd,conn);
			break;
		case 19:
			printf("Accept Friend\n");
			acceptFriend(sockfd,conn);
			break;
		case 20:
			printf("Share Place\n");
			sharePlace(sockfd,conn);
			break;
		case 999:

			close(sockfd);
			return;
		
		case 21:
			printf("Delete Request\n");
			deleteRequest(sockfd,conn);
			break;
		default:
			break;
		}
	}
	
}

int main(int argc, char* argv[]) {

    // Server

    int listen_sock, conn_sock; /* file descriptors */
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	pid_t pid;
	int sin_size;
	int PORT = atoi(argv[1]);

	if ((listen_sock=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		printf("socket() error\n");
		return 0;
	}
	
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   

	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listen_sock, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}
	
	/* Establish a signal handler to catch SIGCHLD */
	signal(SIGCHLD, sig_chld);

    // Server

    while(1){
		sin_size=sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size))==-1){
			if (errno == EINTR)
				continue;
			else{
				perror("\nError: ");			
				return 0;
			}
		}
		
		/* For each client, fork spawns a child, and the child handles the new client */
		pid = fork();
		
		/* fork() is called in child process */
		if(pid  == 0){
			close(listen_sock);
			printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */
			controller(conn_sock);
			printf("done\n");
			//echo(conn_sock);					
			exit(0);
		}
		
		/* The parent closes the connected socket since the child handles the new client */
		close(conn_sock);
	}
	close(listen_sock);

    return 0;
}

void sig_chld(int signo){
	pid_t pid;
	int stat;
	
	/* Wait the child process terminate */
	while((pid = waitpid(-1, &stat, WNOHANG))>0)
		printf("\nChild %d terminated\n",pid);
}