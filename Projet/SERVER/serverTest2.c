/* Programme serveur TCP très simple :
 *    - on ouvre une socket d'écoute sur le port 5000
 *    - on attend qu'un client vienne s'y connecter
 *    - une fois le client connecté
 *           - on affiche à l'écran ce qu'a dit le client
 *           - on envoie "Coucou, ici serveur" au client
 *    - on referme les sockets
 *    - on termine le programme
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

static volatile int keepRunning = 1;


void     INThandler(int);

void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
     printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
          exit(0);
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}

struct position {
		int x;
		int y;
	};

	/* string used to handle the map and send to the client */
	char c_carte[400];
	//memset (c_carte, 0, 400);

int number_connections = 0;

int carte[20][20] = {
	    	{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7},
	    	{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
	    };


struct position player_position[10];

/*
 * On crée une socket d'écoute sur le port donné avec un nombre maximum de
 * nb_max_clients de clients en file d'attente.
 */
int cree_serveur_tcp (int port, int nb_max_clients) {
  int s_ecoute ;
  struct sockaddr_in serv_addr ;

  /* on designe l'adresse+port qu'on va ouvrir */
  serv_addr.sin_family = AF_INET ;
  serv_addr.sin_addr.s_addr = htonl (INADDR_ANY) ; /* on attend sur toutes nos adresses */
  serv_addr.sin_port = htons (port) ;
  memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

  /* on cree la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus */
  s_ecoute = socket (PF_INET, SOCK_STREAM, 0) ;
  bind (s_ecoute, (struct sockaddr *)&serv_addr, sizeof serv_addr) ;
  listen (s_ecoute, 5) ; /* on definit la socket s_ecoute, comme etant une socket d'ecoute*/

  return s_ecoute ;
}


char* map_to_string () {
	/* passing the map to the string */
	int i,j;
	memset (c_carte, 0, 400);

	for (i=0;i < 20; i++) {
		for (j=0; j < 20; j++) {
		    c_carte[i*20 + j] = carte[i][j] + '0';
		}
	}

	printf("%s\n", c_carte);
}

/*
 * Le processus qui gere le client recupere par la socket ce que lui raconte le
 * client, l'affiche a l'ecran et repete la meme chose au client.
 */
void *gerer_client (void *data) {
	int s_dial = *(int *) data ;
	char buf [80];
	char login [80] ;
	char password [80];
	char verify_login [80];
	char verify_password [80];
	char s_port [80];
	int port;

	number_connections++;
	int connection_number = number_connections;


	FILE *input_file;

	/* opening file for reading
		fopen (name of the file, "r" for reading mode);
	 */

	input_file = fopen("server_parameters.txt", "r");

	if (input_file == NULL) {
		printf ("Error opening file");
		return(-1);
	}

	/* Allocating all the file informations in variables */

	else {
		fgets (s_port, 80, input_file);
		port = atoi (s_port);
		fgets (verify_login, 80, input_file);
		strtok(verify_login, "\n"); /* function to remove the newline character */
		fgets (verify_password, 80, input_file);
		strtok(verify_password, "\n");
	}

	while (1) {
		memset (buf, 0, 80);
	 	strcpy (buf, "Starting connection...") ;
		write (s_dial, buf, strlen (buf)) ;


		memset (buf, 0, 80);
       	read (s_dial, buf, 80) ;			//receive move from the client
       	printf ("J'ai recu [%s] du client\n", buf) ;

       	while (strcmp(buf,"Retry") == 0) {
       		memset (buf, 0, 80);
		 	strcpy (buf, "Starting connection...") ;
			write (s_dial, buf, strlen (buf)) ;


			memset (buf, 0, 80);
	       	read (s_dial, buf, 80) ;
	       	printf ("J'ai recu [%s] du client\n", buf) ;
       	}


		memset (buf, 0, 80);
	 	strcpy (buf, "Enter Login...") ;
		write (s_dial, buf, strlen (buf)) ;

		memset (login, 0, 80);
		read (s_dial, login, 80);
		printf ("J'ai recu [%s] du client \n", login);

		memset (buf, 0, 80);
	 	strcpy (buf, "Enter Password...") ;
	 	write (s_dial, buf, strlen (buf)) ;

	 	memset (password, 0, 80);
		read (s_dial, password, 80);
		printf ("J'ai recu [%s] du client \n", password);

		int verify1, verify2;

		verify1 = strcmp(verify_login, login);
		verify2 = strcmp(verify_password, password);

		if (verify1 == 0 && verify2 == 0) {
			memset (buf, 0, 80);
	    	strcpy (buf, "Login ok, bienvenue");
	        write (s_dial, buf, strlen (buf));
	        printf ("J'ai envoye [%s] au client\n", buf) ;

	        break;
		}

		else {
			memset (buf, 0, 80);
	    	strcpy (buf, "Sorry, wrong login and/or password");
	        write (s_dial, buf, strlen (buf));
	        printf ("J'ai envoye [%s] au client\n", buf) ;
	        //close (s_dial) ;
			//close (s_ecoute) ;
			return 0;
		}

	}


   	memset (buf, 0, 80);
    read (s_dial, buf, 80);
    printf ("J'ai recu [%s] du client\n", buf) ;


    /* if connection is Ok and Start signal is received */
    if (strcmp(buf,"Start") == 0) {
		/* knight */
		if (connection_number == 1) {
			player_position[connection_number].x = 1;
			player_position[connection_number].y = 12;
			carte[player_position[connection_number].x][player_position[connection_number].y] = 10;
		}
		/* barbarian */
		if (connection_number == 2) {
			player_position[connection_number].x = 10;
			player_position[connection_number].y = 1;
			carte[player_position[connection_number].x][player_position[connection_number].y] = 20;
		}

		/* samurai */
		if (connection_number == 3) {
			player_position[connection_number].x = 12;
			player_position[connection_number].y = 19;
			carte[player_position[connection_number].x][player_position[connection_number].y] = 30;
		}

		if (connection_number == 4) {
			player_position[connection_number].x = 2;
			player_position[connection_number].y = 2;
			carte[player_position[connection_number].x][player_position[connection_number].y] = 40;
		}

		if (connection_number == 5) {
			player_position[connection_number].x = 6;
			player_position[connection_number].y = 6;
			carte[player_position[connection_number].x][player_position[connection_number].y] = 50;
		}

		if (connection_number == 6) {
			player_position[connection_number].x = 11;
			player_position[connection_number].y = 11;
			carte[player_position[connection_number].x][player_position[connection_number].y] = 60;
		}


	//}

	bzero (buf, 80) ;

		while (1) {
			if (connection_number == 7){
				memset (buf, 0, 80);
				read (s_dial, buf, 80);

				memset (buf, 0, 80);
	 			strcpy (buf, "Sorry, the server is full !");
	 			write (s_dial, buf, strlen (buf)) ;
				break;
			}

			memset (buf, 0, 80);
			read (s_dial, buf, 80);
	    	/* see what was received from the client and make the change needed. */
	    	//printf ("J'ai recu [%s] du client\n", buf) ;
	    	printf("%d\n", connection_number);
	    	if (handle_movement(buf, connection_number) == 0) {
	    		number_connections--;
	    		break;
	    	}

	    	/* transform the map into a string and sends to the client */
			map_to_string();
		    //printf("%s", c_carte); // printing the map in the server side for debug
			write (s_dial, c_carte, strlen(c_carte));	//send map to the client via string

		}
	}

	number_connections--;
	free (data) ;
	close (s_dial) ;
	return (NULL) ;
}


int handle_movement (char buf[80], int connection_number) {
	int check;
        	if (strcmp(buf,"Right") == 0) {
        		memset (buf, 0, 80);
        		/* checks if it hits an tree before moving... if yes, the player dont move */
        		check = carte[player_position[connection_number].x][player_position[connection_number].y+1];
        		if (check != 7 && check != 10 && check != 20 && check != 30 && check != 40 && check != 50 && check != 60) {

        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number%3 +1;
        			player_position[connection_number].y++;
        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number*10;


        		}
        	}

        	else if (strcmp(buf,"Left") == 0) {
        		memset (buf, 0, 80);
        		check = carte[player_position[connection_number].x][player_position[connection_number].y-1];
        		if (check != 7 && check != 10 && check != 20 && check != 30 && check != 40 && check != 50 && check != 60) {

        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number%3 +1;
        			player_position[connection_number].y--;
        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number*10;
        		}

        	}

        	else if (strcmp(buf,"Up") == 0) {
        		memset (buf, 0, 80);
        		check = carte[player_position[connection_number].x-1][player_position[connection_number].y];
        		if (check != 7 && check != 10 && check != 20 && check != 30 && check != 40 && check != 50 && check != 60) {
        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number%3 +1;
        			player_position[connection_number].x--;
        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number*10;
        		}
        	}

        	else if (strcmp(buf,"Down") == 0) {
        		memset (buf, 0, 80);
        		check = carte[player_position[connection_number].x+1][player_position[connection_number].y];
        		if (check != 7 && check != 10 && check != 20 && check != 30 && check != 40 && check != 50 && check != 60) {
        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number%3 +1;
        			player_position[connection_number].x++;
        			carte[player_position[connection_number].x][player_position[connection_number].y] = connection_number*10;
        		}
        	}

        	else if (strcmp(buf,"Nothing") == 0) {
        	}

        	else if (strcmp(buf,"Esc") == 0) {
        		return 0;
        	}

        	/* default case */
        	else {
        		memset (buf, 0, 80);
				printf("Invalid Command \n");
				return 1;
        	}

        	return 1;
}



int main () {
	char buf [80];
	char login [80] ;
	char password [80];
	char verify_login [80];
	char verify_password [80];
	char s_port [80];
	int port;

	FILE *input_file;

	int s_ecoute, s_dial, cli_len ;
	struct sockaddr_in serv_addr, cli_addr ;
	int so_reuseaddr = 1 ;

	int r ;

	/* On crée une socket d'écoute sur le port 5000 et on peut mettre
           jusqu'à 5 clients en file d'attente */
	s_ecoute = cree_serveur_tcp (5000, 6) ;
	while (1) {
    /*
		signal(SIGINT, INThandler);
	      while (1)
	           pause();
	      return 0;
    */ 
		pthread_attr_t *thread_attributes ;
		pthread_t tid ;
		int *s_dial ;

		s_dial = malloc (sizeof *s_dial) ;

		/* On se met en ecoute sur la socket. C'est une fonction
                   bloquante qui ne se debloque que lorsqu'un client vient
                   se connecter sur cette socket d'ecoute.
                   La valeur de retour est alors le descripteur de la socket
	           de connexion permettant  de dialoguer avec CE client. */
		*s_dial = accept (s_ecoute, NULL, 0) ;

		thread_attributes = malloc (sizeof *thread_attributes) ;
		r = pthread_attr_init (thread_attributes) ;
		r = pthread_attr_setdetachstate (thread_attributes, PTHREAD_CREATE_DETACHED) ;

		r = pthread_create (&tid, thread_attributes, gerer_client, s_dial) ;

		r = pthread_attr_destroy (thread_attributes) ;



	}

	printf("Closing server.... \n");
	close (s_ecoute) ;
	exit (EXIT_SUCCESS);
}
