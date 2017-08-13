#include <allegro5/allegro.h>                     
#include <allegro5/allegro_image.h>             
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fileLoader.h"
#include "rect.h"
#include "item.h"
#include "dialogo.h"
#include "puzzles.h"
#include <string.h>
#include <stdbool.h>
#define QTD_DIALOGOS 7

//quantidade de elementos da tabela periodica
#define QTD_ELE 90
//quantidade de slots da tabela periodica (somatoria dos elementos + slots vazios)
#define QUAD_TAB 126 
//quantidade de colunas do arquivo dos colliders
int const cols = 4; //{x, y, h, w}
//Quantidade de linhas arquivo dos colliders
int linesCollider = 0; 

//Sprites
ALLEGRO_BITMAP * spritePlayer = NULL; 
ALLEGRO_BITMAP * brilho = NULL;
ALLEGRO_BITMAP * inventory = NULL; 
ALLEGRO_BITMAP * spriteElementos [QTD_ELE]; 
Dialogo dialogosObj [QTD_DIALOGOS];
ALLEGRO_BITMAP * elementosINFO [QTD_ELE];
ALLEGRO_BITMAP * cena1 = NULL;
ALLEGRO_BITMAP * layerEscura = NULL;
ALLEGRO_BITMAP * spritePorta = NULL;
ALLEGRO_BITMAP * spriteLousa = NULL;
//Arquivos de texto
FILE *collidersFile	 = NULL;
FILE *itensFile = NULL;
FILE *elementosFile = NULL;
FILE *respostasPuzzlesFile = NULL;
//Estruturas de dados 
Rect *collidersArray = NULL;            //colliders
ListaItens * listaItens = NULL;         //itens da cena
Elemento elementos [QUAD_TAB];           //slots da tabela
Craft respostasPuzzles [2];
void loadingImages ()
{

	// Inicializa o add-on para utilização de imagens
	al_init_image_addon();

	if (!al_init_image_addon){
		fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
	}

	// Carrega o sprite player
	spritePlayer = al_load_bitmap("Images/Player/Player2.png");
	if (!spritePlayer){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (player).\n");	
	}

	//Carregando as imagens do inventario
	brilho = al_load_bitmap("Images/Inventario/brilho.png");
	if (!brilho){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (brilho).\n");	
	}

	//Carregando as imagens do inventario
	inventory = al_load_bitmap("Images/Inventario/InventarioBG.png");
	if (!inventory){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (inventario).\n");	
	}

	//Carregando o background da cena 1
	cena1 = al_load_bitmap("Images/Cena1/Cena.png");
	if (!cena1){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (bg cena 1).\n");	
	}

	//Carregando a layer
	layerEscura = al_load_bitmap("Images/Cena1/LayerEscura.png");
	if (!layerEscura){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (layer escura).\n");	
	}
	//Carregando a layer
	spriteLousa = al_load_bitmap("Images/Craft/Lousa.png");
	if (!spriteLousa){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (lousa).\n");	
	}

	spritePorta = al_load_bitmap("Images/Cena1/Porta.png");
	if (!spritePorta){
	    fprintf(stderr, "Falha ao carregar o arquivo de imagem (porta).\n");	
	}



	char endereco [100] = "";
	//Icones da tabela
	for (int i = 0; i < QTD_ELE; i++){
		sprintf(endereco, "%s%d%s", "Images/Inventario/Elementos/Elemento", i, ".png");
	    spriteElementos [i] = al_load_bitmap (endereco);
	    if (!spriteElementos[i])
	    {
	    	fprintf(stderr, "Falha ao carregar imagem do elemento %d\n",i);	
	    }
	}
	//Imagens de dialogos
	//limpando a string endereco
	strcpy(endereco, "");
	for (int i = 0; i < QTD_DIALOGOS; i++){
			sprintf(endereco, "%s%d%s", "Images/Dialogos/Interacao/Dialogo", i, ".png");
		    dialogosObj[i].img = al_load_bitmap (endereco);
		    dialogosObj[i].id = i;
		    if (!dialogosObj[i].img)
		    {
		    	fprintf(stderr, "Falha ao carregar imagem do dialogo %d\n",i);	
		    }	
	}

	//Carregando as informacoes de cada elemento da tabela
	strcpy(endereco, "");
	for (int i = 0; i < QTD_ELE; i++){
		sprintf(endereco, "%s%d%s", "Images/Dialogos/Desbloqueados/Elemento", i+1, ".png");
		elementosINFO[i] = al_load_bitmap (endereco);

		if (!elementosINFO[i])
		{
			fprintf(stderr, "Falha ao carregar imagem da info do elemento %d\n",i);	
		}
	}
}

int contarNumLinhas(FILE * file)
{
	int i = 0;
	char ch;
	if (file == NULL)
	{
		printf("O arquivo nao pode ser aberto.\n");
	}else
	{
		//Contando o numeros de linhas do arquivo
		while( (ch=fgetc(file))!= EOF )
		{
			if(ch == '\n')
		 		i++;
		}
	}
	return i; 
}

