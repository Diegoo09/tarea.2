#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"

typedef struct Cancion{

    char * titulo;
    char * artista;
    char * duracion;
    char * album;
}Cancion;

Cancion * crearCancion(char * linea);

void leer_datos(FILE * fp, Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album);

void exportar_archivo(Map * m);

void agregar_album (Map * mapa_canciones, Map * mapa_artistas, Map * Mapa_album);

Cancion * buscar_cancion(char * nombre_cancion, Map * mapa_canciones);
void mostrar_Cancion (Cancion * c);

void mostrar_canciones_artista (char * nombre, Map * mapa_artistas);

void agregar_cancion (Cancion *c, Map* mapa_canciones, Map * mapa_artistas, Map * Mapa_album);

void eliminar_canciones_artista(char * artista, Map * mapa_canciones, Map * mapa_artistas, Map * Mapa_album);

void clrscr(){  system("cls"); }
const char *get_csv_field (char * tmp, int i);
char * _strdup (const char *s);
long long stringHash(const void * key);
int stringEqual(const void * key1, const void * key2);

int main(){

    char nombre[99] ={};
    char titulo[99] ={};
    char artista[99] ={};
    char duracion[99] ={};
    char album[99] ={};

    Cancion *c = (Cancion*) malloc(sizeof(Cancion)*2);

    FILE * fp = fopen("canciones.csv", "r");

    Map * mapa_canciones = createMap(stringHash, stringEqual);
    Map * mapa_artistas = createMap(stringHash, stringEqual);
    Map * Mapa_album = createMap(stringHash, stringEqual);

    Map * mapa = createMap(stringHash, stringEqual);

    leer_datos( fp,  mapa_canciones, mapa_artistas, Mapa_album);

    int opcion;
    printf ("\n    BIENVENIDO\n");

    while ( 1 ){

        printf ("\n(1) Para exportar archivo. \n");
        printf ("(2) Para agregar album. \n");
        printf ("(3) Para agregar una cancion. \n");
        printf ("(4) Para eliminar canciones de un artista. \n");
        printf ("(5) Para buscar cancion. \n");
        printf ("(6) Para buscar canciones de un artista. \n");
        printf ("(7) Para buscar album. \n");
        printf ("(8) Para finalizar el programa. \n");
        printf ("Seleccione una opcion : ");
        scanf ("%d", &opcion);

        switch (opcion){

        case 1:
                clrscr();

                exportar_archivo(mapa_canciones);
                printf ("\nARCHIVO EXPORTADO \n");

                break;


        case 2:
                clrscr();

                list * L = list_create_empty();
                Cancion * song = (Cancion*)malloc(sizeof(Cancion));

                printf ("\nIngresar nombre de album que desea crear : ");
                fflush( stdin );
                scanf ("%[^\n]s", album);

                if (searchMap( Mapa_album, album ) != NULL ){
                    printf ("ERROR, EL ALBUM '%s' YA EXISTE\n", album);
                }else{

                    int respuesta = 1;
                    while (respuesta == 1){

                        printf ("\nIngresar Titulo : ");
                        fflush( stdin );
                        scanf ("%[^\n]s", titulo);

                        if (searchMap( mapa_canciones, titulo ) != NULL ){
                            printf ("ERROR, LA CANCION '%s' YA EXISTE\n", titulo);
                            break;
                        }

                        printf ("Ingresar Artista : ");
                        fflush( stdin );
                        scanf ("%[^\n]s", artista);

                        printf ("Ingresar Duracion : ");
                        fflush( stdin );
                        scanf ("%[^\n]s", duracion);

                        song->titulo = titulo;
                        song->artista = artista;
                        song->duracion = duracion;
                        song->album = album;

                        insertMap( mapa_canciones, song->titulo, song);  //insertar en mapa canciones

                        list_push_back(L,song);
                        printf("%s, %s, %s, %s.\n", song->titulo, song->artista, song->duracion, song->album);

                        printf ("\n(1) Para agregar otra cancion \n");
                        printf ("(2) Para volver al Menu \n");
                        printf ("Respuesta : ");
                        scanf ("%d", &respuesta);
                    }

                    insertMap(mapa_artistas, song->artista, L);
                    insertMap(Mapa_album, song->album, L);  //insertar en mapa album.
                }

                printf ("Album agregado \n");

                break;


        case 3:
                clrscr();

                printf ("\nIngresar Titulo : ");
                fflush( stdin );
                scanf ("%[^\n]s", titulo);

                if (searchMap( mapa_canciones, c->titulo ) != NULL ){
                    printf ("ERROR, LA CANCION '%s' YA EXISTE\n", c->titulo);
                    break;
                }

                printf ("Ingresar Artista : ");
                fflush( stdin );
                scanf ("%[^\n]s", artista);

                printf ("Ingresar Duracion : ");
                fflush( stdin );
                scanf ("%[^\n]s", duracion);

                printf ("Ingresar Album : ");
                fflush( stdin );
                scanf ("%[^\n]s", album);
                printf("\n");

                c->titulo = titulo;
                c->artista = artista;
                c->duracion = duracion;
                c->album = album;

                agregar_cancion (c, mapa_canciones, mapa_artistas, Mapa_album);

                printf ("Cancion Agregada");

                break;


        case 4:
                clrscr();

                printf ("\nIngresar nombre artista a eliminar : ");
                fflush( stdin );
                scanf ("%[^\n]s", artista);

                if(searchMap( mapa_artistas, artista ) == NULL ){
                    printf ("ERROR, El ARTISTA '%s' NO EXISTE\n", artista);

                }else{
                     eliminar_canciones_artista( artista, mapa_canciones , mapa_artistas, Mapa_album );
                     printf ("Artista Eliminado\n");
                }

                break;


        case 5:
                clrscr();

                printf ("\nIngresar nombre de cancion : ");
                fflush( stdin );
                scanf ("%[^\n]s", titulo);

                c = buscar_cancion(titulo, mapa_canciones);
                mostrar_Cancion (c);

                break;


        case 6:
                clrscr();

                printf ("\Ingresar nombre del artista deseado : ");
                fflush( stdin );
                scanf ("%[^\n]s", nombre);

                mostrar_canciones_artista( nombre, mapa_artistas );

                break;


        case 7:
                clrscr();

                printf ("\nIngresar nombre del album deseado : ");
                fflush( stdin );
                scanf ("%[^\n]s", album);

                mostrar_canciones_album (album, Mapa_album);

                break;


        case 8:
                clrscr();
                printf ("\nFINALIZANDO\n");
                return (0);

                break;


        default:  //Cualquier otra opcion ingresada.
                clrscr();
                printf ("ERROR AL INGRESAR OPCION. \n");
                break;
        }
    }
    return 0;
}

