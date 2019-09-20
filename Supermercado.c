#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "Cliente.c"
#include "Cajero.c"
#include "Cola.c"

void *seleccionarProductos(void *data)
{
    struct Cliente *cliente = (struct Cliente *) data;
    
    sleep((*cliente).tiempoS);
    printf("Cliente N° %d, Preferencial: %d, tiempo de seleccion de productos: %d\n", (*cliente).numero, (*cliente).preferencial, (*cliente).tiempoS);
    if ((*cliente).preferencial == 0)
        insertar(*cliente);
    imprimir();
}

void identificarClientes (struct Cliente *clientes, int cantClientes) 
{
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
    
    while(1) {
        if(vacia() == 0 && raiz->info.preferencial == 0) {
            (*cajero).ocupado = 1;
            atendido = extraer();
            sleep(atendido.tiempoS);
            printf("\nCliente N° %d, Atención en caja: %d (Normal) Tiempo: %d\n\n", atendido.numero, (*cajero).numero, atendido.tiempoS*2);
            (*cajero).ocupado = 0;        
        }
    }
}

void identificarCajeros (struct Cajero *cajeros, int cantCajeros)
{
    struct Cajero *auxCajeros = cajeros;
    for (int i = 1; i <= cantCajeros; i++)
    {
        (*cajeros).numero = i;
        (*cajeros).ocupado = 0;
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

void simulacion (int cantCajas) 
{
    int cantClientes = 5;
    struct Cliente *clientes = malloc (cantClientes * sizeof(struct Cliente));
    int cantCajeros = 3;
    struct Cajero *cajeros = malloc (cantCajeros * sizeof(struct Cajero));

    identificarClientes(clientes, cantClientes);
    identificarCajeros(cajeros, cantCajeros);
}

int main () 
{
    int cantCajas = 3;
    simulacion(cantCajas);

    return 0;
}