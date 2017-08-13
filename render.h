#include <allegro5/allegro.h>    
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
//Cabeçalhos das funcoes implementadas no render.c
#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED
//Funcao responsavel por desenhar imagens na tela
extern void drawPlayer();
extern void drawInventory(int x, int y);
extern void drawAllItens();
extern void drawColliders();
extern void drawScene();
extern void drawDialogos(bool * WINDOWS_STATUS);
//Atribuicao da variavel dialogo que informa qual dialogo deve ser exibido
extern void setDialogo (int dialogo);
extern void drawInfos();
extern void drawAreaPuzzles();
extern void drawSolutionField (bool * WINDOWS, ALLEGRO_FONT * fonte, int n);
extern void drawLayer();
extern void drawPorta();
#endif //fim da definicao de RENDER_H_INCLUDED
