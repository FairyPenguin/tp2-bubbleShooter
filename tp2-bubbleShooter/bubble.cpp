#include "bubble.h"

/* Constructeur de la classe "Bubble"
====================================== */
Bubble::Bubble() //: inGameStatus(false), velocityX(0), velocityY(0)
{
	inGameStatus = false;
	velocityX = 0;
	velocityY = 0;
	//constructeur par défaut
}

Bubble::Bubble(int posX, int posY) //: inGameStatus(false), velocityX(0), velocityY(0)
{
	inGameStatus = false;
	position.x = posX;
	position.y = posY;
}

/* Destructeur de la classe "Bubble" 
====================================== */
Bubble::~Bubble()
{
	//Libérer la surface des bulles de la mémoire si la surface est en jeu
	if (inGameStatus)
	{
		SDL_FreeSurface(spriteSheet);
	}
	
}

/*Retourne la position d'une bulle donnée
========================================== */
BubbleSprite* Bubble::getSpriteSheet()
{
	return spriteSheet;
}

/* Retourne la position d'une bulle donnée
============================================ */
SDL_Rect Bubble::getPosition() 
{
	return position;
}

/* Retourne la couleur d'une bulle donnée 
========================================== */
int Bubble::getColor() 
{
	return color;
}

/* Retourne la largeur d'une bulle donnée
========================================== */
int Bubble::getWidth() 
{
	return position.w;
}

/* Retourne la hauteur d'une bulle donnée
========================================== */
int Bubble::getHeight()
{
	return position.h;
}

/* Retourne la veleur indiquant si une bulle donnée a explosé (et est donc inaccessible)
========================================================================================= */
bool Bubble::getInGameStatus()
{
	return inGameStatus;
}

/* Retourne la vitesse en X d'une bulle donnée
=============================================== */
int Bubble::getVelocityX() 
{
	return velocityX;
}

/* Retourne la vitesse en Y d'une bulle donnée
=============================================== */
int Bubble::getVelocityY()
{
	return velocityY;
}

/* Retourne la hitbox d'une bulle donnée
=============================================== */
Bubble::Circle Bubble::getHitbox() 
{
	return hitbox;
}

/* Assigne à la bulle un sprite donné et une couleur
===================================================== */
void Bubble::setSprite(int colorValue)
{
	spriteSheet = loadBitmap("bubble.bmp");
	color = colorValue;
	initCharset(sprite, NB_COLORS);
	inGameStatus = true;
}

/* Calcule et assigne à la bulle une vitesse en X et en Y
============================================================= */
void Bubble::setVelocity(int velX, int velY) 
{
	//TODO: Calculs dans cette section pour trouver la bonne vélocité. Va falloir faire un peu de maths.
	velocityX = velX;
	velocityY = velY;
}

/* Met en place la hitbox de la bulle
================================================= */
void Bubble::setHitbox(SDL_Rect &position, int radius) 
{
	hitbox.radius = BUBBLE_RADIUS;	//le rayon du cercle
	
	//le centre du cercle en pixels
	hitbox.centerX = position.x + BUBBLE_RADIUS;
	hitbox.centerY = position.y + BUBBLE_RADIUS;
}

/* Bouge une bulle dans l'espace et vérifie s'il y a des collisions
==================================================================== */
void Bubble::move() 
{
	//si la bulle n'a pas de vélocité, elle ne bouge pas.
	if (velocityX == 0 && velocityY == 0)
	{
		return;
	}

	if (hasCollidedWithWall())
	{
		velocityX *= -1;
	}

	position.x += velocityX;
	position.y += velocityY;
}

/* Affiche la bulle à une position donnée dans l'écran
======================================================== */
void Bubble::update(SDL_Surface *screen) 
 {
	 if (inGameStatus)		//si les bulles sont bien en jeu
	 {
		/*setPosition(spriteSheet, position, x, y);*/
		move();
		setHitbox(position, BUBBLE_RADIUS);
		setTransparency(spriteSheet, 255, 255, 255);
		updateScreenWithSpriteSheet(screen, spriteSheet, sprite[color], position);
	 }
	
}

/* Vérifie la collision entre deux bulles
=========================================== */
bool Bubble::checkCollisions(Bubble* otherBubble)
{
	double distance;		//la distance entre deux bulles

	if (inGameStatus) 
	{
		//Calcule la distance entre les centres du cercle
		distance = calculateDistance(hitbox.centerX, otherBubble->hitbox.centerX, hitbox.centerY, otherBubble->hitbox.centerY);

		//Il y a collision si la distance entre les centres du cercles est plus petite que la somme de leurs rayons
		if (distance < (hitbox.radius + otherBubble->hitbox.radius))
		{
			return true;
		}
	}

	return false;

}

bool Bubble::hasCollidedWithWall()
{
	//Si elle touche le haut de l'écran, la vélocité en Y devient 0
	if (position.y < 0)
	{
		velocityY = 0;
		velocityX = 0;
		return true;
	}

	return (position.x + position.w > SCREEN_WIDTH) || (position.x < 0);
}

void Bubble::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}