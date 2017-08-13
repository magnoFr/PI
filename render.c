#include <allegro5/allegro.h>                     /* bibilioteca Allegro 5 */
#include  <allegro5/allegro_image.h>              /* add-on para uso de imagens */
#include  <allegro5/allegro_primitives.h>
#include  <stdio.h>
#include  "player.h"
#include  "fileLoader.h"
#include  "item.h"
#include "rect.h"
#include "eventListener.h"
#include "avisos.h"
#include "puzzles.h"
//Declaracao de variaveis constantes
#define WIDTH 1280
#define HEIGHT 720
//Quantidade de slots da tabela periodica (slots com imagens dos elementos + slots vazios)
#define QTD_SLOTS 126
//Margem inicial dos elementos da tabela periodica
#define TOP 236
#define RIGHT 228
//(Espacamento entre os elementos + largura da imagem do elemento)
#define INC_X 45
#define INC_Y 52
//Quantidade de colunas da tabela periodica
#define TAB_COL 18
//Quantidade de dialogosExibidos
#define QTD_DIALOGOS 2

// Telas do jogo
enum WINDOWS {GAME, INVENTORY, DIALOG, INFOS, CRAFT};
//Quantidade de frames por movimentos 
const int maxFrame = 3; 
//Quantidade dos tipos de movimentos
const int moveTypes = 4;
//Frame atual
int currentFrame = 0;
//Contador de Frames
int counter = 0;
//Tempo de troca entre os frames 
int delay = 5;

//id do dialogo que foi solicitado para ser exibido;
int id_dialogo = -1;
//vetor de dialogosExibidos ja exibidos
int dialogosExibidos [QTD_DIALOGOS];
//primeira chamada do draw dialogosExibidos
bool firstCall = true; 

//contador de tempo dos avisos
int counter2 = 0;
//tempo atual
int current = 0; 
//tempo de espera entre os avisos
int delayAvisos = 400;
//Lista de avisos
ListaAvisos * avisos = NULL;

//Mensagens de alerta
char str0 [] = "Resposta correta!";
char str1 [] = "Resposta errada!";
char str2 [] = "Elementos bloqueados.";
char str3 [] = "Elementos bloqueados.";
void drawPlayer (){

	Player *p; 
	p = getPlayer();
	ALLEGRO_BITMAP * imagemPlayer =  (*p).spritePlayer;
	float larguraSpriteSheet = al_get_bitmap_width(imagemPlayer);
	float alturaSpriteSheet = al_get_bitmap_height(imagemPlayer);
	float larguraFrame = larguraSpriteSheet/maxFrame;
	float alturaFrame = alturaSpriteSheet/moveTypes;
	//Tempo de troca entre os frames 
	int delay = 5;
	if (getMovimentando()){
		//Se o tempo de espera ja foi alcancado
		if (counter >= delay){

			//Ir para proximo frame
			currentFrame++;
			//Reinicia o contador
			counter = 0;
			//Reinicia a sequencia de frames
			if (currentFrame >= maxFrame){
				//voltar para o frame inicial
				currentFrame = 0; 
			}
		}
		//incrementa contador
		counter++;
	}else{
		currentFrame = 1;
	}

	int linhaSpriteSheet = (*p).direcao; 

	//Desenha a imagem do player na tela
	al_draw_bitmap_region((*p).spritePlayer, currentFrame * larguraFrame, linhaSpriteSheet * alturaFrame, 
		larguraFrame, alturaFrame, (*p).position.x, (*p).position.y, 0);
	
}

void drawPorta(){
	Porta *p;
	p = getPorta();
	ALLEGRO_BITMAP * img = (*p).sprite;

	al_draw_bitmap((*p).sprite, (*p).x, (*p).y, 0);
}

void drawColliders (){
	int i = 0;
	int lines; 
	Rect * colliders = getCollidersMap();

	lines = getLinesCollidersMap();

	for (i = 0; i < lines; i++){
		// Retângulo: x1, y1, x2, y2, cor, espessura
		al_draw_rectangle(colliders[i].x1, colliders[i].y1, 
			colliders[i].x2, colliders[i].y2, al_map_rgb(255, 0, 0), 1.0);
	}
}

