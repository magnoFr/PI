#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef struct rect 
{
	float x1; 
	float x2; 
	float y1; 
	float y2; 
}Rect; 

typedef struct coordinates
{ 
	float x;
	float y; 
}Coordinates;

typedef struct player
{ 
	ALLEGRO_BITMAP *spritePlayer;
	Coordinates position;
	int life; 
}Player;

#endif //fim da definicao de STRUCTS_H