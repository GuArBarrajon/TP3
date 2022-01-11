#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct pelicula
{
    char nombre[50];
    char genero[20];
    int duracion;
    char descripcion[100];
    int puntaje;
    int estado;
};

void getString(char mensaje[],char input[]);
int getInt(char* mensaje);
char getChar(char* mensaje);
void cargarPelicula(struct pelicula *p);
int guardarPelicula(struct pelicula *p);
void imprimir();
struct pelicula* eliminarPelicula(char* nomFilm);
struct pelicula* modificarPelicula(char* nomFilm);
void cierre();
void crearEncabezado(char* destino);
struct pelicula* crearCuerpo(char* nomfilm, char* destino);
void crearPie(char* destino);


void main()
{ 
    struct pelicula p;
    int opcion;
    char continuar;
    char nombrep[50];
    char arch[50];

    for(;opcion!=5;)
    {
        printf("Marque:\n1. Ingresar pelicula\n2. Eliminar pelicula\n3. Modificar pelicula\n4. Generar pagina web\n5. Salir del programa\n");
        scanf("%d", &opcion);
        fflush(stdin);
        
        switch (opcion)
        {
        case 1:
            continuar='s';
            while(continuar=='s')
            {
                cargarPelicula(&p);
                guardarPelicula(&p);
                imprimir();
                continuar = getChar("Ingresar uno nuevo? s/n:");
                fflush(stdin);
            }
            
            break;
    
        case 2:
            imprimir();
            getString("Ingrese el nombre del film: ", nombrep);
            eliminarPelicula(nombrep);
            imprimir();
            break;

        case 3:
            imprimir();
            getString("Ingrese el nombre del film a modificar: ", nombrep);
            modificarPelicula(nombrep);
            imprimir();
            break;

        case 4:
            getString("Ingrese el nombre del film: ", nombrep);
            getString("Ingrese el nombre del archivo a crear: ", arch);
            crearEncabezado(arch);
            crearCuerpo(nombrep, arch);
            crearPie(arch);
            printf("Fue creado el archivo %s\n", arch);
            break;
        
        case 5:
            cierre();
            break;
        
        default:
            printf("La opcion ingresada no es correcta");
            break;

        }

        fflush(stdin);
    } 
}

void getString(char mensaje[], char input[])
{
    printf("%s", mensaje);
    gets(input);
}

int getInt(char* mensaje)
{
    int auxiliar;
    printf("%s", mensaje);
    scanf("%d", &auxiliar);
    return auxiliar;
}

char getChar(char* mensaje)
{
    char auxiliar;
    printf("%s", mensaje);
    fflush(stdin);
    scanf("%c", &auxiliar);
    return auxiliar;
}

void cargarPelicula(struct pelicula *p)
{
    getString("Ingrese el nombre del film: ", p->nombre);
    fflush(stdin);
    getString("Ingrese el genero: ", p->genero);
    p->duracion=getInt("Ingrese la duracion en minutos: ");
    fflush(stdin);
    getString("Ingrese la descripcion: ", p->descripcion);
    p->puntaje=getInt("Ingrese el puntaje: ");
    fflush(stdin);
    p->estado=1;
}

int guardarPelicula(struct pelicula *p)
{
    FILE* fp;
    fp=fopen("bin.dat", "a+b");
    if(fp==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }    
    fwrite(p,sizeof(struct pelicula),1,fp);
    fclose(fp);

    return 1;
}

void imprimir ()
{
    FILE *bin;
    struct pelicula p;
    int cant;

    if ((bin=fopen("bin.dat","rb"))==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }

    while(!feof(bin))
    {
        cant=fread(&p,sizeof(p),1,bin);

        if(cant!=1)
        {
            if(feof(bin))
                break;
            else
            {
                printf("No leyo el ultimo registro");
                break;
            }
        }

        if(p.estado==1)
        printf("Pelicula\nNombre: %s\nGenero: %s\nDuracion: %d\nDescripcion: %s\nPuntaje: %d\n", p.nombre, p.genero, p.duracion, p.descripcion, p.puntaje);
        puts("---------------------------------------------------");
    }

    fclose(bin);
}

