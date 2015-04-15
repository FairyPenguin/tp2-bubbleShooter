/* En-tête du programme
=========================
Programme: Bubble Shooter
Fichier: bubble.h
Auteur : Amélie Frappier et Marc-Antoine Larose
Date création : 23/03/2015
Date modification: 12/04/2015
Description :  Classe Bubble du jeu Bubble Shooter */

/* Directives au pré-processeur
================================= */
#include "bubbleShooterUtil.h"
#include "../SDL/SDL.h"

using namespace BubbleShooterUtil;

#ifndef BUBBLE_HEADER
#define BUBBLE_HEADER

/* Définition de la classe
=========================== */
class Bubble
{
private:

	static const int BUBBLE_RADIUS = 23;		//rayon de la bulle en pixels
	static const int SPEED_BUBBLE = 6;			//vitesse de l'hypothénuse de la balle
	const double PI = 3.14159265358979323846;	//valeur de pi
	static const int NB_COLORS = 3;				//nombre de couleurs que peut avoir une bulle

	SDL_Surface *spriteSheet;			//charset des bulles
	SDL_Rect sprite[3];					//sprite individuel de la bulle
	SDL_Rect position;					//position de la bulle dans l'espace
	int color;							//couleur de la bulle (0 - vert, 1 - rouge, 2 - jaune)
	int velocityX;						//velocité en X
	int velocityY;						//velocité en Y
	bool inGameStatus;					//identifie si la bulle est en-jeu (vrai) ou si elle me l'est pas (faux)

	bool isChecked;						//valide si la bulle a été déjà vérifiée durant checkAdjacentBubbles()
	bool isAdjacent;					//vérifie si une bulle est adjacente à une autre de même couleur

	

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
	void setVelocity(int angle);
	void setPosition(int x, int y);
	void setPosition(SDL_Rect otherPosition);
	void setInGameStatus(bool status);
	void setIsChecked(bool checked);
	void setIsAdjacent(bool adjacent);
	
	//Fonctions utilitaires
	void move();
	void initCharset();
	bool checkCollisions(Bubble* otherBubble);
	void update(SDL_Surface *screen);
	bool hasCollidedWithWall();

	//Getters
	SDL_Surface* getSpriteSheet();
	SDL_Rect getPosition();
	int getWidth();
	int getHeight();
	int getColor();
	int getVelocityX();
	int getVelocityY();
	bool getInGameStatus();
	bool getIsChecked();
	bool getIsAdjacent();
	Circle getHitbox();

};

#endif