#include "bubbleShooterUtil.h"

using namespace std;

namespace BubbleShooterUtil 
{

	const int SCREEN_HEIGHT = 600;
	const int SCREEN_WIDTH = 450;

	/* Initialise la SDL
	====================== */
	void initVideo()
	{
		SDL_Init(SDL_INIT_EVERYTHING);	//Démarrage de la SDL (charger le système vidéo)

		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)  //Si le démarrage de la librairie SDL échoue
		{
			//Gestion des erreurs de la librairie SDL
			cout << "Erreur lors de l'initialisation de la SDL", SDL_GetError();
			exit(EXIT_FAILURE);
		}
	}

	/* Initialise les paramètres de l'écran
	========================================== */
	SDL_Surface* setupScreen(char* title, char* img)
	{
		SDL_Surface* screen;	//Écran à paramètrer

		//Chargement de la vidéo
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		if (screen == NULL)
		{
			cout << "Erreur lors de l'initialisation de la SDL" << SDL_GetError();
			exit(EXIT_FAILURE);
		}

		//Modification des paramètres de la vidéo
		SDL_WM_SetCaption(title, NULL);
		SDL_WM_SetIcon(SDL_LoadBMP(img), NULL);

		return screen;
	}

	/* Télécharge une image en format .bmp
	========================================== */
	SDL_Surface* loadBitmap(char* imgName)
	{
		SDL_Surface* loadedImage = NULL;		//stocke temporairement l'image à charger
		SDL_Surface* optimizedImage = NULL;		//l'image optimisée en format 32-bits

		loadedImage = SDL_LoadBMP(imgName);		//Charge l'image

		//si l'image est bien chargée en mémoire
		if (loadedImage != nullptr)
		{
			//optimiser l'image en format 32-bits
			optimizedImage = SDL_DisplayFormat(loadedImage);

			//libérer la surface de l'image temporaire de la mémoire
			SDL_FreeSurface(loadedImage);
		}

		return optimizedImage;
	}

	/* Paramètre la position d'une image dans l'espace
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
		//Si l'image est bien chargée en mémoire
		if (picture != NULL)
		{
			SDL_SetColorKey(picture, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(picture->format, red, green, blue));
		}
	}

	/* Met à jour l'écran du jeu
	Cette fonction ne prend pas en compte les charset
	===================================================== */
	void updateScreen(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &position)
	{
		SDL_BlitSurface(picture, NULL, screen, &position);
	}

	/* Met à jour l'écran du jeu
	Cette fonction prend pas en compte les charset
	===================================================== */
	void updateScreenWithSpriteSheet(SDL_Surface *screen, SDL_Surface *picture, SDL_Rect &currentSprite, SDL_Rect &position) 
	{
		SDL_BlitSurface(picture, &currentSprite, screen, &position);
	}

	/* Assigne et retourne une valeur aléatoire
	============================================ */
	int getRandomValue(int maxValue)
	{
		int random;
		
		for (int i = 0; i < 3; i++)
		{
			random = (rand() % maxValue);		//génère un nombre aléatoire
		}
		
		return random;
	}

	/* Calcule la distance entre deux points (Théorème de Pythagore)
	================================================================== */
	double calculateDistance(int x1, int x2, int y1, int y2) 
	{
		return sqrt((double)pow(x2 - x1, 2) + (double)pow(y2 - y1, 2));
	}
	
}

