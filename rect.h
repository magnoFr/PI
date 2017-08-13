//Cabeçalhos das funcoes implementadas no rect.c
#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

typedef struct rect 
{
	//coordenadas do bloco na tela
	float x1; 
	float x2; 
	float y1; 
	float y2;
	//id do dialogo referente a esse bloco
	//Se a id for igual a -1 nao existe dialogo
	int id_dialogo;
}Rect; 

#endif //fim da definicao de RECT_H

