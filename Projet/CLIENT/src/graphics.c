#include "graphics.h"

SDL_Surface* loadImage(char *name)
{
	/* Load the image using SDL Image */

	SDL_Surface *temp = IMG_Load(name);
	SDL_Surface *image;

	if (temp == NULL)
	{
		printf("Failed to load image %s\n", name);

		return NULL;
	}

	/* Make the background transparent */

	SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 0, 0, 0));

	/* Convert the image to the screen's native format */

	image = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	if (image == NULL)
	{
		printf("Failed to convert image %s to native format\n", name);

		return NULL;
	}


	/* Return the processed image */

	return image;
}

void drawImage(SDL_Surface *image, int x, int y)
{
	SDL_Rect dest;

	/* Set the blitting rectangle to the size of the src image */

	dest.x = x;
	dest.y = y;
	dest.w = image->w;
	dest.h = image->h;

	/* Blit the entire image onto the screen at coordinates x and y */

	SDL_BlitSurface(image, NULL, screen, &dest);
}

void updateScreen(int carte[20][20])
{
	/* Blank the screen */

	SDL_FillRect(screen, NULL, 0);

	/* Draw Map */


	int i, j;
	int knight_x, knight_y, barbarian_x, barbarian_y, samurai_x, samurai_y, knight2_x, knight2_y, barbarian2_x, barbarian2_y, samurai2_x, samurai2_y;

	for (i=0; i < 20; i++) {
		for (j=0; j < 20; j++) {
			int a = carte[i][j];
			drawImage(Zone1Image, i*32, j*32);
			switch(a)
			{
				case 1:
					drawImage(Zone1Image, i*32, j*32);
					break;

				case 2:
					drawImage(Zone2Image, i*32, j*32);
					break;

				case 3:
					drawImage(Zone3Image, i*32, j*32);
					break;

				case 4:
					drawImage(Zone4Image, i*32, j*32);
					break;

				case 5:
					drawImage(Zone5Image, i*32, j*32);
					break;

				case 6:
					drawImage(WaterImage, i*32, j*32);
					break;

				case 7:
					drawImage(TreeImage, i*32, j*32);
					break;

				case 10:
					knight_x = i;
					knight_y = j;
					break;

				case 40:
					knight2_x = i;
					knight2_y = j;
					break;

				case 20:
					barbarian_x = i;
					barbarian_y = j;
					break;
				case 50:
					barbarian2_x = i;
					barbarian2_y = j;
					break;

				case 30:
					samurai_x = i;
					samurai_y = j;
					break;
				case 60:
					samurai2_x = i;
					samurai2_y = j;
					break;
			}
		}
	}

	drawImage(KnightImage, knight_x*32 - 32, knight_y*32 - 32);
	drawImage(BarbarianImage, barbarian_x*32, barbarian_y*32);
	drawImage(SamuraiImage, samurai_x*32 - 32 , samurai_y*32 - 32);
	drawImage(KnightImage2, knight2_x*32 - 32, knight2_y*32 - 32);
	drawImage(BarbarianImage2, barbarian2_x*32, barbarian2_y*32);
	drawImage(SamuraiImage2, samurai2_x*32 - 32, samurai2_y*32 - 32);

	/* Swap the image buffers */

	SDL_Flip(screen);
}
