#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define IDENTIFICADOR 256
#define NUMERO 257
#define CUADRADO 258
#define CIRCULO 259
#define TRIANGULO 260
#define RECTANGULO 261
#define LIENZO 262
#define COLOR 263
#define PUNTOYCOMA   ';'
#define PARENTESIS_INI '('
#define PARENTESIS_FIN ')'
#define COMA ','

FILE *entrada, *salida, *tokens;
char lexema[80];
char colorFigura[30];
int tokenActual;

FILE *figuras; 


char tipoFiguraActual[30];
char nombreFigura[30];
int parametros[10];
int cantidad_parametros;


int palabras_reservadas() {
    if (strcmp(lexema, "cuadrado") == 0) return CUADRADO;
    if (strcmp(lexema, "circulo") == 0) return CIRCULO;
    if (strcmp(lexema, "triangulo") == 0) return TRIANGULO;
    if (strcmp(lexema, "rectangulo") == 0) return RECTANGULO;
    if (strcmp(lexema, "lienzo") == 0) return LIENZO;
    return -1;
}

int es_color_reservado() {
    const char* colores[] = {"rojo", "verde", "azul", "amarillo", "negro", "blanco", "gris", "marron", "celeste", "violeta", "naranja"};
    int n = sizeof(colores) / sizeof(colores[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(lexema, colores[i]) == 0) return COLOR;
    }
    return -1;
}

int obtener_token_lexico() {
    int c, i;
    do c = fgetc(entrada); while (isspace(c));
    if (c == EOF) return EOF;

    if (isalpha(c)) {
        i = 0;
        do {
            lexema[i++] = c;
            c = fgetc(entrada);
        } while (isalnum(c) || c == '_');
        lexema[i] = '\0';
        ungetc(c, entrada);
        int palabra = palabras_reservadas();
        if (palabra != -1) return palabra;

        int color_token = es_color_reservado();
        if (color_token != -1) return color_token;

        return IDENTIFICADOR;
    }

    if (isdigit(c)) {
        i = 0;
        do {
            lexema[i++] = c;
            c = fgetc(entrada);
        } while (isdigit(c));
        lexema[i] = '\0';
        ungetc(c, entrada);
        return NUMERO;
    }

    lexema[0] = c;
    lexema[1] = '\0';
    return c;
}

void guardar_token(int token) {
    switch (token) {
        case IDENTIFICADOR: fprintf(salida, "IDENTIFICADOR %s\n", lexema); break;
        case NUMERO: fprintf(salida, "NUMERO %s\n", lexema); break;
        case CUADRADO: fprintf(salida, "CUADRADO %s\n", lexema); break;
        case CIRCULO: fprintf(salida, "CIRCULO %s\n", lexema); break;
        case TRIANGULO: fprintf(salida, "TRIANGULO %s\n", lexema); break;
        case RECTANGULO: fprintf(salida, "RECTANGULO %s\n", lexema); break;
        case LIENZO: fprintf(salida, "LIENZO %s\n", lexema); break;
        case COLOR: fprintf(salida, "COLOR %s\n", lexema); break;
        case PARENTESIS_INI: fprintf(salida, "PARENTESIS_INI %s\n", lexema); break;
        case PARENTESIS_FIN: fprintf(salida, "PARENTESIS_FIN %s\n", lexema); break;
        case COMA: fprintf(salida, "COMA %s\n", lexema); break;
        case PUNTOYCOMA: fprintf(salida, "PUNTOYCOMA %s\n", lexema); break;
        default: fprintf(salida, "DESCONOCIDO %s\n", lexema); break;
    }
}

void fase_lexica() {
    entrada = fopen("entrada.txt", "r");
    if (!entrada) {
        printf("Error al abrir entrada.txt\n");
        exit(1);
    }

    salida = fopen("tokens.txt", "w");
    if (!salida) {
        printf("Error al crear tokens.txt\n");
        fclose(entrada);
        exit(1);
    }

    int token;
    while ((token = obtener_token_lexico()) != EOF) {
        guardar_token(token);
    }

    fclose(entrada);
    fclose(salida);
    printf("Fase lexica completada. Tokens guardados en tokens.txt\n");
}



int leer_token_parser() {
    char tipo[30];
    if (fscanf(tokens, "%s %s", tipo, lexema) == EOF) return EOF;

    if (strcmp(tipo, "IDENTIFICADOR") == 0) return IDENTIFICADOR;
    if (strcmp(tipo, "NUMERO") == 0) return NUMERO;
    if (strcmp(tipo, "CUADRADO") == 0) return CUADRADO;
    if (strcmp(tipo, "CIRCULO") == 0) return CIRCULO;
    if (strcmp(tipo, "TRIANGULO") == 0) return TRIANGULO;
    if (strcmp(tipo, "RECTANGULO") == 0) return RECTANGULO;
    if (strcmp(tipo, "LIENZO") == 0) return LIENZO;
    if (strcmp(tipo, "COLOR") == 0) return COLOR;
    if (strcmp(tipo, "PARENTESIS_INI") == 0) return PARENTESIS_INI;
    if (strcmp(tipo, "PARENTESIS_FIN") == 0) return PARENTESIS_FIN;
    if (strcmp(tipo, "COMA") == 0) return COMA;
    if (strcmp(tipo, "PUNTOYCOMA") == 0) return PUNTOYCOMA;

    return -1;
}

