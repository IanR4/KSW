#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMLEX 124
#define DIC 124

extern FILE* entrada;
extern int seguirScanneando;

typedef enum {
	INICIO,
	FIN,
	LEER,
	ESCRIBIR,
	ID,
	CONSTANTE,
	PARENIZQUIERDO,
	PARENDERECHO,
	PUNTOYCOMA,
	COMA,
	ASIGNACION,
	SUMA,
	RESTA,
	MULTIPLICACION,
	DIVISION,
	COMENTARIO,
	FDT,
	ERRORASIG=17,
	ERRORCTE=19,
	DECLARACION=20,
	MODULO=21,
	ERRORCOMUN=23
} token;

struct lexema_tok{
    char lexema [TAMLEX];
    token tok;
};

extern struct lexema_tok tabla_simb[DIC];
extern char* texto;

token scanner();
int buscar(char *id, struct lexema_tok *ts, token *tok);
void inicializarDiccionario(struct lexema_tok *ts);
void agregarPalabrasReservadas(struct lexema_tok *ts);
void colocar(char *id, struct lexema_tok *ts, token tok);
void insertarEnBuffer(char* b, int* s, char c);
void iniciarBuffer(char* b);
int caracterIgnorado(char caracter);
int estadoCentinela(int estado);
int estadoAceptor(int estado);
int tipoDeCaracter(char c);
token tipoDeToken(int estado);
char* token_name(token tok);
extern token token_actual;
token prox_token(void);
void match(token tok_esperado);
void error_lexico(token tok);
void error_comun(token tok);
void error_asignacion(token tok);
void error_cte(token tok);
void error_sintactico(token tok);

#endif // SCANNER_H_INCLUDED