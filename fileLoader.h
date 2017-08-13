#include <allegro5/allegro_image.h>              /* add-on para uso de imagens */
#include "rect.h"
#include "item.h"
#include "dialogo.h"
#include "puzzles.h"
#include <stdio.h>
#include <stdlib.h>
//Cabeçalhos das funcoes implementadas no fileLoader.c
#ifndef FILELOADER_H_INCLUDED
#define FILELOADER_H_INCLUDED

/******** Funcoes de carregar arquivos do tipo txt ou imagem *********/
extern void loadingImages(); 
extern void loadingColliders(); 
extern void loadingItens(); 
extern void loadingElements();
extern void loadingRespostasPuzzles();

//Conta o numero de linhas de um arquivo txt
extern int contarNumLinhas(FILE * file);

/********* Getters and Setters **********/
extern ALLEGRO_BITMAP* getPlayerImage (); 
extern ALLEGRO_BITMAP ** getSpritesElementos();
extern Dialogo * getDialogos();
extern Rect * getCollidersMap();                            //Retorna vetor do tipo Rect (x1, x2, y1, y2)
extern ListaItens * getItensCena();                        //Retorna lista do tipo listaItens (id, x1, x2, y1, y2)
extern int getLinesCollidersMap(); 
extern ALLEGRO_BITMAP * getInventoryImage();                //imagem de fundo do inventario
extern void setItensCena(ListaItens * l);
extern Elemento * getElementos();
extern int getQtdElementos();
extern void desalocarTabelaBG();
extern void desalocarElementos();
extern ALLEGRO_BITMAP * getCena1();
extern ALLEGRO_BITMAP ** getElementosINFO();
extern ALLEGRO_BITMAP * getInventoryImage();
extern ALLEGRO_BITMAP * getBrilhoImg();
extern ALLEGRO_BITMAP * getLayerEscura();
extern ALLEGRO_BITMAP * getPortaSprite ();
extern ALLEGRO_BITMAP * getSpriteLousa();
extern Craft * getRespostasPuzzles();
/******** Funcoes responsaveis por liberar memoria utilizada *********/

extern void desalocarMemoria ();

#endif //fim da definicao de FILELOADER_H