//Funcao que le o arquivo de itens da cena
void loadingItens(){
	Item novoItem; 
	//Abrindo o arquivo
	itensFile = fopen("itens.txt", "r");

	//Armazenando as coordenadas dos colliders em cada elemento do tipo Rect no vetor de colliders
	while((fscanf(itensFile,"%s %d %f %f %f %f\n", 
		novoItem.id, &novoItem.num, &novoItem.x1, &novoItem.y1, &novoItem.x2, &novoItem.y2))!= EOF)
	{
		listaItens = insereItem(listaItens, novoItem);
	}	
	fclose (itensFile); 
}
//Funcao que le o arquivo de Elementos da tabela periodica
void loadingElements()
{
	int i = 0;
	//Abrindo o arquivo
	elementosFile = fopen("Elementos.txt", "r");

	while((fscanf(elementosFile,"%s %d %d\n", 
		elementos[i].id, &elementos[i].num, &elementos[i].status))!= EOF)
	{
		//printf ("Elemento: %s %d %d %d \n", elementos[i].id, elementos[i].lin, elementos[i].col, elementos[i].status);
		i++;		
	}	
	fclose (elementosFile);
}

void loadingRespostasPuzzles(){
	int i = 0; 
	respostasPuzzlesFile = fopen("Respostas.txt", "r");

	while((fscanf(respostasPuzzlesFile,"%d %s\n", &respostasPuzzles[i].id, respostasPuzzles[i].solucao ))!= EOF)
	{
		i++;		
	}	
	fclose (respostasPuzzlesFile); 
}

void loadingColliders()
{
	int i = 0;
	//Abrindo o arquivo
	collidersFile = fopen("Colliders.txt", "r");
	linesCollider = contarNumLinhas(collidersFile);

	//Alocando o array de ponteiros do tipo rect
	collidersArray = (Rect*)malloc(linesCollider * sizeof(Rect)); 

	//Retornando para o inicio do arquivo
	rewind (collidersFile);
	//Armazenando as coordenadas dos colliders em cada elemento do tipo Rect no vetor de colliders
	while((fscanf(collidersFile,"%f %f %f %f %d\n", 
		&collidersArray[i].x1, &collidersArray[i].y1, &collidersArray[i].x2, &collidersArray[i].y2, &collidersArray[i].id_dialogo))!= EOF)
	{
		i++;
		//printf ("%f %f %f %f \n", collidersArray[i].x1, collidersArray[i].y1, collidersArray[i].x2, collidersArray[i].y2);
	}	
	fclose (collidersFile); 
}

void desalocarMemoria(){
	al_destroy_bitmap(spritePlayer);
	al_destroy_bitmap(inventory);
	al_destroy_bitmap(brilho);
	al_destroy_bitmap(layerEscura);
	al_destroy_bitmap(spritePorta);
	al_destroy_bitmap(spriteLousa);
	//Elementos da tabela periodica
	for (int i = 0; i < QTD_ELE; i++)
	{
		al_destroy_bitmap(spriteElementos[i]); 
	}
	//Dialogos
	for (int i = 0; i < QTD_DIALOGOS; i++)
	{
		al_destroy_bitmap(dialogosObj[i].img); 
	}
	//Colliders
	free (collidersArray);
	//Itens coletaveis
	liberaLista(listaItens);
}

	
ALLEGRO_BITMAP * getPlayerImage ()
{
	return spritePlayer;
}
ALLEGRO_BITMAP * getPortaSprite ()
{
	return spritePorta;
}
ALLEGRO_BITMAP * getInventoryImage(){
	return inventory;
}
ALLEGRO_BITMAP * getBrilhoImg(){
	return brilho;
}
ALLEGRO_BITMAP * getCena1(){
	return cena1;
}

ALLEGRO_BITMAP * getLayerEscura(){
	return layerEscura;
}
ALLEGRO_BITMAP * getSpriteLousa(){
	return spriteLousa;
}

Rect * getCollidersMap(){
	return collidersArray;
}

ListaItens * getItensCena(){
	return listaItens;
}

void setItensCena(ListaItens * l){
	listaItens = l; 
}

int getLinesCollidersMap(){
	return linesCollider;
}

int getColsCollidersMap(){
	return cols; 
}

Elemento * getElementos(){
	return elementos; 
}

int getQtdElementos(){
	return QUAD_TAB; 
}

ALLEGRO_BITMAP ** getSpritesElementos(){
    return spriteElementos;
}
ALLEGRO_BITMAP ** getElementosINFO(){
	return elementosINFO;
}
Dialogo * getDialogos(){
	return dialogosObj;
}
Craft * getRespostasPuzzles(){
	return respostasPuzzles;
}