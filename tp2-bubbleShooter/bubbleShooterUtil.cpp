#include "bubbleShooterUtil.h"

using namespace std;

namespace BubbleShooterUtil 
{

	const int SCREEN_HEIGHT = 600;
	const int SCREEN_WIDTH = 450;
	const int NB_COLORS = 3;
	/*const int GRID_HEIGHT = 10;
	const int GRID_WIDTH = 8;*/

	/* Initialise la SDL
	====================== */
	void initVideo()
	{
		SDL_Init(SDL_INIT_EVERYTHING);	//D�marrage de la SDL (charger le syst�me vid�o)

		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)  //Si le d�marrage de la librairie SDL �choue
		{
			//Gestion des erreurs de la librairie SDL
			cout << "Erreur lors de l'initialisation de la SDL", SDL_GetError();
			exit(EXIT_FAILURE);
		}
	}

	/* Initialise les param�tres de l'�cran
	========================================== */
	SDL_Surface* setupScreen(char* title, char* img)
	{
		SDL_Surface* screen;	//�cran � param�trer

		//Chargement de la vid�o
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		if (screen == NULL)
		{
			cout << "Erreur lors de l'initialisation de la SDL" << SDL_GetError();
			exit(EXIT_FAILURE);
		}

		//Modification des param�tres de la vid�o
		SDL_WM_SetCaption(title, NULL);
		SDL_WM_SetIcon(SDL_LoadBMP(img), NULL);

		return screen;
	}

	/* T�l�charge une image en format .bmp
	========================================== */
	SDL_Surface* loadBitmap(char* imgName)
	{
		SDL_Surface* loadedImage = nullptr;		//Temporary storage for the image that's loaded
		SDL_Surface* optimizedImage = nullptr;		//The optimized image that will be used

		loadedImage = SDL_LoadBMP(imgName);		//Load the image

		//If nothing went wrong in loading the image
		if (loadedImage != nullptr)
		{
			//Create an optimized image
			optimizedImage = SDL_DisplayFormat(loadedImage);

			//Free the old image
			SDL_FreeSurface(loadedImage);
		}

		return optimizedImage;
	}

	/* Param�tre la position d'une image dans l'espace
	==================================================== */
	void setPosition(SDL_Surface *picture, SDL_Rect &position, int posX, int posY)
	{
		position.x = posX;
		position.y = posY;
	}

	/* Met la couleur de fond d'une image en transparence
	======================================================= */
	void setTransparency(SDL_Surface *picture, int red, int green, int blue)
	{
		//If nothing went wrong in loading the image
		if (picture != NULL)
		{
			SDL_SetColorKey(picture, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(picture->format, red, green, blue));
		}
	}

	/* Met � jour l'�cran du jeu
	Cette fonction ne prend pas en compte les charset
	===================================================== */
	void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position)
	{
		SDL_BlitSurface(picture, NULL, screen, &position);
	}

	/* Met � jour l'�cran du jeu
	Cette fonction prend pas en compte les charset
	===================================================== */
	void updateScreenWithSpriteSheet(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position) 
	{
		SDL_BlitSurface(picture, &currentSprite, screen, &position);
	}

	/* Initialise le tableau des sprites d'un charset donn�
	========================================================= */
	void initCharset(SDL_Rect charset[], int taille)
	{

		for (int x = 0; x < taille; x++)		//pour chaque ligne de l'image
		{

			charset[x].x = x * 47;		//position du posCharset sur l'axe des X
			charset[x].y = 0;			//position de posCharset sur l'axe des Y
			charset[x].w = 47;			//largeur en pixels du rectangle
			charset[x].h = 47;			//hauteur en pixels du rectangle
		}

	}

	/* Assigne et retourne une valeur al�atoire
	============================================ */
	int setRandomValue(int maxValue)
	{
		
		srand(time(NULL));	//initialise le g�n�rateur de nombres al�atoires
		
		int random;

		random = (rand() % maxValue);		//g�n�re un nombre al�atoire

		return random;
	}

	/* Calcule la distance entre deux points (Th�or�me de Pythagore)
	================================================================== */
	double calculateDistance(int x1, int x2, int y1, int y2) 
	{
		return sqrt((double)pow(x2 - x1, 2) + (double)pow(y2 - y1, 2));
	}
	
}

