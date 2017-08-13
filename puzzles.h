#include <allegro5/allegro.h>    
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#ifndef PUZZLES_H_INCLUDED
#define PUZZLES_H_INCLUDED

typedef struct craft
{ 
	int id;              //Numero do puzzle
	char solucao[50];    //Solucao do puzzle
}Craft;

typedef struct porta
{
	int x; 
	int y; 
	ALLEGRO_BITMAP * sprite;
}Porta; 
extern void gerenciadorPuzzles (bool * WINDOWS, ALLEGRO_FONT * fonte);
extern bool * getPuzzles ();
extern void initPorta();
extern Porta * getPorta();
extern int getMsgAlertaPuzzle();
#endif 
