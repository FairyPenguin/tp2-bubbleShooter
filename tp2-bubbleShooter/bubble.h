#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"

using namespace BubbleShooterUtil;

class Bubble
{
	private:

	const int BUBBLE_RADIUS = 23;		//rayon de la bulle en pixels

	SDL_Surface *spriteSheet;			//charset des bulles
	SDL_Rect sprite[3];					//rectangle des bulles
	SDL_Rect position;					//position de la bulle dans l'espace
	int color;							//couleur de la bulle (0 - vert, 1 - rouge, 2 - jaune)
	int velocityX;						//velocité en X
	int velocityY;						//velocité en Y
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

	Circle hitbox;			//hitbox circulaire de la bulle

	//Constructeur et destructeur
	Bubble(int colorValue);
	~Bubble();

	//Setters
	void setHitbox(SDL_Rect &position, int radius);
	void setVelocity(int velX, int velY);
	
	//Fonctions utilitaires
	void move();
	void update(SDL_Surface *screen, int x, int y);
	//void rotate(Orientation dir);

	//Getters
	SDL_Rect getPosition();
	int getColor();
	int getVelocityX();
	int getVelocityY();
	bool getInGameStatus();
	Circle getHitbox();

};