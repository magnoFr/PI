#include "item.h"
#include "puzzles.h"
//Cabeçalhos das funcoes implementadas no player.c
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

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
	int direcao; //0 - baixo, 1 - esquerda, 2 - direita, 3 - cima
}Player;

//Inicializa o objeto do tipo jogador
extern void initPlayer (int larguraJanela, int alturaJanela);
//Retorna o ponteiro do tipo player
extern Player *getPlayer ();
//Funcao responsavel pelo movimento do jogador
extern void playerMovement ();
extern void coletarItem (Item item);
//Funcao responsavel por detectar se o player esta colidindo com algum colisor
extern bool onCollision();
extern void onCollisionCraft();
extern void onCollisionListaItens();
extern bool getColidiuCollider();
extern void desalocarItensColetados();
extern ListaItens * getItensColetados ();
extern int getQtdItensColetados();
extern bool getMovimentando ();
extern int getItemColetadoNA();
extern void setItemColetadoNA();
extern Craft getCraftColidido();
extern bool podeCraftar();
extern bool onCollisionPorta();
#endif //fim da definicao de PLAYER_H