void drawAllItens (){

	ListaItens * listaItens = getItensCena();
	ListaItens* aux; 

	for (aux = listaItens; aux != NULL; aux = aux->prox)
	{
		ALLEGRO_BITMAP * brilho = getBrilhoImg();
		al_draw_bitmap(brilho, aux->item.x1, aux->item.y1, 0);
		al_draw_rectangle(aux->item.x1,aux->item.y1, aux->item.x2, aux->item.y2, 
			al_map_rgb(0, 0, 255), 1.0); 
	}
}
//Area onde o player tem que estar para poder acessar a janela de craft
void drawAreaPuzzles(bool * WINDOWS){

	for (int i = 0; i < 2; i++){
		//150 600 200 650
		//desenhando area de crafting
		al_draw_rectangle(150, 600, 200, 650, al_map_rgb(255, 0, 0), 1.0);
	}
}
//janela de craft
void drawSolutionField (bool * WINDOWS, ALLEGRO_FONT * fonte, int n){
	ALLEGRO_BITMAP * lousa = getSpriteLousa();
	float posX = al_get_bitmap_width(lousa)/2 + 127;
	float posY = al_get_bitmap_height(lousa)/2 + 62;
	//efeito visual na area de crafting
	al_draw_rectangle(150, 600, 200, 650, al_map_rgb(255, 0, 0), 6.0);
	//Desenha lousa
	al_draw_bitmap(lousa, 127, 62, 0);
	if (n == 0){
		char str [] = "Solução 1.";
		al_draw_text(fonte, al_map_rgb(255, 255, 255), 197, 82, ALLEGRO_ALIGN_CENTRE, str);
	}else if (n == 1){
		char str [] = "Solução 2.";
		al_draw_text(fonte, al_map_rgb(255, 255, 255), 197, 82, ALLEGRO_ALIGN_CENTRE, str);
	}
	//Exibe o texto que o player esta digitando enquanto ele nao aperta enter
	if (!getResposta ()){
		al_draw_text(fonte, al_map_rgb(255, 255, 255), posX, posY, ALLEGRO_ALIGN_CENTRE, getEntradaTxt());
	}else{
		int msgType = getMsgAlertaPuzzle();
		printf("mstYpe: %d\n", msgType);
		switch (msgType) {
			case 0:
				al_draw_text(fonte, al_map_rgb(255, 0, 0), posX, posY, ALLEGRO_ALIGN_CENTRE, str0);
			break;
			case 1:
				al_draw_text(fonte, al_map_rgb(255, 0, 0), posX, posY, ALLEGRO_ALIGN_CENTRE, str1);
			break;
			case 2:
				al_draw_text(fonte, al_map_rgb(255, 0, 0), posX, posY, ALLEGRO_ALIGN_CENTRE, str2);
			break;
			case 3:
				al_draw_text(fonte, al_map_rgb(255, 0, 0), posX, posY, ALLEGRO_ALIGN_CENTRE, str3);
			break;
		}
	}
	WINDOWS[CRAFT] = true;
}

void drawScene (){
	ALLEGRO_BITMAP * cena = getCena1();
	al_draw_bitmap(cena, 0, 0, 0);
}

void drawLayer(){

	ALLEGRO_BITMAP * layerEscura = getLayerEscura();
	al_draw_bitmap(layerEscura, 0, 0, 0);
}

void drawInventory(int x, int y)
{

	int posX = RIGHT; 
	int posY = TOP;
	int nColunas = 1;

	ALLEGRO_BITMAP * inventoryBG = getInventoryImage();
	Elemento * elementos = getElementos();
	// Desenhando a tabela do inventario no centro da tela
	al_draw_bitmap(inventoryBG, x, y, 0);

	ALLEGRO_BITMAP **imagens = getSpritesElementos();
	for (int i = 0; i < QTD_SLOTS; i++)
	{
	//verifica se o elemento foi desbloqueado
	//primeiro elemento
		if (i == 0)
		{
			if (elementos[i].status == 1)
			{
				al_draw_bitmap (imagens[i], RIGHT, TOP, 0);
			}
		}
		else{
		int strDif = 1;
		//Verifica se slot contem um elemento e nao um espaco vazio
		strDif = strcmp(elementos[i].id, "0");
		//Verifica se o elemento foi desbloqueado
 			if (strDif != 0 && elementos[i].status == 1)
 			{
				//indice da imagem no vetor de imagens dos elementos
				int j = elementos[i].num - 1;	
				//Desenha o elemento respectivo
				al_draw_bitmap(imagens[j], posX, posY, 0);
			}
		}
		//Incrementa a coordenada dos slots em x
		posX += INC_X;	
		if (nColunas == TAB_COL){
			nColunas = 0; 
			posY += INC_Y;
			posX = RIGHT;
		}
		nColunas++;
	}
}

void drawDialogos(bool * WINDOWS)
{
	bool exibir = true; 
	//intf("bloco id: %d\n", id_dialogo);
	Dialogo * dialogoImgs = getDialogos();
	int posX = WIDTH/2 - al_get_bitmap_width(dialogoImgs[0].img)/2;
	int posY = HEIGHT - al_get_bitmap_height(dialogoImgs[0].img); 
	if (id_dialogo != -1 && getInteragir() && getColidiuCollider())
	{	
		//Desenha a imagem
		al_draw_bitmap(dialogoImgs[id_dialogo].img, posX, posY, 0);
		WINDOWS[DIALOG] = true;
	}else{
		setInteragir (false);
	}

}

void drawInfos(bool * WINDOWS)
{

	//imagem das informacoes de cada elemento
	ALLEGRO_BITMAP ** imgInfo = getElementosINFO();
	//Posicao na tela
	int posX = WIDTH/2 - al_get_bitmap_width(imgInfo[0])/2;
	int posY = HEIGHT - al_get_bitmap_height(imgInfo[0]); 
	//numero atomico do elemento coletado
	int coletado = getItemColetadoNA();

	if (coletado > -1){
		WINDOWS[INFOS] = true; 
		al_draw_bitmap(imgInfo[coletado - 1], posX, posY, 0);
	}

}
void setDialogo (int dialogo){
	id_dialogo = dialogo; 
}
