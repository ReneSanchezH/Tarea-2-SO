#include <iostream>
#include <string>
#include <sstream>

// Clase que será llamada en el programa monitor
class ping
{
    public:
        std::string ip;
        std::string cantidadPaquetes;
        std::string getIp();

	//metodos de la clase
        void ejecutarComando();
        int getCantidadPaquetesPerdidos();
        int getCantidadPaquetesRecibidos();
        int getCantidadPaquetesTransmitidos();

	//metodp constructor
        ping(std::string ip, std::string cantidadPaquetes);

    private:
        std::string respuesta;
        int paquetesPerdidos;
        int paquetesRecibidos;
        int paquetesTransmitidos;
        void separarRespuesta();
};

// constructor
ping::ping(std::string ip, std::string cantidadPaquetes)
{
    this->ip = ip;
    this->cantidadPaquetes = cantidadPaquetes;
}

// almacenar la respuesta en variables
void ping::separarRespuesta()
{
    std::stringstream inputStringstream(this->respuesta);
    std::string aux;
    getline(inputStringstream, aux, '\n');
    this->paquetesTransmitidos = std::stoi(aux);
    getline(inputStringstream, aux, '\n');
    this->paquetesRecibidos = std::stoi(aux);
    this->paquetesPerdidos = this->paquetesTransmitidos - this->paquetesRecibidos;
}

// ejecuta el comando ping y separa los paquetes transmitidos con los paquetes recibidos
void ping::ejecutarComando()
{
    std::string grep = " | grep -oP '[\\d+.,]+(?= packets transmitted)'\\|'[\\d+.,]+(?= received)'";

    // comando ping a ejecutar
    std::string comando = "ping -q -c " + this->cantidadPaquetes + " " + this->ip + grep;

     char buffer[256];
    FILE *pipe = popen(comando.c_str(), "r");
    if (!pipe)
    {
        return;
    }
    while (!feof(pipe))
    {
        if (fgets(buffer, 256, pipe) != NULL)
            this->respuesta += buffer;
    }
    pclose(pipe);
    this->separarRespuesta();
}

//Métodos
// retorna la cantidad de paquetes recibidos
int ping::getCantidadPaquetesRecibidos()
{
    return this->paquetesRecibidos;
}

// retorna la cantidad de paquetes transmitidos
int ping::getCantidadPaquetesTransmitidos()
{
    return this->paquetesTransmitidos;
}

// retorna la cantidad de paquetes perdidos
int ping::getCantidadPaquetesPerdidos()
{
    return this->paquetesPerdidos;
}
