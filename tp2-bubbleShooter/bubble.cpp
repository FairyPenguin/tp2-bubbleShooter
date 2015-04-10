#include "bubble.h"

/* Constructeur de la classe "Bubble"
====================================== */
Bubble::Bubble()
{
	inGameStatus = false;
}

/* Destructeur de la classe "Bubble" 
====================================== */
Bubble::~Bubble()
{
	//Libérer la surface des bulles de la mémoire
	SDL_FreeSurface(spriteSheet);
}

/*Retourne la position d'une bulle donnée
========================================== */
SDL_Surface* Bubble::getSpriteSheet()
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
void Bubble::move(bool &hasCollided) 
{
	//Calcule et assigne à la bulle sa vélocité de base
	//*********************************** TESTÉ ICI AVEC DES VALEURS ARBITRAIRES!!! ***********************************************
	setVelocity(-3, 3);

	//Si la position en X touche les murs de l'écran,  la vélocité en X s'inverse (multipliée par -1) et la bulle recule pour être collée au bord

	if (position.x + position.w > SCREEN_WIDTH)		//si la balle touche le mur de droite
	{
		int newVelocity = velocityX * (-1);
		velocityX = newVelocity;

	}
	else if (position.x < 0)				//si la balle touche le mur de gauche
	{
		int newVelocity = velocityX * (-1);
		velocityX = newVelocity;

	}

	//Si elle touche le haut de l'écran, la vélocité en Y devient 0
	if (position.y < 0)
	{
		velocityY = 0;
		hasCollided = true;
	}

	//Bouge la bulle dans l'espace
	position.x += velocityX;
	position.y += velocityY;
}

/* Affiche la bulle à une position donnée dans l'écran
======================================================== */
void Bubble::update(SDL_Surface *screen, int x, int y) 
 {
	 if (inGameStatus)		//si les bulles sont bien en jeu
	 {
		setPosition(spriteSheet, position, x, y);
		setHitbox(position, BUBBLE_RADIUS);
		setTransparency(spriteSheet, 255, 255, 255);
		updateScreenWithSpriteSheet(screen, spriteSheet, sprite[color], position);
	 }
	
}

/* Vérifie la collision entre deux bulles
=========================================== */
bool Bubble::checkCollisions(Bubble otherBubble)
{
	double distance;		//la distance entre deux bulles
	bool hasCollision;		//flag déterminant s'il y a eu collision

	hasCollision = false;	//initialisation du flag

	//Calcule la distance entre les centres du cercle
	distance = calculateDistance(hitbox.centerX, otherBubble.hitbox.centerX, hitbox.centerY, otherBubble.hitbox.centerY);

	//Il y a collision si la distance entre les centres du cercles est plus petite que la somme de leurs rayons
	if (distance < (hitbox.radius + otherBubble.hitbox.radius))
	{
		hasCollision = true;
	}
	else
	{
		hasCollision = false;
	}

	return hasCollision;

}