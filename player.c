#include <allegro5/allegro.h>                     
#include <allegro5/allegro_image.h>              
#include <allegro5/allegro_native_dialog.h>
#include "fileLoader.h"
#include <stdio.h>
#include "render.h"
#include "eventListener.h"
#include "fileLoader.h"
#include "player.h"
#include "rect.h"
#include "item.h"
#include "puzzles.h"
//Declaração de variaveis globais
Player jogador; 
ListaItens * itensColetados = NULL; 
int qtdItensCol = 0; 
enum KEYS {UP, DOWN, RIGHT, LEFT};
int i = 0, j = 0, jogH, jogW, jogX, jogY; 
//Informa se o jogador esta se movendo
bool movimentando = false;
bool colidiuCollider = false; 
int itemColetadoNA = -1;

Craft craftColidido; 
bool areaCraft = false;

void initPlayer (int larguraJanela, int alturaJanela)
{
	jogador.spritePlayer = getPlayerImage();
	jogador.position.x = 300;
	jogador.position.y = 600;
	jogador.direcao = 0;
}

void playerMovement ()
{
	bool *keys = getActivekeys(); 
	//Posicao anterior do jogador
	float oldX = jogador.position.x; 
	float oldY = jogador.position.y; 

	//Atualiza a posicao do player
	if (keys[UP] == true && keys[DOWN] == false && keys[LEFT] == false && keys[RIGHT] == false)
	{
		jogador.position.y -= keys[UP] * 2; 
		jogador.direcao = 3; // cima
		movimentando = true;

	}else if (keys[DOWN] == true && keys[UP] == false && keys[LEFT] == false && keys[RIGHT] == false)
	{
		jogador.position.y += keys[DOWN] * 2;
		jogador.direcao = 0; // cima
		movimentando = true;

	}else if (keys[LEFT] == true && keys[DOWN] == false && keys[UP] == false && keys[RIGHT] == false)
	{
		jogador.position.x -= keys[LEFT] * 2;
		jogador.direcao = 1; // esquera
		movimentando = true;

	}else if (keys[RIGHT] == true && keys[DOWN] == false && keys[LEFT] == false && keys[UP] == false)
	{
		jogador.position.x += keys[RIGHT] * 2; 
		jogador.direcao = 2; // direita
		movimentando = true;
	}
	if (keys[UP] == false && keys [DOWN] == false && keys[RIGHT] == false && keys[LEFT] == false){
		//jogador esta parado
		movimentando = false;
	}
	//Os valores 25, 20 e 10 sao ajustes parar ignorar os espaços vazios da imagem
	jogH = al_get_bitmap_height(jogador.spritePlayer)/4 - 25;
	jogW = al_get_bitmap_width(jogador.spritePlayer)/4 - 20;
	jogX = jogador.position.x + 10; 
	jogY = jogador.position.y + 25;

	//Se a nova posicao do jogador colidir com um colisor
	//Voltamos a posicao antiga
	if (movimentando){
		if (onCollision(jogH, jogW, jogX, jogY) || onCollisionPorta()){
			jogador.position.x = oldX; 
			jogador.position.y = oldY;
			movimentando = false;
			colidiuCollider = true; 
		}else{
			colidiuCollider = false;
		}

		//Verifica se o player colidiu com algum item
		//onCollisionItem(jogH, jogW);
		onCollisionListaItens();
	}
	onCollisionCraft();
}

void onCollisionListaItens(){
	bool colidiu = false;
	ListaItens * listaItens = getItensCena();
	ListaItens* aux; 
	for (aux = listaItens; aux != NULL; aux = aux->prox){
		if (jogX + jogW > aux->item.x1 && 
			jogY + jogH > aux->item.y1)
		{
			if (jogX < aux->item.x2 &&
				jogY < aux->item.y2)
			{
				//printf("Voce encontrou o item: %s\n",aux->item.id);
				itemColetadoNA = aux->item.num;
				coletarItem(aux->item); 
				movimentando = false;
			}
		}	
	}
}

bool onCollision(){
	bool colidiu = false; 
	Rect *colliders = getCollidersMap();
	int lines = getLinesCollidersMap();
	for (i = 0; i < lines; i++)
	{
		if (jogX + jogW > colliders[i].x1 && 
			jogY + jogH > colliders[i].y1)
		{
			if (jogX < colliders[i].x2 &&
				jogY < colliders[i].y2)
			{
				colidiu = true;
				setDialogo (colliders[i].id_dialogo);
				break;
			}
		}
	}
	return colidiu;
}
bool onCollisionPorta(){
	bool * puzzles = getPuzzles();
	if (!puzzles[1]){
	Porta * p = getPorta();
		ALLEGRO_BITMAP * img = (*p).sprite;
			float x2= al_get_bitmap_width(img) + (*p).x;
			float y2 = al_get_bitmap_height(img) + (*p).y;
			if (jogX + jogW > (*p).x&& 
				jogY + jogH > (*p).y)
			{
				if (jogX < x2 &&
					jogY < y2)
				{
					printf("bateu na porta!\n");
					return true;
				}
			}
			return false;
	}	
}
void onCollisionCraft(){
	areaCraft = false;
	//150 600 200 650
	for (i = 0; i < 2; i++)
	{
		if (jogX + jogW > 150 && 
			jogY + jogH > 600)
		{
			if (jogX < 200 &&
				jogY < 650)
			{
				areaCraft = true;
				//printf("esta na area\n");
				break;
			}
		}
	}

	if (areaCraft){
		//printf("esta na area\n");
	}else{
		//printf("NAO esta na area\n");
	}
}

void coletarItem (Item item){
	ListaItens * listaItens = getItensCena();
	ListaItens * aux = NULL;
	//Insere o item encontrado na lista de itens coletados
	itensColetados = insereItem(itensColetados, item); 
	//Remove o item encontrado da lista dos itens da cena
	aux = removeItem(listaItens, item);
	//Atualiza a lista do itens da cena
	setItensCena (aux);
	
	Elemento * elementos = getElementos();

	for (int i = 0; i < getQtdElementos(); i++){
		int strsDif;
		//1 se a id dos itens forem diferentes e 0 se forem iguais
		strsDif = strcmp(elementos[i].id, item.id);
		if (!strsDif){
			elementos[i].status = 1; //elemento foi desbloqueado da tabela
			//printf("O elemento: %s foi desbloqueado %d\n", elementos[i].id, elementos[i].status);
		}
	}
	qtdItensCol++; 
}
Player * getPlayer()
{
	return &jogador;
}

void desalocarItensColetados(){
	liberaLista(itensColetados);
}

ListaItens * getItensColetados (){
	return itensColetados;
}

int getQtdItensColetados(){
	return qtdItensCol;
}

bool getMovimentando (){
	return movimentando;
}

bool getColidiuCollider(){
	return colidiuCollider;
}
int getItemColetadoNA(){
	return itemColetadoNA;
}
void setItemColetadoNA(int valor){
	itemColetadoNA = valor;
}

bool podeCraftar(){
	return areaCraft;
}


