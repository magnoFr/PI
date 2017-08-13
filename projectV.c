#include <allegro5/allegro.h>                     
#include <allegro5/allegro_image.h>              
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 
#include <stdio.h>
#include <stdbool.h>
#include "fileLoader.h"
#include "render.h"
#include "eventListener.h"
#include "player.h"
#include "puzzles.h"
//Declaracao de variaveis constantes
#define WIDTH 1280
#define HEIGHT 720
//FPS
const int FPS =  60;

//Declaracao de variaveis globais
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_SAMPLE *relogio;
ALLEGRO_SAMPLE *music;
// Telas do jogo 
enum WINDOWS {GAME, INVENTORY, DIALOG, INFOS, CRAFT};
//Informa o estado da janela (true = aberta, false = fechada)
bool WINDOWS [] = {true, false, false, false, false};
ALLEGRO_FONT *fonte = NULL;
char str [17];


void finalize ();
bool initialize ();

int main (void)
{	
	//Variaveis do jogo
	bool close_application = false;
	bool open_inventory = false; 
	int contador = 0;  

	//Variaveis de entrada do usuario
	strcpy(str, "");
	bool textoExibido = false;

	//Inicializando as tarefas relativas ao programa
	if (!initialize ()){
		return -1; 
	}
	//Colocando as musicas de fundo para tocar
	al_play_sample(music, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
	al_play_sample(relogio, 3.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
	al_start_timer(timer);
	WINDOWS[GAME] = true; //janela do jogo ativa
	/* ------------- GAME LOOP --------------- */
	while (WINDOWS[GAME])
	{
		//Informa que a tela do jogo esta aberta
		WINDOWS[GAME] = true;
		//Atualizar a logica e a tela do jogo respeitando o FPS 
		if (getUpdating ())
		{
			drawScene ();
			//verifica se o puzzle 2 foi solucionado se nao foi desenha a porta
			if (!getPuzzles()[1]){
				drawPorta();
			}
			drawColliders();
			drawAllItens();
			gerenciadorPuzzles(WINDOWS, fonte);
			//Desenha imagem do jogador na tela
			drawPlayer();
			bool * p = getPuzzles();
			if (!p[0]) 
				drawLayer();
			drawDialogos(WINDOWS);
			drawInfos(WINDOWS);
			drawAreaPuzzles();
			if (!WINDOWS[INVENTORY] && !WINDOWS[DIALOG] && !WINDOWS[INFOS] && !WINDOWS[CRAFT])
			{
				//Atualiza o movimento do player
				playerMovement();
				//printf("move player\n");
			}
			else if (WINDOWS[INVENTORY] && !WINDOWS[DIALOG] && !WINDOWS[INFOS] && !WINDOWS[CRAFT])
			{
				//Desenha o inventario
				ALLEGRO_BITMAP * inventory = getInventoryImage();
				int x = WIDTH/2 - al_get_bitmap_width (inventory)/2;
				int y = HEIGHT/2 - al_get_bitmap_height (inventory)/2;
				drawInventory(x, y);			
			}
			//Atualizando a tela
			al_flip_display();
			//Limpando a tela 
			al_clear_to_color(al_map_rgb(190, 190, 190));
			//Informa que a atualizacao ja foi realizada
			setUpdating (false);
		}
		listningEvents(event_queue, WINDOWS);			
	}

	finalize();

	return 0;
}


bool initialize ()
{
	//Inicializando a biblioteca Allegro
	if (!al_init())
	{
	    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
	    return false;
	}

	//Criando e setando as dimensoes da janela
	window = al_create_display(WIDTH, HEIGHT);

	if (!window)
	{
	    fprintf(stderr, "Falha ao criar janela.\n");
	    return false;
	}
	al_set_window_title(window, "Project Vancouver!");

	/* ---- INICIALIZAÇÃO DE ADDONS E INSTALAÇÕES ----*/
	
	if (!al_install_keyboard())
	{
	    fprintf(stderr, "Falha ao instalar o teclado.\n");
	    return false;
	}
	al_init_font_addon();
	 
	if (!al_init_ttf_addon())
	{
	    fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
	    return false;
	}

	fonte = al_load_font("Djb.ttf", 28, 0);
	if (!fonte)
	{
	    fprintf(stderr, "Falha ao carregar \"Djb.ttf\".\n");
	    return false;
	}
	if (!al_init_acodec_addon()){
	        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
	        return false;
	}
	if (!al_install_audio()){
		fprintf(stderr, "Falha ao instalar o audio.\n");
		return false;	
	}
	music = al_load_sample("Peter.ogg");
	if (!music){
		fprintf(stderr, "Falha ao carregar sample da musica de fundo.\n");
		return false;	
	}

	relogio = al_load_sample("Clock.ogg");
	if (!relogio){
		fprintf(stderr, "Falha ao carregar sample do relogio.\n");
		return false;	
	}

	//Reservando as músicas
	al_reserve_samples(2);
	timer = al_create_timer(1.0 / FPS); //Em um segundo teremos 60 quadros

	/* ---- CRIAÇÃO DE FILAS E DEMAIS DISPOSITIVOS ---*/
	event_queue = al_create_event_queue();

	if (!event_queue)
	{
	    fprintf(stderr, "Falha ao criar fila de eventos.\n");
	    al_destroy_display(window);
	    return false;
	}

	/* ------------- REGISTRO DE SOURCE ------------- */

	//Registrando a janela na fila de eventos
	al_register_event_source(event_queue, al_get_display_event_source(window)); 
	//Registrando o teclado na fila de eventos
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	/* ---------- CARREGAMENTO DE ARQUIVOS -----------*/

 	//Carregando imagens
	loadingImages();
	//Carregando Colisores do cenario
 	loadingColliders();
 	//Carregando os itens do cenario
 	loadingItens();
 	//Carregando o arquivo de elementos da tabela 
 	loadingElements();

 	loadingRespostasPuzzles();
	//Criação e Inicializando objeto do tipo player
	initPlayer(WIDTH, HEIGHT);
	initPorta();
 
	return true;
}

void finalize ()
{

	desalocarItensColetados();
	desalocarMemoria();
	al_destroy_sample(music);
	al_destroy_sample(relogio);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(window);
}


