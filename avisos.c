#include "avisos.h"
#include <stdlib.h>

ListaAvisos * insereAviso (ListaAvisos * l, int id){
	ListaAvisos * novo = (ListaAvisos *)malloc(sizeof(ListaAvisos));
	novo->id = id;
	novo->prox = l;
	return novo;
}

int existeAviso (ListaAvisos * l){
	if (l == NULL)
		return 0;
	else
		return 1;
}

ListaAvisos * removeAviso (ListaAvisos * l, int id){
	ListaAvisos * ant = NULL; 
	ListaAvisos * aux = l;

	while (aux != NULL && aux->id != id){
		ant = aux;
		aux = aux->prox;
	}

	if (aux == NULL){
		return l;
	}

	if(ant == NULL){
		l = aux->prox;
	}else{
		ant->prox = aux->prox;
	}
	free(aux);
	return l;
}