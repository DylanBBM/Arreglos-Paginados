#include <iostream>
#include <fstream> //Para trabajor con archivos
#include <random>
#include <cstdint> //Para asegurarse siempre que los numeros sean de 4bytes


int main(){

    std::ofstream BinaryFile("BinaryIntFile.dat", std::ios::binary);

    std::mt19937 randomSeedGenerator(std::random_device{}());
    //Numeros de 4 bytes SIEMPRE SEA CUAL SEA LA COMPUTADORA
    int32_t randomNumber;

    //Tecnicamente con int sirve, pero long/long long es mas seguro.
    long long Limit = 134217728;

    for(long long i = 0; i<Limit; i++){
        randomNumber = randomSeedGenerator();

        //Write funciona: (dir EN CHAR , tamanio)
        BinaryFile.write((char*)&randomNumber, sizeof(int32_t));

    }
    BinaryFile.close();
    return 0;
}