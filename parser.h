#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "scanner.h"

void parser();
void programa();
void lista_sentencias();
void lista_ids();

typedef struct {
    char **items;
    size_t size;
} StringList;

StringList *crear_lista();
void agregar_string(StringList *lista, const char *str);
int contiene_string(StringList *lista,char *str);
void liberar_lista(StringList *lista);
void lista_expresiones(StringList *lista);
void expresion(StringList *lista);
void sentencia();
#endif // PARSER_H_INCLUDED