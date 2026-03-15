#include <string>
#include <cstddef>   // size_t para tamaños e índices siempre es + 
#include <iostream>
#include <fstream>

using string = std::string;

class PagedArray {
    //Todo lo que sorter ocupa
public:

    PagedArray(const string& rutaArchivo, size_t tamPagina){

        this->rutaArchivo = rutaArchivo;
        this->tamPagina = tamPagina;
        archivo.open(rutaArchivo, std::ios::in | std::ios::out | std::ios::binary);

        archivo.seekg(0, std::ios::end); 
        size_t sizeBytes = archivo.tellg();  // Donde estoy en bytes
        archivo.seekg(0, std::ios::beg); 

        totalElementos = sizeBytes / sizeof(int);
        intxPagina = tamPagina / sizeof(int);
        pageHits = 0;
        pageFaults = 0;

        //Para las paginas
        for(size_t i = 0 ; i <MAX_PAGINAS; i++)
        { //Crear
            paginas[i] = new int[intxPagina];
        }

        for (size_t i = 0; i < MAX_PAGINAS; i++)
        { //Marca las paginas vacias
            paginasCargadas[i] = -1;
        }

        for (size_t i = 0; i < MAX_PAGINAS; i++)
        { //Ninguna pagina se ha usado todavia
            ultimoUso[i] = 0;
        }
    }

    ~PagedArray();

    // Sobrecarga del operador [] si alguien escribe arr[] es esto
    int& operator[](size_t indice);

    // Métodos para obtener estadísticas
    size_t obtenerPageHits() const 
    { 
        return pageHits; 
    }
    size_t obtenerPageFaults() const //solo devuelve el valor y no modifica el estado del objeto
    { 
        return pageFaults; 
    }
    size_t tamaño() const 
    { 
        return totalElementos; 
    }

private:

    void cargarPagina(size_t numPagina); // Ambos desde el disco
    void guardarPagina(size_t numPagina); 
    void reemplazarPagina(); // LRU

    size_t calcularNumPagina(size_t indice) const 
    { 
        return indice / tamPagina; 
    }
    size_t calcularOffsetEnPagina(size_t indice) const 
    { 
        return indice % tamPagina; 
    }

    std::fstream archivo;
    string rutaArchivo;         
    size_t tamPagina;          
    size_t totalElementos;     
    size_t intxPagina;

    static const size_t MAX_PAGINAS = 4;

    /*Esto es punteros a las 4 pags
    que pagina esta cargada en cada espacio
    y para el lru
    */
    int* paginas[MAX_PAGINAS]; 
    size_t paginasCargadas[MAX_PAGINAS];  
    size_t ultimoUso[MAX_PAGINAS];   

    size_t pageHits;
    size_t pageFaults;  
};