Cancion * crearCancion (char * linea){

    Cancion * c = (Cancion *)malloc (sizeof(Cancion));

    c->titulo = (char *) get_csv_field(linea, 1);
    c->artista = (char *) get_csv_field(linea,2);
    c->duracion = (char *)get_csv_field(linea, 3);
    c->album = (char *)get_csv_field(linea,4);

    return c;
}

void leer_datos(FILE * fp, Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album){

    char * linea =(char*)malloc(1024*sizeof(char));

    Cancion * c = (Cancion *) malloc (sizeof(Cancion));

    while( (fgets( linea, 251, fp)) != NULL){  //1

        linea = strtok(linea,"\r");  //2
        linea = _strdup(linea);  //3

        char * nLinea =(char*)malloc(1024*sizeof(char));
        strcpy(nLinea,linea);

        c = crearCancion(nLinea);  //4

        agregar_cancion(c, mapa_canciones, mapa_artistas, Mapa_album);
    }
}

void agregar_cancion (Cancion * c, Map * mapa_canciones, Map * mapa_artistas, Map * Mapa_album){

    if( (searchMap( mapa_canciones, c->titulo )) == NULL ){  //Canciones.
        insertMap( mapa_canciones, c->titulo, c);
    }

    if( (searchMap( mapa_artistas, c->artista )) == NULL ){  //Artistas.
        list * L = list_create_empty();
        list_push_back (L,c);
        insertMap(mapa_artistas, c->artista, L);

            Map * mapa_canciones_aux = createMap(stringHash, stringEqual);
            insertMap( mapa_canciones_aux, c->titulo, c);
            insertMap( mapa_artistas, c->artista, mapa_canciones_aux );
    }else{
        list * L = searchMap(mapa_artistas, c->artista);
        list_push_back (L,c);

            insertMap( mapa_artistas, c->artista, c );
    }

    if( (searchMap( Mapa_album, c->album )) == NULL ){  //Albumes.
        list * L = list_create_empty();
        list_push_back (L,c);
        insertMap(Mapa_album, c->album, L);

            Map * mapa_canciones_aux = createMap(stringHash, stringEqual);
            insertMap( mapa_canciones_aux, c->titulo, c);
            insertMap( Mapa_album, c->album, mapa_canciones_aux );
    }else{
        list * L = searchMap(Mapa_album, c->album);
        list_push_back (L,c);

            insertMap( Mapa_album, c->titulo, c  );
    }
}