struct pelicula* modificarPelicula(char* nomFilm)
{
    FILE* fp = fopen("bin.dat","r+b");
    if(fp==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    struct pelicula* pp = malloc(sizeof(struct pelicula));
    int flagFound=0;
    while(fread(pp,sizeof(struct pelicula),1,fp)!=0)
    {
        
        if(strcmp(nomFilm, pp->nombre)==0 && pp->estado==1) 
        {//strupr
            fseek(fp, ftell(fp) - sizeof(struct pelicula), SEEK_SET);
            int opcion=0;
            while(opcion!=6)
            {
                opcion=getInt("Modificar:\n1. Nombre\n2. Genero\n3. Duracion\n4. Descripcion\n5. Puntaje\n6. Salir\n");
                fflush(stdin);
                switch (opcion)
                {
                case 1:
                    getString("Ingrese el nuevo nombre del film: ", pp->nombre);
                    break;

                case 2:
                    getString("Ingrese el genero: ", pp->genero);
                    break;

                case 3:
                    pp->duracion=getInt("Ingrese la duracion en minutos: ");
                    fflush(stdin);
                    break;

                case 4:
                    getString("Ingrese la descripcion: ", pp->descripcion);
                    break;

                case 5:
                    pp->puntaje=getInt("Ingrese el puntaje: ");
                    fflush(stdin);
                    break;
                }
            }
            fwrite(pp, sizeof(struct pelicula), 1, fp);
            flagFound=1;
            break;
        }
    }
    fclose(fp);

    if(flagFound==0)
        printf("No se encontro esa pelicula\n");

    free(pp);
    return NULL;
}

struct pelicula* eliminarPelicula(char* nomFilm)
{
    FILE* fp = fopen("bin.dat","r+b");
    if(fp==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    struct pelicula* pp = malloc(sizeof(struct pelicula));
    int flagFound=0;
    while(fread(pp,sizeof(struct pelicula),1,fp)!=0)
    {
        
        if(strcmp(nomFilm, pp->nombre)==0) 
        {
            fseek(fp, ftell(fp) - sizeof(struct pelicula), SEEK_SET);
            char opcion=getChar("Esta seguro que desea eliminarla? (s/n)");
            if(opcion=='s' || opcion=='S')
            {
			    pp->estado = 0;
			    fwrite(pp, sizeof(struct pelicula), 1, fp);
			    printf("La pelicula fue eliminada\n");
            }
            flagFound=1;
            break;
        }
    }
    fclose(fp);

    if(flagFound==0)
        printf("No se encontro esa pelicula\n");

    free(pp);
    return NULL;
}

void crearEncabezado(char* destino)
{
    FILE *origen;
    FILE *destin;

    char linea[100];
    origen=fopen("inicio.txt", "r");
    if(origen==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    destin=fopen(destino, "a");
    if(destin==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    fgets(linea, 100, origen);
    while(!feof(origen))
    {
        fputs(linea, destin);
        fgets(linea, 100, origen);
    }
    fclose(origen);
    fclose(destin);
}

struct pelicula* crearCuerpo(char* nomfilm, char* destino)
{
    FILE *origen;
    FILE *destin;

    origen=fopen("bin.dat","rb");
    if(origen==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    destin=fopen(destino, "a");
    if(destin==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    struct pelicula* pp = malloc(sizeof(struct pelicula));
    int flagFound=0;
    while(fread(pp, sizeof(struct pelicula), 1, origen)!=0)
    {
        
        if(strcmp(nomfilm, pp->nombre)==0) 
        {
            fprintf(destin, "            <article class='col-md-4 article-intro'>\n");
            fprintf(destin, "                <a href='#'>\n");
            fprintf(destin, "                    <img class='img-responsive img-rounded' src='http://ia.media-imdb.com/images/M/MV5BMjA5NTYzMDMyM15BMl5BanBnXkFtZTgwNjU3NDU2MTE@._V1_UX182_CR0,0,182,268_AL_.jpg' alt=''>\n");
            fprintf(destin, "                </a>\n");
            fprintf(destin, "                <h3>\n");
            fprintf(destin, "                    <a href='#'>%s</a>\n", pp->nombre);
            fprintf(destin, "                </h3>\n");
            fprintf(destin, "				<ul>\n");
            fprintf(destin, "					<li>Género: %s</li>\n", pp->genero);
            fprintf(destin, "					<li>Puntaje: %d</li>\n", pp->puntaje);
            fprintf(destin, "					<li>Duración: %d</li>\n", pp->duracion);
            fprintf(destin, "				</ul>\n");
            fprintf(destin, "                <p>%s</p>\n", pp->descripcion);
            fprintf(destin, "            </article>\n");
            flagFound=1;
            break;
        }
    }
    fclose(origen);
    fclose(destin);

    if(flagFound==0)
        printf("No se encontro esa pelicula\n");

    free(pp);
}

void crearPie(char* destino)
{
    FILE *origen;
    FILE *destin;

    char linea[100];
    origen=fopen("final.txt", "r");
    if(origen==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    destin=fopen(destino, "a");
    if(destin==NULL)
    {
        printf("No se pudo abrir el archivo");
        exit(1);
    }
    fgets(linea, 100, origen);
    while(!feof(origen))
    {
        fputs(linea, destin);
        fgets(linea, 100, origen);
    }
    fclose(origen);
    fclose(destin);
}

void cierre()
{
    printf("Cerrando el programa\n");
    printf("***                 \n");
    Sleep(100);
    system("cls");
    printf("Cerrando el programa\n");
    printf("   ***              \n");
    Sleep(100);
    system("cls");
    printf("Cerrando el programa\n");
    printf("      ***           \n");
    Sleep(100);
    system("cls");
    printf("Cerrando el programa\n");
    printf("         ***        \n");
    Sleep(100);
    system("cls");
    printf("Cerrando el programa\n");
    printf("            ***     \n");
    Sleep(100);
    system("cls");
    printf("Cerrando el programa\n");
    printf("               ***  \n");
    Sleep(100);
    system("cls");
    printf("Cerrando el programa\n");
    printf("                  ***\n");
    Sleep(700);
    system("cls");
}