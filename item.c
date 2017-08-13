#include <stdio.h>
#include "item.h"

/* inserção no início: retorna a lista atualizada */
ListaItens* insereItem (ListaItens* list, Item item)
{
	ListaItens* novoItem = (ListaItens*) malloc(sizeof(ListaItens));
	novoItem->item = item;
	novoItem->prox = list;
	return novoItem;
}

/* função vazia: retorna 1 se vazia ou 0 se não vazia */
bool estaVazia (ListaItens* l)
{
	if (l == NULL)
		return true;
	else
		return false;
}

void liberaLista (ListaItens* l)
{
	ListaItens * p = l;
	while (p != NULL) {
	ListaItens* t = p->prox; /* guarda referência para o próximo elemento
	*/
	free(p);
	/* libera a memória apontada por p */
	p = t;
	/* faz p apontar para o próximo */
	}
}

ListaItens * removeItem(ListaItens* l, Item item){
	ListaItens * ant = NULL; 
	ListaItens * aux = l; 
	int strsDif;

	while (aux != NULL){
 
		//strDif = 0 se forem iguais e strDif!= 0 se forem diferentes
		strsDif = strcmp(aux->item.id, item.id);
		if (strsDif == 0){
			break;
		}
		else {
			ant = aux; 
			aux = aux->prox;
		}
	}
	/* verifica se encontrou o item */
	if (aux == NULL){
		return l; // o elemento nao esta na lista
	}
	/* remove o item */
	if (ant == NULL){
		//remove o elemento do inicio da lista
		l = aux-> prox; 
	}else {
		//remove o elemento do meio da lista
		ant->prox = aux->prox; 
	}
	free(aux);

	return l;
}

/* função busca: busca um elemento na lista */
bool buscaItem (ListaItens * l, char * id)
{
	ListaItens* aux;
	printf("Buscando o elemento: %s\n", id);
	for (aux = l; aux != NULL; aux = aux->prox)
		if (strcmp(aux->item.id, id) == 0){
			printf("achei!\n");
			return true;
		}
    printf("nao achei!\n");
	return false;
	/* não achou o elemento */
}

