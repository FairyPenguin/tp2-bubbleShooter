#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"

using namespace BubbleShooterUtil;

#ifndef BUBBLE_HEADER
#define BUBBLE_HEADER

class Bubble
{
	private:

	const int BUBBLE_RADIUS = 23;		//rayon de la bulle en pixels

	BubbleSprite *spriteSheet;			//charset des bulles
	SDL_Rect sprite[3];					//rectangle des bulles
	SDL_Rect position;					//position de la bulle dans l'espace
	int color;							//couleur de la bulle (0 - vert, 1 - rouge, 2 - jaune)
	int velocityX;						//velocit� en X
	int velocityY;						//velocit� en Y
	bool inGameStatus;					//identifie si la bulle est en-jeu (vrai) ou si elle a �t� d�truite (faux)

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
	Bubble();
	Bubble(int posX, int posY);
	~Bubble();

	//Setters
	void setSprite(int colorValue);
	void setHitbox(SDL_Rect &position, int radius);
	void setVelocity(int velX, int velY);
	void setPosition(int x, int y);
	
	//Fonctions utilitaires
	void move();
	bool checkCollisions(Bubble* otherBubble);
	void update(SDL_Surface *screen);
	bool hasCollidedWithWall();
	//void rotate(Orientation dir);

	//Getters
	BubbleSprite* getSpriteSheet();
	SDL_Rect getPosition();
	int getWidth();
	int getHeight();
	int getColor();
	int getVelocityX();
	int getVelocityY();
	bool getInGameStatus();
	Circle getHitbox();

};

#endif