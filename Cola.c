#include <stdio.h>
#include <stdlib.h>

struct nodo {
    struct Cliente info;
    struct nodo *sig;
};

struct nodo *raiz = NULL;
struct nodo *fondo = NULL;

int vacia()
{
    if (raiz == NULL)
        return 1;
    else
        return 0;
}

void insertarColaNormal(struct Cliente x)
{
    struct nodo *nuevo;
    nuevo = malloc(sizeof(struct nodo));
    nuevo->info = x;
    nuevo->sig = NULL;
    if (vacia())
    {
        raiz = nuevo;
        fondo = nuevo;
    }
    else
    {
        fondo->sig = nuevo;
        fondo = nuevo;
    }
}

struct Cliente extraerColaNormal()
{
   if (!vacia())
   {
        struct Cliente informacion = raiz->info;
        if (raiz == fondo)
        {
            raiz = NULL;
            fondo = NULL;
        }
        else
        {
            raiz = raiz->sig;
        }
        return informacion;
   }
   else 
   {
       return;
   } 
}

void imprimir()
{
    struct nodo *reco = raiz;
    printf("Listado de todos los elementos de la cola.\n");
    while (reco != NULL)
    {
        printf("%i - ", reco->info.numero);
        reco = reco->sig;
    }
    printf("\n\n");
}


void liberar()
{
    struct nodo *reco = raiz;
    struct nodo *bor;
    while (reco != NULL)
    {
        bor = reco;
        reco = reco->sig;
        free(bor);
    }
}