void error(const char* msg) {
    printf("Error de sintaxis: %s. Token actual: [%s]\n", msg, lexema);
    exit(1);
}

void avanzar() { tokenActual = leer_token_parser(); }

void match(int esperado) {
    if (tokenActual == esperado) avanzar();
    else error("Token inesperado");
}

void parseInstruccion();
void parseLienzoDecl();
void parseFiguraDecl();
void parseTipoFigura();
void parseParametros();

void parsePrograma() {
    avanzar();
    while (tokenActual != EOF) {
        parseInstruccion();
    }
}

void parseInstruccion() {
    if (tokenActual == LIENZO) {
        parseLienzoDecl();
    } else if (tokenActual == CUADRADO || tokenActual == CIRCULO ||
               tokenActual == TRIANGULO || tokenActual == RECTANGULO) {
        parseFiguraDecl();
    } else {
        error("Se esperaba 'lienzo' o tipo de figura");
    }
}

void parseLienzoDecl() {
    match(LIENZO);
    match(PARENTESIS_INI);
    
    char ancho[20], alto[20];
    if (tokenActual != NUMERO) error("Se esperaba un numero (ancho)");
    strcpy(ancho, lexema);
    match(NUMERO);
    
    match(COMA);
    
    if (tokenActual != NUMERO) error("Se esperaba un numero (alto)");
    strcpy(alto, lexema);
    match(NUMERO);

    fprintf(figuras, "LIENZO %s %s\n", ancho, alto);

    match(PARENTESIS_FIN);
    match(PUNTOYCOMA);
}

void parseFiguraDecl() {

    if (tokenActual == CUADRADO) strcpy(tipoFiguraActual, "CUADRADO");
    else if (tokenActual == CIRCULO) strcpy(tipoFiguraActual, "CIRCULO");
    else if (tokenActual == TRIANGULO) strcpy(tipoFiguraActual, "TRIANGULO");
    else if (tokenActual == RECTANGULO) strcpy(tipoFiguraActual, "RECTANGULO");
    else error("Tipo de figura desconocido");

    parseTipoFigura(); 


    strcpy(nombreFigura, lexema);
    match(IDENTIFICADOR);

    match(PARENTESIS_INI);


    cantidad_parametros = 0;

    if (tokenActual != NUMERO) {
        error("Se esperaba al menos un número como parámetro");
    }

  
    parametros[cantidad_parametros++] = atoi(lexema);
    match(NUMERO);

    while (tokenActual == COMA) {
    
        int token_siguiente;
        long pos = ftell(tokens);
        char lexema_copia[80];
        strcpy(lexema_copia, lexema);

        match(COMA);

        if (tokenActual == NUMERO) {
            parametros[cantidad_parametros++] = atoi(lexema);
            match(NUMERO);
        } else if (tokenActual == COLOR || tokenActual == IDENTIFICADOR) {
        
            fseek(tokens, pos, SEEK_SET);
            strcpy(lexema, lexema_copia);
            tokenActual = COMA; 
            break; 
        } else {
            error("Se esperaba un número o un color después de la coma");
        }
    }


    match(COMA);

    if (tokenActual != COLOR && tokenActual != IDENTIFICADOR) {
        error("Se esperaba un color como último parámetro");
    }

    strcpy(colorFigura, lexema);
    avanzar();

    match(PARENTESIS_FIN);
    match(PUNTOYCOMA);


    fprintf(figuras, "%s %s", tipoFiguraActual, nombreFigura);
    for (int i = 0; i < cantidad_parametros; i++) {
        fprintf(figuras, " %d", parametros[i]);
    }
    fprintf(figuras, " %s\n", colorFigura);
}

void parseTipoFigura() {
    if (tokenActual == CUADRADO || tokenActual == CIRCULO ||
        tokenActual == TRIANGULO || tokenActual == RECTANGULO) {
        avanzar();
    } else { 
        error("Tipo de figura no reconocido");
    }
}

void parseParametros() {
    match(NUMERO);
    while (tokenActual == COMA) {
        match(COMA);
        match(NUMERO);
    }
}

void fase_sintactica() {
    tokens = fopen("tokens.txt", "r");
    if (!tokens) {
        printf("Error al abrir tokens.txt\n");
        exit(1);
    }

    figuras = fopen("figuras.txt", "w");
    if (!figuras) {
        printf("Error al crear figuras.txt\n");
        fclose(tokens);
        exit(1);
    }

    parsePrograma();

    fclose(tokens);
    fclose(figuras);
    printf("Analisis sintactico completado correctamente. Figuras guardadas en figuras.txt\n");
}



int main(int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "--lex") == 0) {
        fase_lexica();
    } else if (argc == 2 && strcmp(argv[1], "--parse") == 0) {
        fase_sintactica();
    } else {
        fase_lexica();
        fase_sintactica();
    }

    return 0;
}
