#include "main.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>



extern void init(char *);
extern void cleanup(void);
extern void getInput(char buf[80]);
extern SDL_Surface *loadImage(char *);
extern void updateScreen(int carte[20][20]);



void string_to_map (int carte[20][20], char buf[400]) {
	int i = 0;
	for (i = 0; i < 400; i++) {
		carte[i%20][i/20] = buf[i] - '0';
	}
}



int main(int argc, char *argv[])
{
	int go;
	int port;

	char ip_adress[80];
	char login[80];
	char password[80];
	char c_carte[400];
	char buf [80] ;
	char s_port [80];
	int carte[20][20];

	FILE *input_file;

	/* opening file for reading
		fopen (name of the file, "r" for reading mode);
	 */

	input_file = fopen("client_parameters.txt", "r");

	if (input_file == NULL) {
		printf ("Error opening file");
		return(-1);
	}

	/* Allocating all the file informations in variables */

	else {
		fgets (s_port, 80, input_file);
		port = atoi (s_port);
		fgets (ip_adress, 80, input_file);
		strtok(ip_adress, "\n"); 	/* function to remove the newline character */
		fgets (login, 80, input_file);
		strtok(login, "\n");
		fgets (password, 80, input_file);
		strtok(password, "\n");
	}

	/* Start up SDL */

	init("KingdomWar");

	/* Start the login menu, where the client will insert the server ip_adress, his login and password */
	loginMenu(ip_adress, login, password);

	/* Call the cleanup function when the program exits */

	atexit(cleanup);

	go = 1;

	/* Loop indefinitely for messages
		1ª - Check input
		2ª - Sends to Server
		3ª - Receives map from server
		4ª - UpdateScreen;
	*/

	int s_cli ;
	struct sockaddr_in serv_addr ;
	port = 5000;
	strcpy (ip_adress, ip_adress);

	s_cli = socket (PF_INET, SOCK_STREAM, 0) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr (ip_adress) ;
	serv_addr.sin_port = htons (port) ;
	memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

	connect (s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr) ;

    printf ("Je me suis connecté au serveur %s sur son port %d\n", \
            inet_ntoa (serv_addr.sin_addr), ntohs (serv_addr.sin_port));

    strcpy (buf, "Nothing");




        memset (buf, 0, 80);
        read (s_cli, buf, 80) ;
        printf ("J'ai recu [%s] du serveur\n", buf) ;

        while (strcmp(buf,"") == 0) {
        	printf ("Failed to connect, didnt receive nothing from the server.... Trying again \n");
        	memset (buf, 0, 80);
			strcpy (buf, "Retry") ;
			write (s_cli, buf, strlen (buf));
			printf ("J'ai envoye [%s] au serveur\n", buf) ;

			memset (buf, 0, 80);
        	read (s_cli, buf, 80);
        	printf ("J'ai recu [%s] du serveur\n", buf) ;
        }

        memset (buf, 0, 80);
		strcpy (buf, "Connection Started...") ;
		write (s_cli, buf, strlen (buf));
		printf ("J'ai envoye [%s] au serveur\n", buf) ;



        /*
	memset (login, 0, 80);
	 strcpy (login, "abcdef") ; */

	memset (buf, 0, 80);
    read (s_cli, buf, 80);
    printf ("J'ai recu [%s] du serveur\n", buf) ;

    if (strcmp(buf,"Enter Login...") == 0) {
		write (s_cli, login, strlen (login)) ;
		printf ("J'ai envoye [%s] au serveur\n", login) ;
	}


  	memset (buf, 0, 80);
    read (s_cli, buf, 80) ;
    printf ("J'ai recu [%s] du serveur\n", buf) ;

	if (strcmp(buf,"Enter Password...") == 0) {
		write (s_cli, password, strlen (password)) ;
		printf ("J'ai envoye [%s] au serveur\n", password) ;
	}

	memset (buf, 0, 80);
    read (s_cli, buf, 80) ;
    printf ("J'ai recu [%s] du serveur\n", buf) ;

    if (strcmp(buf,"Login ok, bienvenue") == 0) {

	    memset (buf, 0, 80);
		strcpy (buf, "Start") ;
		write (s_cli, buf, strlen (buf));
		printf ("J'ai envoye [%s] au serveur\n", buf) ;
	}




	while (go)
	{

		//SDL_Delay(1000);
		strcpy (buf, "Nothing");
		getInput(buf);
		printf ("J'ai envoye [%s] du serveur\n", buf) ;
		write (s_cli, buf, strlen (buf));

		if (strcmp(buf,"Esc") == 0) {
			go = 0;
		}

		memset (c_carte, 0, 400);
        read (s_cli, c_carte, 400) ;
        printf ("J'ai recu [%s] du serveur\n", c_carte) ;

        string_to_map(carte, c_carte);

		updateScreen(carte);

	}

	/* Exit the program */

	close (s_cli) ;
	exit(0);
}
