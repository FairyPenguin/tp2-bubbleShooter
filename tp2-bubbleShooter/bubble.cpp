#include "bubble.h"


/* Constructeur de la classe "Bubble"
====================================== */
Bubble::Bubble()	//constructeur par défaut
{
	inGameStatus = false;
	velocityX = 0;
	velocityY = 0;
	position.w = 47;
	position.h = 47;
}

Bubble::Bubble(int posX, int posY)	//constructeur assignant une position donnée à la bulle
{
	inGameStatus = false;
	position.x = posX;
	position.y = posY;
	velocityX = 0;
	velocityY = 0;
	position.w = 47;
	position.h = 47;
}

/* Destructeur de la classe "Bubble" 
====================================== */
Bubble::~Bubble()
{
	//Libérer la surface de la bulle seulement si elle n'Eest pas déjà en jeu
	if (inGameStatus)
	{
		SDL_FreeSurface(spriteSheet);
	}
	
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
	initCharset();
	inGameStatus = true;
}

/* Calcule et assigne à la bulle une vitesse en X et en Y
============================================================= */
void Bubble::setVelocity(int velX, int velY) 
{
	velocityX = velX;
	velocityY = velY;
}

/* Calcule et assigne à la bulle une vitesse en X et en Y selon l'angle du canon
============================================================= */
void Bubble::setVelocity(int angle)
{
	double angleRadian = (angle + 180) * PI / 180;		//l'angle en radian est donné par l'angle du canon
														//auquel on applique une rotation de 180 degrés,
														//considérant que rotozoom tourne dans le sens horaire.

	velocityX = SPEED_BUBBLE * sin(angleRadian);
	velocityY = SPEED_BUBBLE * cos(angleRadian);
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

	//si la bulle touche un mur, inverser la vélocité en X pour la faire aller de l'autre sens.
	if (hasCollidedWithWall())
	{
		velocityX *= -1;
	}

	//Déplacement dans l'espace et réinitialisation de la hitbox
	position.x += velocityX;
	position.y += velocityY;
}

/* Affiche la bulle à une position donnée dans l'écran
======================================================== */
void Bubble::update(SDL_Surface *screen) 
 {
	 if (inGameStatus)		//si les bulles sont bien en jeu
	 {
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
		if (distance <= (hitbox.radius + otherBubble->hitbox.radius))
		{
			return true;
		}
	}

	return false;

}

/* Vérifie la collision entre une bulle et le mur
=================================================== */
bool Bubble::hasCollidedWithWall()
{
	//Si elle touche le haut de l'écran, la vélocité en Y devient 0
	if (position.y <= 0)
	{
		velocityY = 0;
		velocityX = 0;
		return true;
	}

	//Vérifie si la bulle touche les murs de côté et retourne si la condition est vraie ou fausse
	return (position.x + position.w >= SCREEN_WIDTH) || (position.x <= 0);
}

/* Assigne une position à une bulle donnée selon des valeurs passées en paramètre
=================================================================================== */
void Bubble::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

/* Assigne une position à une bulle donnée à partir de la position d'un autre objet
=================================================================================== */
void Bubble::setPosition(SDL_Rect otherPosition)
{
	position.x = otherPosition.x;
	position.y = otherPosition.y;
}

/* Assigne le statut en-jeu ou non à une bulle donnée
======================================================= */
void Bubble::setInGameStatus(bool status) 
{
	inGameStatus = status;
}

/* Initialise le charset des bulles dans le tableau sprite[]
============================================================== */
void Bubble::initCharset() 
{
	for (int x = 0; x < NB_COLORS; x++)		//pour chaque ligne de l'image
	{
		sprite[x].x = x * 47;		//position du posCharset sur l'axe des X
		sprite[x].y = 0;			//position de posCharset sur l'axe des Y
		sprite[x].w = 47;			//largeur en pixels du rectangle
		sprite[x].h = 47;			//hauteur en pixels du rectangle
	}
}