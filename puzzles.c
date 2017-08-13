#include "puzzles.h"
#include "eventListener.h"
#include "fileLoader.h"
#include "player.h"
#include "render.h"
#include "item.h"
#include <stdio.h>
#define QTD_PUZZLES 2

bool puzzles [] = {false, false};
enum WINDOWS {GAME, INVENTORY, DIALOG, INFOS, CRAFT};
int puzzleAtual = 0;
int puzzle1 = 0; 
int puzzle2 = 0;
int alertaMsgPuzzle = -1;
Porta portaSala;

void initPorta (){
	portaSala.x = 892; 
	portaSala.y = 310;
	portaSala.sprite = getPortaSprite();
}
void gerenciadorPuzzles (bool * WINDOWS, ALLEGRO_FONT * fonte)
{
	//Informa se o jogador apertou a tecla C
	bool craftar = getCraftar();
	//Diz se o jogador esta na area de crafting
	bool areaCrafting = podeCraftar();
	//Reposta final do jogador para o puzzle atual
	bool jaRespondeu = getResposta ();
	//repostas dos puzzles
	Craft * respostas = getRespostasPuzzles();

	//Variaveis auxiliares
	char elemento1[17]; 
	char elemento2[17]; 
	char temp [2];
	strcpy(temp, "");
	strcpy(elemento1, "");
	strcpy(elemento2, "");
	bool seg = false;
	//Apertou C e esta na area de crafting
	if (craftar && areaCrafting && puzzleAtual < QTD_PUZZLES)
	{
		WINDOWS[CRAFT] = true;
		char * respostaJogador = getEntradaTxt();
		//Ja apertou enter e respondeu
		if (jaRespondeu)
		{
			//Separa os elementos que o jogador deu como resposta
			for (int i = 0; i < 17; i++)
			{
				temp[0] = respostaJogador[i];
				if (strcmp (temp, "") == 0){
					break;
				}else{
					if (strcmp(temp, "+") == 0){
						seg = true;
						continue;
					}

					if (!seg){
						//printf("elemento 1: %s \n", temp);
						strcat(elemento1, temp);
					}else{
						//printf("elemento 2: %s \n", temp);
						strcat(elemento2, temp);
					}
				}
			}	
			setResposta (false);
			setCraftar(false);
			//verifica se os elementos ja foram desbloqueados
			ListaItens * elementos = getItensColetados();
			if (!estaVazia (elementos))
			{
				printf("procurando os elementos... \n");
				bool temPrimeiro = buscaItem(elementos, elemento1);
				bool temSegundo = buscaItem(elementos, elemento2);
				if (temPrimeiro && temSegundo)
				{
					printf("tem os elementos necessarios!\n");
					printf("puzzle atual: %d \n", puzzleAtual);
					printf("reposta esperada: %s\n", respostas[puzzleAtual].solucao);
					if (strcmp (strcat(elemento1, elemento2), respostas[puzzleAtual].solucao) == 0){
						alertaMsgPuzzle = 0;
						printf("resposta correta!\n");
						puzzles[puzzleAtual] = true;
						puzzleAtual++; //proximo puzzle
					}else{
						alertaMsgPuzzle = 1;
						printf("resposta errada!\n");
						//fazer som de errado
					}
				}else{
					alertaMsgPuzzle = 2;
					printf("voce nao tem todos os elementos necessarios!\n");
					//fazer som de errado
				}
			}else{
				alertaMsgPuzzle = 3;
				printf("Voce ainda nao coletou nenhum elemento!\n");
			}
		}
	}else if (craftar && !areaCrafting){
		setCraftar(false);
	}
	//Desenha a janela de reposta
	if (WINDOWS[CRAFT]){
		drawSolutionField(WINDOWS, fonte, puzzleAtual);
	}
}

bool * getPuzzles (){
	return puzzles;
}

Porta * getPorta()
{
	return &portaSala;
}

int getMsgAlertaPuzzle(){
	return alertaMsgPuzzle;
}
