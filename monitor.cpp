#include <iostream>
#include <string>
#include <omp.h>
#include <fstream>
#include <vector>
#include <cctype>
#include "ping.h"

struct Nodo //nodo de una pila para almacenar las ip
{
    std::string ip;
    std::string estado;
    int paquetesTransmitidos;
    int paquetesRecibidos;
    int paquetesPerdidos;
    Nodo *siguiente;
};

void agregarPila(Nodo *&, std::string ip, int paquetesTransmitidos, int paquetesRecibidos, int paquetesPerdidos);
void imprimirPila(Nodo *);
bool esNumero(const std::string &str);
 
int main(int argc, char *argv[])
{
    Nodo *pila = NULL; //inicialización de la pila
    int cantidadIp = 0;
    int paquetesRecibidos;
    int paquetesTransmitidos;
    int cantidadElementos = 0;
    std::string linea;
    std::string archivoEntrada = argv[1];
    std::string cantidadPaquetes = argv[2];
    std::vector<std::string> ips;
    std::ifstream archivo(archivoEntrada);


    // recorrer archivo para almacenarlas las ip
    while (std::getline(archivo, linea))
    {
        if (linea != "")
        {
            ips.push_back(linea);
        }
    }
    archivo.close();

    // cuenta las cantidades de ip
    cantidadIp = ips.size();

    // hebras segun la cantidad de ip encontradas
    #pragma omp parallel num_threads (cantidadIp)
    {
        #pragma omp for
        for (auto x : ips)
        {
	    //llamar a ping.h y crear un objeto de la clase
	    //para poder ocupar sus métodos
            ping  obj(x, cantidadPaquetes);
            obj.ejecutarComando();

            #pragma omp critical
            {
		//cuando la sección crítita está pretgida
		//se llama a la función agregarPila()
                agregarPila(pila, obj.ip, obj.getCantidadPaquetesTransmitidos(), obj.getCantidadPaquetesRecibidos(), obj.getCantidadPaquetesPerdidos());
            }
        }
    }
    // mostrar los elementos de la pila
    imprimirPila(pila);
}

// comprueba si un string es solo numeros
bool esNumero(const std::string &str)
{
    for (char const &val : str)
    {
        if (std::isdigit(val) == 0){
            return false;
        }
    }
    return true;
}

void agregarPila(Nodo *&pila, std::string ip, int paquetesTransmitidos, int paquetesRecibidos, int paquetesPerdidos)
{
    Nodo *nuevoNodo = new Nodo();
    nuevoNodo->ip = ip;
    nuevoNodo->estado = (paquetesRecibidos > 0) ? "UP" : "DOWN";
    nuevoNodo->paquetesTransmitidos = paquetesTransmitidos;
    nuevoNodo->paquetesRecibidos = paquetesRecibidos;
    nuevoNodo->paquetesPerdidos = paquetesPerdidos;
    nuevoNodo->siguiente = pila;
    pila = nuevoNodo;
}

// imprimir los elementos de la pila
void imprimirPila(Nodo *pila)
{
    std::cout << "IP\t\tESTADO\tTRANS.\tRECIB.\tPERD." << std::endl;
    Nodo *aux = pila;
    while (aux != NULL)
    {
        std::cout << aux->ip << "\t";
        std::cout << aux->estado << "\t";
        std::cout << aux->paquetesTransmitidos << "\t";
        std::cout << aux->paquetesRecibidos << "\t";
        std::cout << aux->paquetesPerdidos << std::endl;
        aux = aux->siguiente;
    }
}
