#include "init.h"

void init(char *title)
{
	/* Initialise SDL Video */
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	/* Open a 640 x 640 screen */
	
	screen = SDL_SetVideoMode(640, 640, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
	
	if (screen == NULL)
	{
		printf("Couldn't set screen mode to 640 x 640 %s\n", SDL_GetError());

		exit(1);
	}
	
	KnightImage = loadImage("assets/knight/1.png");
	BarbarianImage = loadImage("assets/barbarian/1.png");
	SamuraiImage = loadImage("assets/samurai/1.png");

	KnightImage2 = loadImage("assets/knight2/1.png");
	BarbarianImage2 = loadImage("assets/barbarian2/1.png");
	SamuraiImage2 = loadImage("assets/samurai2/1.png");
	WaterImage = loadImage("assets/6.png");
	Zone1Image = loadImage("assets/1.png");
	Zone2Image = loadImage("assets/2.png");
	Zone3Image = loadImage("assets/3.png");
	Zone4Image = loadImage("assets/4.png");
	Zone5Image = loadImage("assets/5.png");
	TreeImage = loadImage("assets/7.png");
	/* Set the screen title */
	
	SDL_WM_SetCaption(title, NULL);
}

void loginMenu(char* ip_adress, char* login, char* password) 
{

		/* Login menu
		   Here the player will insert the host's ip_adrres, his login and password.
		   If the logins is accepted, he will be sent to the main menu game, else, try again.
		   And also choose his character;
		*/ 
}

void cleanup()
{
	/* Free the image */

	SDL_FreeSurface(KnightImage);
	SDL_FreeSurface(BarbarianImage);
	SDL_FreeSurface(SamuraiImage);
	SDL_FreeSurface(KnightImage2);
	SDL_FreeSurface(BarbarianImage2);
	SDL_FreeSurface(SamuraiImage2);
	SDL_FreeSurface(WaterImage);
	SDL_FreeSurface(Zone1Image);
	SDL_FreeSurface(Zone2Image);
	SDL_FreeSurface(Zone3Image);
	SDL_FreeSurface(Zone4Image);
	SDL_FreeSurface(Zone5Image);

	
	
	/* Shut down SDL */
	
	SDL_Quit();
}
