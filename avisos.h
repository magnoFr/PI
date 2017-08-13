#ifndef AVISOS_H_INCLUDED
#define AVISOS_H_INCLUDED
struct listaAvisos{
	int id; 
	struct listaAvisos * prox; 
}; 
typedef struct listaAvisos ListaAvisos;

ListaAvisos * insereAviso (ListaAvisos * l, int id);
int existeAviso (ListaAvisos * l);
ListaAvisos * removeAviso (ListaAvisos *l, int id);
#endif