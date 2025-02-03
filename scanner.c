#include "scanner.h"

static char buffer [TAMLEX];
struct lexema_tok tabla_simb[DIC];
char* texto;
FILE* entrada;


static int tabla[25][17]={  
    //      Let Dig  +   -   (   )   ,   ;   :   =  fdt esp otr  *   /  \n   %  
    /*0- */{ 1,  3,  5,  6,  7,  8,  9, 10 , 11, 17, 13, 0, 16, 14, 15,  0, 24}, // Inicial  
    /*1  */{ 1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2}, // Identificador  
    /*2+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Identificador reconocido  
    /*3  */{18,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4}, // Constante  
    /*4+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Constante reconocida  
    /*5+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // "+" reconocido  
    /*6+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // "-" reconocido  
    /*7+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // "(" reconocido  
    /*8+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // ")" reconocido  
    /*9+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // "," reconocido  
    /*10+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // ";" reconocido  
    /*11 */{17, 17, 17, 17, 17, 17, 17, 17, 17, 12, 17, 17, 17, 17, 17, 17, 17}, // Asignación  
    /*12+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Asignación reconocida  
    /*13+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // FDT reconocido  
    /*14+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // "*" reconocido  
    /*15 */{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 20, 20}, // "/" o Comentario  
    /*16+*/{16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 23, 16, 16, 16, 23, 16}, // Error común  
    /*17+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Error de asignación  
    /*18 */{18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19}, // Constante errónea  
    /*19+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Error de constante  
    /*20+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // "/" reconocido  
    /*21 */{21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 21, 21, 21, 21, 22, 21}, // Comentario  
    /*22 */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Comentario completo  
    /*23+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Error común  
    /*24+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}  // "%" reconocido  
};

token tipoDeToken(int estado){
    switch (estado){
        case 2: return ID;
        case 4: return CONSTANTE;
        case 5: return SUMA;
        case 6: return RESTA;
        case 7: return PARENIZQUIERDO;
        case 8: return PARENDERECHO;
        case 9: return COMA;
        case 10: return PUNTOYCOMA;
        case 12: return ASIGNACION;
        case 13: return FDT;
        case 14: return MULTIPLICACION;
        case 20: return DIVISION;
        case 23: return ERRORCOMUN;
        case 17: return ERRORASIG;
        case 19: return ERRORCTE;
        case 22: return COMENTARIO;
		case 24 :return MODULO;
        default: return (token)-1;
    }
}

char* tok_names[] = { "INICIO", "FIN", "LEER", "ESCRIBIR", "ID", "CONSTANTE",
		"PARENIZQUIERDO", "PARENDERECHO", "PUNTOYCOMA", "COMA", "ASIGNACION", 
		"SUMA", "RESTA", "MULTIPLICACION", "DIVISION", "COMENTARIO", "FDT",
		"ERRORCOMUN", "ERRORASIG", "ERRORCTE","DECLARACION","MODULO" };

enum {
	letra,
	digito,
	signoMas,
	signoMenos,
	parenIzq,
	parenDer,
	coma,
	puntoYComa,
	dosPuntos,
	igual,
	fdt,
	espacio,
	otro,
	signoPor,
	signoDiv,
	saltoLinea,
	modulo = 16,
};

int tipoDeCaracter(char c){
    if (( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ))   {return letra;}
    else if ( c >= '0' && c <= '9' )                            {return digito;}
    else if (c == '+')                                          {return signoMas;}
    else if (c == '-')                                          {return signoMenos;}
    else if (c == '(')                                          {return parenIzq;}
    else if (c == ')')                                          {return parenDer;}
    else if (c == ',')                                          {return coma;}
    else if (c == ';')                                          {return puntoYComa;}
    else if (c == ':')                                          {return dosPuntos;}
    else if (c == '=')                                          {return igual;}
    else if (c == EOF)                                          {return fdt;}
    else if (c == ' ' || c == '\t')                       		{return espacio;}
    else if (c == '*')                                          {return signoPor;}
    else if (c == '/')                                          {return signoDiv;}
    else if (c =='\n')                                          {return saltoLinea;}
	else if (c =='%') 											{return modulo;}
    else                                                        {return otro;}
}

int estadoAceptor(int estado) {
	return (estado == 2 || (estado >= 4 && estado <= 10)
			|| (estado >= 12 && estado <= 14) || (estado == 17) || (estado == 23)
			|| (estado == 19) || (estado == 20) || estado == 22 || estado == 24);
}

int estadoCentinela(int estado) {
	return (estado == 2 || estado == 4 || estado == 19 || estado == 20
			|| estado == 22);
}

int caracterIgnorado(char caracter) {
	return (caracter == ' ' || caracter == '\n' || caracter == '\t');
}

void iniciarBuffer(char* b) {
	b[0] = '\0';
}

void insertarEnBuffer(char* b, int* s, char c) {
	b[(*s)++] = c;
	b[*s] = '\0';
}

void colocar(char *id, struct lexema_tok *ts, token tok) {
	int i;
	for (i = 0; i < DIC; i++) {
		if (ts[i].lexema[0] == '\0') {
			strcpy(ts[i].lexema, id);
			ts[i].tok = tok;
			break;
		}
	}
}

void agregarPalabrasReservadas(struct lexema_tok *ts) {
	colocar("programa", ts, INICIO);
	colocar("fin", ts, FIN);
	colocar("leer", ts, LEER);
	colocar("escribir", ts, ESCRIBIR);
    colocar("entero", ts, DECLARACION);
}

void inicializarDiccionario(struct lexema_tok *ts) {
	int i;
	for (i = 0; i < DIC; i++) {
		ts[i].lexema[0] = '\0';
		ts[i].tok = (token) 0;
	}
}


int buscar(char *id, struct lexema_tok *ts, token *tok) {
	int i;
	for (i = 0; i < DIC; i++) {
		if (strcmp(ts[i].lexema, id) == 0) {
			*tok = ts[i].tok;
			return 1;
		} else if (ts[i].lexema[0] == '\0')
			break;
	}
	return 0;
}

char* token_name(token tok) {
	return tok_names[tok];
}

token scanner() {
	char c;
	int es, estado = 0, buffer_size = 0;
	token tok;
	iniciarBuffer(buffer);
	texto = buffer;
	while (!estadoAceptor(estado)) {
		c = (char) fgetc(entrada);
        if (c == EOF) {
            return FDT; 
        }
		es = tipoDeCaracter(c);
		estado = tabla[estado][es];
		if (estadoCentinela(estado))
			ungetc(c, entrada);
		else if (!caracterIgnorado(c))
			insertarEnBuffer(buffer, &buffer_size, c);
	}
	tok = tipoDeToken(estado);

	if (tok == ID){
		buscar(buffer, tabla_simb, &tok);
	}
    
	return tok;
}

int seguirScanneando = 1;
token token_actual;
token prox_token(void)
{
	if (seguirScanneando) {
		token_actual = scanner();
		while (token_actual == 17 || token_actual == 23 || token_actual == 19) {
			error_lexico(token_actual);
			token_actual = scanner();
		}
		seguirScanneando = 0;
	}
	return token_actual;
}

void match(token tok_esperado)
{
	token token = prox_token();
	if (token != tok_esperado){
		error_sintactico(token);
	}
	seguirScanneando = 1;
}

void error_lexico(token tok){
	switch ((int) tok) {
	case ERRORCOMUN:
		error_comun(tok);
		break;
	case ERRORASIG:
		error_asignacion(tok);
		break;
	case ERRORCTE:
		error_cte(tok);
		break;
	default:
		break;
	}
}
void error_comun(token tok) {
	printf("Error comun. %s\n",buffer);
}

void error_asignacion(token tok) {
	printf("Error de asignacion %s\n",buffer);
}

void error_cte(token tok) {
	printf("Error lexico: constante mal formada %s\n",buffer);
}

void error_sintactico(token tok){
	printf("Error Sintactico, no se esperaba el token %s (%s).\n",token_name(tok),buffer);
}