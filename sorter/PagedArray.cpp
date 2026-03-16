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
        tiempo = 0;

        //Para las paginas por que no solo es 1
        for(size_t i = 0 ; i <MAX_PAGINAS; i++)
        { //Crear
            paginas[i] = new int[intxPagina];
        }

        for (size_t i = 0; i < MAX_PAGINAS; i++){ //Marca los paginas vacias
            paginasCargadas[i] = -1;
        }

        for (size_t i = 0; i < MAX_PAGINAS; i++){ 
            //Ninguna pagina se ha usado todavia
            ultimoUso[i] = 0;
        }
    }

    ~PagedArray();

    // Sobrecarga del operador [] si se escibre arr[indice] es esto
    int& operator[](size_t indice){

        //que pag y posicion dentro
        size_t numPagina = calcularNumPagina(indice);
        size_t offset = calcularOffsetEnPagina(indice);
        
        int slot = -1; //como decir ninguno de momento

        for(size_t i = 0; i < MAX_PAGINAS; i++)
        {
            if (paginasCargadas[i] == numPagina){ //si el slot esta vacio
                slot = i;
                break; 
            }
        }

        if(slot != -1) //Si hay que reemplazar 
        {
            pageHits++;

        }else {

            pageFaults++;
            cargarPagina(numPagina); //desde el disco

            //volver a buscar el slot donde se cargo
            for(size_t i = 0; i <MAX_PAGINAS; i++ ){
                if (paginasCargadas[i] == numPagina){
                    slot = i;
                    break;
                }
            }
        }
            tiempo++;
            ultimoUso[slot] = tiempo;
            return paginas[slot][offset]; 

    }

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

    void cargarPagina(size_t numPagina){

        int slot = -1;
        for(size_t i = 0; i < MAX_PAGINAS; i++){
            if (paginasCargadas[i]== -1){
                slot = i;
                break;
            }
        }
        if (slot == -1){
            slot = reemplazarPagina();
        }
        else{
            //Calcular donde empieza la página.
            size_t byteInicial = numPagina * intxPagina * sizeof(int);
            archivo.seekg(byteInicial, std::ios::beg);

            archivo.read((char*)paginas[slot],intxPagina* sizeof(int));
            paginasCargadas[slot] = numPagina;
        }
    } 

    void guardarPagina(size_t numPagina){

        if (numPagina == -1){
            return;
        }
        int slot = -1;

        for(size_t i = 0 ; i < MAX_PAGINAS; i++){
            if(paginasCargadas[i] == numPagina){
                slot = i;
                break;
            }

        }
        if (slot == -1){
            return;
        }
        size_t byteInicial= numPagina * intxPagina * sizeof(int);
        archivo.seekp(byteInicial, std::ios::beg);

        archivo.write((char*)paginas[slot], intxPagina * sizeof(int)
    );
    }

    size_t reemplazarPagina() // LRU
    {
        size_t menorUso = ultimoUso[0];
        size_t slot = 0;

        for(size_t i = 1; i < MAX_PAGINAS; i++)
        {
            if (ultimoUso[i] < menorUso)
            {
                menorUso = ultimoUso[i];
                slot = i;
            }
        }
        // guardar la pagina vieja antes de reemplazar
        guardarPagina(paginasCargadas[slot]);
        return slot; 
    } 

    //En que pagina esta pagedarray[indice]
    size_t calcularNumPagina(size_t indice) const 
    { 
        return indice / intxPagina; 
    }
    // y en que posicion dentro de esa pagina 
    size_t calcularOffsetEnPagina(size_t indice) const 
    { //No se hace con tamPagina por que eso esta en bytes
        return indice % intxPagina; 
    }

    std::fstream archivo;
    string rutaArchivo;         
    size_t tamPagina;          
    size_t totalElementos;     
    size_t intxPagina;
    size_t tiempo;

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