void exportar_archivo (Map * mapa_canciones){

    char nuevo[99]={};
    printf ("Ingresar nombre del archivo que desea crear \n");
    fflush (stdin);
    scanf ("%[^\n]s", nuevo);

    strcat(nuevo,".csv");
    getchar();

    FILE * fpp;
    fpp = fopen (nuevo,"w");

    Cancion * cn;
    cn = firstMap (mapa_canciones);

    if (fpp == NULL){
        printf ("ERROR ABRIENDO EL ARCHIVO");
    }else{
        while (cn != NULL){

            fflush(stdin);
            fprintf(fpp, "%s,", cn->titulo);
            fprintf(fpp, "%s,", cn->artista);
            fprintf(fpp, "%s,", cn->duracion);
            fprintf(fpp, "%s ", cn->album);
            fprintf(fpp, "\n");

            cn = nextMap(mapa_canciones);
        }
    }
}

void mostrar_canciones_artista (char * nombre , Map * mapa_artistas){

    list * Aux;
    Cancion * Aux2;

    if( emptyMap(mapa_artistas) == 1 ){
        printf("\nEl mapa no contiene elementos \n\n");

    }else{

        if( searchMap(mapa_artistas, nombre) == NULL){
            printf("\nEl artista '%s' no se encuentra registrado \n\n", nombre);

       }else{
            printf("\nEl artista '%s' cuenta con las siguientes canciones : \n",nombre);

            Aux = searchMap(mapa_artistas, nombre);
            Aux2 = list_first(Aux);  //Igualamos la variable Aux2 al valor del primer nodo de la lista.

            while(Aux2 != NULL){
                printf("   %s, %s, %s, %s \n", Aux2->titulo, Aux2->artista, Aux2->duracion, Aux2->album);
                Aux2 = list_next(Aux);
            }
       }
    }
}

void mostrar_canciones_album (char * nombre, Map * Mapa_album){

    list * Aux;
    Cancion * Aux2;

    if( emptyMap(Mapa_album) == 1 ){
        printf("\nEl mapa no contiene elementos \n\n");

    }else{

        if( searchMap(Mapa_album, nombre) == NULL){
            printf("\nEl album '%s' no se encuentra registrado \n\n", nombre);

       }else{
            printf("\nEl album '%s' cuenta con las siguientes canciones : \n",nombre);

            Aux = searchMap(Mapa_album, nombre);
            Aux2 = list_first(Aux);  //Igualamos la variable Aux2 al valor del primer nodo de la lista.

            while(Aux2 != NULL){
                printf("   %s, %s, %s, %s \n", Aux2->titulo, Aux2->artista, Aux2->duracion, Aux2->album);
                Aux2 = list_next(Aux);
            }
       }
    }

}

void eliminar_canciones_artista(char * artista,Map * mapa_canciones, Map * mapa_artistas, Map * Mapa_album){

    Cancion *c = (Cancion *)malloc(sizeof(Cancion));
    c=firstMap( mapa_canciones);

    while (c != NULL){
        if ( strcmp(c->artista, artista) ==0  ){

            eraseKeyMap(mapa_canciones, c->titulo);
            eraseKeyMap(Mapa_album, c->album);
        }
        c = nextMap(mapa_canciones);
    }

    c=firstMap( mapa_canciones);
    eraseKeyMap(mapa_artistas, artista);
}

Cancion * buscar_cancion (char * nombre_cancion, Map * mapa_canciones){

    Cancion *c = (Cancion *) malloc (sizeof(Cancion));

    if (  (c = ( Cancion* )searchMap(mapa_canciones, nombre_cancion)) != NULL  ){
        return c;
    }else{
        return NULL;
    }
}

void mostrar_Cancion( Cancion * c){

    printf ("Nombre cancion : %s \n", c->titulo);
    printf ("Nombre artista : %s \n", c->artista);
    printf ("Duracion : %s \n", c->duracion);
    printf ("Nombre album : %s \n\n", c->album);
}

/** Funciones Apoyo**/
long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}
int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

char * _strdup (const char *s) {
	size_t len = strlen (s) + 1;
    void *new = malloc (len);

	if (new == NULL)
	    return NULL;

	return (char *) memcpy (new, s, len);
}
const char *get_csv_field (char * tmp, int i) {

    //se crea una copia del string tmp
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) {
            return tok;
        }
    }
    return NULL;
}
