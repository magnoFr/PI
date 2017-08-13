//Cabeçalhos das funcoes implementadas no eventListener.c
#ifndef EVENTLISTENER_H_INCLUDED
#define EVENTLISTENER_H_INCLUDED

//Funcao responsavel por escutar eventos 
//retorna true quando o evento for do tipo display close
extern bool listningEvents(ALLEGRO_EVENT_QUEUE *event_queue, bool *WINDOWS_STATUS);
extern bool *getActivekeys ();
//Altera o valor da variavel responsavel por informar precisa ser atualizado (true)
//Ja esta atualizado (false)
extern void setUpdating (bool valor);
extern bool getUpdating ();
//Se o jogador aperta E interagir eh true, caso contrario, false.
extern bool getInteragir();
extern void setInteragir(bool valor);

extern bool getCraftar();
extern void setCraftar(bool valor);
extern char * getEntradaTxt();
extern bool getResposta ();
extern void setResposta ();
#endif //fim da definicao de FILELOADER_H
