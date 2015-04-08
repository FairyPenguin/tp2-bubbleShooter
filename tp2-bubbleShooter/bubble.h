#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"

using namespace BubbleShooterUtil;

class Bubble
{
private:

	SDL_Surface *spriteSheet;			//charset des bulles
	SDL_Rect sprite[3];					//rectangle des bulles
	SDL_Rect position;					//position de la bulle dans l'espace
	int color;							//couleur de la bulle (0 - vert, 1 - rouge, 2 - jaune)
	bool inGameStatus;					//identifie si la bulle est en-jeu (vrai) ou si elle a été détruite (faux)

public:

	/* Structures du programme
	=========================== */
	struct Circle
	{
		int centerX;		//position du centre du cercle en X
		int centerY;		//position du centre du cercle en Y
		int radius;			//rayon du cercle
	};

	Circle hitbox;						//hitbox circulaire de la bulle

	//Constructeur et destructeur
	Bubble(int colorValue);
	~Bubble();

	void update(SDL_Surface *screen, int x, int y);
	//void rotate(Orientation dir);

	//Getters
	SDL_Rect getPosition();
	int getColor();
	bool getInGameStatus();
	Circle getHitbox();

	//Setters
	void setHitbox(SDL_Rect &position, int radius);

};