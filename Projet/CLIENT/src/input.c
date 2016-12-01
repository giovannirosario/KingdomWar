#include "input.h"
#include <string.h>

void getInput(char buf[80])
{
	SDL_Event event;
	
	/* Loop through waiting messages and process them */
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			/* Closing the Window or pressing Escape will exit the program */
			
			case SDL_QUIT:
				exit(0);
			break;
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						strcpy (buf, "Esc");
						break;
					
					case SDLK_DOWN:
						strcpy (buf, "Down");
						break;

					case SDLK_UP:
						strcpy(buf, "Up");
						break;

					case SDLK_RIGHT:
						strcpy (buf, "Right");
						break;

					case SDLK_LEFT:
						strcpy(buf, "Left");
						break;


					default:
						strcpy(buf, "Nothing");
						printf ("[%s]\n", buf) ;
						break;

					break;
				}
			break;
		}
	}
}
