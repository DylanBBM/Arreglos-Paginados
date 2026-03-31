#include <iostream>
#include <fstream> // Para trabajar con archivos
#include <random>
#include <algorithm>
#include <cstdint> // Para asegurarse siempre que los numeros sean de 4 bytes
#include <stdexcept>
#include <string>
#include <filesystem> //


/* Este metodo no se si incluirlo
void checkDiskSpace(const std::string& folderPath, long long requiredBytes) {
    // Obtener espacio libre en la unidad de folderPath
    std::error_code error;  
    auto spaceInfo = std::filesystem::space(folderPath, error);

    if(ec) {
        throw std::invalid_argument("Error: No se pudo obtener información del espacio en disco.");
    }

    if(spaceInfo.available < requiredBytes) {
        throw std::invalid_argument("Error: No hay suficiente espacio en disco para generar el archivo.");
    }
}*/

bool checkSize(std::string size){

    std::string sizeLower = size;
    std::transform(sizeLower.begin(), sizeLower.end(), sizeLower.begin(), ::tolower);

    if(sizeLower == "small" || sizeLower == "medium" || sizeLower == "large"){

        return true;
    }

    return false;
}


// Valida los argumentos y devuelve el size en minuscula 
std::string inputValidation(int argc, char* argv[]){

    //Valida tamanio de entrada
    if(argc != 5){
        throw std::invalid_argument("Error: La cantidad de argumentos recibidos debe ser de 5 exactamente.");
    }
    //Valida que el argumento 1 y 3 sea size y output
    if(std::string(argv[1]) != "-size" || std::string(argv[3]) != "-output"){
        throw std::invalid_argument("Error: Uso correcto: generator -size <SIZE> -output <OUTPUT FILE PATH>");
    }

    // Lo siguiente no se si se puede utilizar preguntar profe**
    std::string folderPath = argv[4];

    if(!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath)){
        throw std::invalid_argument("Error: La carpeta ingresada no existe en el sistema.");
    }
    //Obtenemos el size del archivo
    std::string size = argv[2];

    std::transform(size.begin(), size.end(), size.begin(), ::tolower);

    //En caso de que no sea de los 3 permitidos
    if(!checkSize(size)){
        throw std::invalid_argument("Error: El argumento <SIZE> debe ser small, medium o large");
    }

    return size;
}

int main(int argc, char* argv[]){ 

    std::string size;
    try{
        size = inputValidation(argc, argv); //Para size
    } catch(const std::invalid_argument& error){
        std::cout << error.what() << std::endl; //Para los throw
        std::cout << "Presione ENTER para salir" << std::endl;
        std::cin.get();
        return 1;
    }

   // Calcular cantidad de enteros
    long long Limit = 0; // Numero de enteros de 4 bytes

    if(size == "small") {

        Limit = 256LL * 1024 * 1024 / 4; 
        //No se si usar esto checkDiskSpace(argv[4], Limit);
    } else if(size == "medium") {

        Limit = 512LL * 1024 * 1024 / 4; 
    } else if(size == "large") {

        Limit = 1024LL * 1024 * 1024 / 4; 
}
    // Obtener la ruta
    std::string FolderPath = argv[4];
    std::string fileName;

    if(size == "small") {
        fileName = "SmallBinaryIntFile.dat";
    } else if(size == "medium") {
        fileName = "MediumBinaryIntFile.dat";
    } else if(size == "large") {
        fileName = "LargeBinaryIntFile.dat";
    }

    // Esto es para que funcione si no termina en \\ o /
    if(FolderPath.back() != '\\' && FolderPath.back() != '/'){
        FolderPath += "\\";
    }

    std::string outputFilePath = FolderPath + fileName;

    // Objeto para escribir
    std::ofstream BinaryFile(outputFilePath, std::ios::binary);
    if(!BinaryFile){
        std::cout << "Error no se pudo abrir el archivo" << '\n';
        std::cout << "Presiona Enter para salir" << std::endl;
        std::cin.get();
        return 1;
    }

    // Hace una semilla para los numeros random
    std::mt19937 randomSeedGenerator(std::random_device{}());
    //Para solo positivos
    std::uniform_int_distribution<int32_t> dist(0, INT32_MAX);
    int randomNumber;

    // Hasta el limite
    for(long long i = 0; i < Limit; i++){
        randomNumber = dist(randomSeedGenerator);
        // Write (direccion char, tamanio)
        BinaryFile.write((char*)&randomNumber, sizeof(int32_t));
    }

    BinaryFile.close();
    // Notificar 
    std::cout << "Archivo generado correctamente en " << outputFilePath << " con size de " << Limit << " enteros aleatorios" << '\n';
    std::cout << "Presione Enter para salir" << std::endl;
    std::cin.get();
    return 0;
}
