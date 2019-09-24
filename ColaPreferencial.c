#include <stdio.h>
#include <stdlib.h>

struct nodoPreferencial {
    struct Cliente info;
    struct nodoPreferencial *sig;
};

struct nodoPreferencial *raizPreferencial = NULL;
struct nodoPreferencial *fondoPreferencial = NULL;

int vaciaPreferencial()
{
    if (raizPreferencial == NULL)
        return 1;
    else
        return 0;
}

void insertarColaPreferencial(struct Cliente x)
{
    struct nodoPreferencial *nuevo;
    nuevo = malloc(sizeof(struct nodoPreferencial));
    nuevo->info = x;
    nuevo->sig = NULL;
    if (vaciaPreferencial())
    {
        raizPreferencial = nuevo;
        fondoPreferencial = nuevo;
    }
    else
    {
        fondoPreferencial->sig = nuevo;
        fondoPreferencial = nuevo;
    }
}

struct Cliente extraerColaPreferencial()
{
   if (!vaciaPreferencial())
   {
        struct Cliente informacion = raizPreferencial->info;
        if (raizPreferencial == fondoPreferencial)
        {
            raizPreferencial = NULL;
            fondoPreferencial = NULL;
        }
        else
        {
            raizPreferencial = raizPreferencial ->sig;
        }
        return informacion;
   }
   else 
   {
       return;
   } 
}

void imprimirPreferencial()
{
    struct nodoPreferencial *reco = raizPreferencial;
    printf("Listado de todos los elementos de la cola.\n");
    while (reco != NULL)
    {
        printf("%i - ", reco->info.numero);
        reco = reco->sig;
    }
    printf("\n\n");
}


void liberarPreferencial()
{
    struct nodoPreferencial *reco = raizPreferencial;
    struct nodoPreferencial *bor;
    while (reco != NULL)
    {
        bor = reco;
        reco = reco->sig;
        free(bor);
    }
}