#include "parser.h"


void parser(){
    programa();
    match(FDT);
}

void programa(){
    match(INICIO);
    match(ID);
    printf("Programa: %s\n",texto);
    sentencias();
    match(FDT); // Deberia ser match(FIN) pero no funciona 
    printf("Fin\n");
}

void sentencias(){
    StringList *lista_ids_declarados = crear_lista();
    while (token_actual != FDT){
        token tok = prox_token();
        match(tok);
        switch (tok) {
            case ID: // Caso asignacion
                match (ASIGNACION);
                printf("Sentencia Asignacion\n");
                lista_expresiones(lista_ids_declarados);
                match(PUNTOYCOMA);
            break;
            case LEER:
                printf("Sentencia Leer\n");
                match(PARENIZQUIERDO);
                lista_ids(lista_ids_declarados);
                match(PARENDERECHO);
                match(PUNTOYCOMA);
            break;
            case ESCRIBIR:
                printf("Sentencia Escribir\n");
                match(PARENIZQUIERDO);
                lista_expresiones(lista_ids_declarados);
                match(PARENDERECHO);
                match(PUNTOYCOMA);
            break;
            case DECLARACION:
                printf("Sentencia Declaracion\n");
                match(ID);
                agregar_string(lista_ids_declarados,texto);
                match(PUNTOYCOMA);
            break;
            default:
                if(tok != FDT) {
                    error_sintactico(tok);
                }
            break;
        }
    }
}

void lista_ids(StringList *lista_ids_declarados){
    match(ID);
    while (prox_token() == COMA){
        match(COMA);
        match(ID);
    }
}

void lista_expresiones(StringList *lista_ids_declarados) {
    expresion(lista_ids_declarados);
    while (prox_token() == COMA) {
        match(COMA);
        expresion(lista_ids_declarados);
    }
}

void expresion(StringList *lista_ids_declarados) {
    if (prox_token() == RESTA) {
        match(RESTA);
    }
    token tok = prox_token();
    switch (tok) {
        case CONSTANTE:
            match(CONSTANTE);
            break;
        case ID:
            match(ID);
            break;
        case PARENIZQUIERDO:
            match(PARENIZQUIERDO);
            expresion(lista_ids_declarados);
            match(PARENDERECHO);
            break;
        default:
            printf("Error sintáctico: Se esperaba una expresión\n");
            break;
    }

    while (prox_token() == SUMA || prox_token() == RESTA || prox_token() == MULTIPLICACION || prox_token() == DIVISION || prox_token() == MODULO) {
        token op = prox_token();
        match(op);
        expresion(lista_ids_declarados);
    }
}


// Función para crear una lista vacía
StringList *crear_lista() {
    StringList *lista = malloc(sizeof(StringList));
    if (!lista) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    lista->items = NULL;
    lista->size = 0;
    return lista;
}

// Función para agregar un string a la lista
void agregar_string(StringList *lista, const char *str) {
    lista->items = realloc(lista->items, (lista->size + 1) * sizeof(char *));
    if (!lista->items) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    lista->items[lista->size] = strdup(str);
    if (!lista->items[lista->size]) {
        perror("Error al copiar string");
        exit(EXIT_FAILURE);
    }
    lista->size++;
}

// Función para verificar si un string está en la lista
int contiene_string(StringList *lista,char *str) {
    for (size_t i = 0; i < lista->size; i++) {
        if (strcmp(lista->items[i], str) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para liberar la memoria de la lista
void liberar_lista(StringList *lista) {
    for (size_t i = 0; i < lista->size; i++) {
        free(lista->items[i]);
    }
    free(lista->items);
    free(lista);
}
