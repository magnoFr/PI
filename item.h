#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

//Item coletáveis do cenario (ex. NaCl, H2O)
typedef struct item
{ 
	//Coordenadas x e y da posicao onde está no cenario
	float x1;
	float y1; 
	float x2;
	float y2; 
	int num; 
	char id[50];    //Identificacao do item
}Item;

//Elemento da tabela periodica (ex: H, N, O, Na)

typedef struct elemento{
	char id[50]; 	//Identificacao do elemento
	int num;        //Numero atomico do elemento
	int status;      //status: bloqueado ou desbloqueado 
}Elemento; 

struct listaItens {
	Item item;
	struct listaItens* prox;
};
typedef struct listaItens ListaItens;
extern ListaItens* insereItem (ListaItens* list, Item item);
extern bool estaVazia (ListaItens* list);
extern ListaItens* removeItem(ListaItens* l, Item item);
extern void liberaLista (ListaItens* l); 
extern bool buscaItem (ListaItens* l, char * v);
#endif 
