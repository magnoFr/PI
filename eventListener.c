#include <allegro5/allegro.h>                    
#include <stdio.h>
#include <allegro5/allegro_native_dialog.h>
#include "player.h"
// Teclas de direcao do player
enum KEYS {UP, DOWN, RIGHT, LEFT};
// Telas do jogo 
enum WINDOWS {GAME, INVENTORY, DIALOG, INFOS, CRAFT};
//Declaracao de variaveis
bool playerMove = false;
int typeMovement = -1;
//Variavel que informa quando devemos atualizar a logica do jogo ou redesenhar algo
bool updateRun = false;  
bool keys [] = {false, false, false, false};
//informa quando a tecla E é apertada
bool interagir = false; 
bool craftar = false; 
char resposta[17];
bool init = true; 
bool respostaEnviada = false;
void listningEvents (ALLEGRO_EVENT_QUEUE * queue, bool * WINDOWS){
	if (init){
		strcpy(resposta, "");
		init = false; 
	}
	//Variaveis booleanas de controle
	bool close_application = false;
	//Armazena os eventos retirados da fila de eventos
	ALLEGRO_EVENT event;
	//Aguardando ate que um evento seja disparado
	//Enquanto um evento nao for detectado o programa ficara ocioso (o while do game loop)
	al_wait_for_event(queue, &event);
	//printf("estado da janela: %d \n estado do inventario: %d \n ", WINDOWS[GAME], WINDOWS[INVENTORY]);
	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		
		switch (event.keyboard.keycode)
		{
			case ALLEGRO_KEY_ESCAPE:
				if (WINDOWS[INVENTORY]){
					WINDOWS[INVENTORY] = false; 
				}
				else if (WINDOWS[DIALOG]){
					printf("dialogo fechado!\n");
					WINDOWS[DIALOG] = false; 
					interagir = false;
				}else if (WINDOWS[INFOS]){
					WINDOWS[INFOS] = false;
					setItemColetadoNA(-1);
					printf("info fechada!\n");
				}else if (WINDOWS[CRAFT]){
					WINDOWS[CRAFT] = false; 
					respostaEnviada = false;
					craftar = false;
					printf("janela craft fechada\n");
				}
				else{
					WINDOWS[GAME] = false; 
				}
			break; 
			case ALLEGRO_KEY_ENTER: 
				if (WINDOWS[CRAFT]){
					//informa que o player acabou de escrever a resposta
					respostaEnviada = true;				
				}
			break;
			case ALLEGRO_KEY_UP:		
				keys[UP]= true;
			break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true; 
			break; 
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
			break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true; 
			break;
			case ALLEGRO_KEY_E:
				interagir = true; 
			break;
			case ALLEGRO_KEY_I:
				WINDOWS[INVENTORY] = true; 
			break; 
			case ALLEGRO_KEY_C:
				craftar = true;
			break;
		}
	}else if (event.type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (event.keyboard.keycode)
		{
			case ALLEGRO_KEY_UP:		
				keys[UP] = false;
			break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
			break; 
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
			break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
			break;
		}
	}else if (event.type == ALLEGRO_EVENT_KEY_CHAR && WINDOWS[CRAFT]){
		if (strlen(resposta) <= 16)
		{
		    char temp[] = {event.keyboard.unichar, '\0'};
		    if (event.keyboard.unichar == ' ')
		    {
		        strcat(resposta, temp);
		    }else if (event.keyboard.unichar == '+'){
		        strcat(resposta, temp);
		    }
		    else if (event.keyboard.unichar >= 'A' &&
		             event.keyboard.unichar <= 'Z')
		    {
		        strcat(resposta, temp);
		    }
		    else if (event.keyboard.unichar >= 'a' &&
		             event.keyboard.unichar <= 'z')
		    {
		        strcat(resposta, temp);
		    }
		}
		
		if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(resposta) != 0)
		{
		    resposta[strlen(resposta) - 1] = '\0';
		}
	}else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		if (!WINDOWS[INVENTORY]){
			WINDOWS[GAME] = false;			
		}
	}else if (event.type == ALLEGRO_EVENT_TIMER)
	{
		//Evento do tipo timer sera disparado 60 vezes em um segundo
		//A logica do jogo devera ser acionada nesse momento ja que queremos que 
		//a logica do jogo so seja atualizada somente 60 vezes em um segundo
		updateRun = true;
	}
}

bool *getActivekeys (){
	return keys;
}

void setUpdating (bool valor){
	updateRun = valor;
}

bool getUpdating (){
	return updateRun;
}

bool getInteragir(){
	return interagir;
}

void setInteragir(bool valor){
	interagir = valor;
}

bool getCraftar(){
	return craftar;
}

void setCraftar(bool valor){
	craftar = valor;
}

char * getEntradaTxt(){
	return resposta;
}

bool getResposta (){
	return respostaEnviada;
}

void setResposta (){
	strcpy(resposta, "");
}



