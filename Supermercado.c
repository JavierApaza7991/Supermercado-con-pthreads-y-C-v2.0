#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "Cliente.c"
#include "Cajero.c"
#include "Cola.c"
#include "ColaPreferencial.c"

int cantClientes = 3;
int cantCajeros = 0;
int clientesAtendidos = 0;
int minutos = 0;
int segundos = 0;

void *seleccionarProductos(void *data)
{
    struct Cliente *cliente = (struct Cliente *) data;

    printf("%d:%d: \"Cliente %d ", minutos, segundos, (*cliente).numero);
    if ((*cliente).preferencial == 1) printf("preferencial");
    else printf("normal");
    printf(" llegó al supermercado y comprará durante %d segundos\"\n\n", (*cliente).tiempoS);

    sleep((*cliente).tiempoS);

    printf("%d:%d: \"Cliente %d ", minutos, segundos, (*cliente).numero);
    if ((*cliente).preferencial == 1) printf("preferencial");
    else printf("normal");
    printf(" terminó de comprar\"\n\n");

    if ((*cliente).preferencial == 0)
        insertarColaNormal(*cliente);
    else
        insertarColaPreferencial(*cliente);
}

void *identificarClientes (void *data) 
{
    struct Cliente *clientes = (struct Cliente *) data;
    
    struct Cliente *auxClientes = clientes;
    for (int i = 1; i <= cantClientes; i++)
    {
        (*clientes).numero = i;
        srand(time(0) + (*clientes).numero);
        (*clientes).preferencial = rand()%2;
        clientes++;
    }

    pthread_t *hilosClientes = malloc(cantClientes * sizeof(pthread_t));
    pthread_t *auxHilosClientes = hilosClientes;

    clientes = auxClientes;

    for (int i = 0; i < cantClientes; i++)
    {
        srand(time(0) + (*clientes).numero);
        (*clientes).tiempoS = rand()%10+1;
        pthread_create(hilosClientes, NULL, *seleccionarProductos, clientes);
        hilosClientes++;
        clientes++;
    }
    
    for (int i = 0; i < cantClientes; i++)
    {   
        pthread_join(*auxHilosClientes, NULL);
        auxHilosClientes++;
    }
}

void *iniciarCajero (void *data)
{
    struct Cajero *cajero = (struct Cajero *) data;
    struct Cliente atendido;
    
    while(clientesAtendidos < cantClientes) {
        sleep(0.01);

        if((*cajero).preferencial == 1)
        {
            if (!vaciaPreferencial())
            {
                atendido = extraerColaPreferencial();
                printf("%d:%d: \"Cliente %d preferencial está pasando a ser atendido por la Caja %d preferencial\"\n\n", minutos, segundos, atendido.numero, (*cajero).numero);
                
                sleep(atendido.tiempoS);
            
                printf("%d:%d: \"Cliente %d preferencial ha sio atendido por la Caja %d preferencial\"\n\n", minutos, segundos, atendido.numero, (*cajero).numero);

                clientesAtendidos++;
            } else if (!vacia) 
            {
                atendido = extraerColaNormal();
                printf("%d:%d: \"Cliente %d normal está pasando a ser atendido por la Caja %d preferencial\"\n\n", minutos, segundos, atendido.numero, (*cajero).numero);
                
                sleep(atendido.tiempoS);
            
                printf("%d:%d: \"Cliente %d normal ha sio atendido por la Caja %d preferencial\"\n\n", minutos, segundos, atendido.numero, (*cajero).numero);
            
                clientesAtendidos++;
            }
        } else if (vacia() == 0 && (*cajero).preferencial == 0) 
        {
            (*cajero).ocupado = 1;
            atendido = extraerColaNormal();

            printf("%d:%d: \"Cliente %d normal está pasando a ser atendido por la Caja %d normal\"\n\n", minutos, segundos, atendido.numero, (*cajero).numero);

            sleep(atendido.tiempoS);

            printf("%d:%d: \"Cliente %d normal ha sido atendido por la Caja %d normal\"\n\n", minutos, segundos, atendido.numero, (*cajero).numero);

            (*cajero).ocupado = 0;  

            clientesAtendidos++;
        }
    }
}

void *identificarCajeros (void *data)
{
    struct Cajero *cajeros = (struct Cajero *) data;
    
    struct Cajero *auxCajeros = cajeros;
    for (int i = 1; i <= cantCajeros; i++)
    {
        (*cajeros).numero = i;
        (*cajeros).ocupado = 0;

        //Solo la primera caja será preferencial
        if (i == 1)
            (*cajeros).preferencial = 1;
        else
            (*cajeros).preferencial = 0;
        cajeros++;
    }

    pthread_t *hilosCajeros = malloc(cantCajeros * sizeof(pthread_t));
    pthread_t *auxHilosCajeros = hilosCajeros;

    cajeros = auxCajeros;

    for (int i = 0; i < cantCajeros; i++)
    {
        pthread_create(hilosCajeros, NULL, *iniciarCajero, cajeros);
        hilosCajeros++;
        cajeros++;
    }

    for (int i = 0; i < cantCajeros; i++)
    {   
        pthread_join(*auxHilosCajeros, NULL);
        auxHilosCajeros++;
    }
}

void *cronometro (void *data)
{
    while(clientesAtendidos < cantClientes)
    {
        if(segundos == 60) {minutos++, segundos = 0;}
        segundos++;
        sleep(1);
    }
}

void simulacion () 
{
    struct Cliente *clientes = malloc (cantClientes * sizeof(struct Cliente));
    int cantCajeros = 3;
    struct Cajero *cajeros = malloc (cantCajeros * sizeof(struct Cajero));

    pthread_t proceso1;
    pthread_t proceso2;
    pthread_t cronom;

    pthread_create(&proceso1, NULL, *identificarClientes, clientes);
    pthread_create(&proceso2, NULL, *identificarCajeros, cajeros);
    pthread_create(&cronom, NULL, *cronometro, NULL);

    pthread_join(proceso1, NULL);
    pthread_join(proceso2, NULL);
    pthread_join(cronom, NULL);
}

int main () 
{
    cantCajeros = 3;
    simulacion();

    return 0;
}