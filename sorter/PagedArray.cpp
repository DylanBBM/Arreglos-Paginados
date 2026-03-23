#include <string>
#include <cstddef>  // para usar size_t para tamanios es un entero sin signo
#include <iostream> //Imprimir
#include <fstream> //Leer y escribir archivos

using string = std::string;

class PagedArray {
    //Todo lo que sorter ocupa
public:
    //Sin el & se crea un string temporal, entonces con el siempre se usa el mismo
    PagedArray(const string& filePath, size_t PageBSize){

        this->filePath= filePath;
        this->pageBSize = PageBSize;

        file.open(filePath, std::ios::in | std::ios::out | std::ios::binary);

        file.seekg(0, std::ios::end); 
        size_t sizeBytes = file.tellg();  // Donde estoy en bytes y el tamanio total
        file.seekg(0, std::ios::beg); 

        //Cantidad de elementos y cuantos hay por página.
        intAmount = sizeBytes / sizeof(int);
        intperPage = PageBSize / sizeof(int);

        pageHits = 0;
        pageFaults = 0;
        LRUcounter = 0;

        //El for es para las paginas por que no solo es 1
        for(size_t i = 0 ; i < MAX_PAGES; i++){   
            //Crear paginas en el heap
            pages[i] = new int[intperPage];
        }

        for (size_t i = 0; i < MAX_PAGES; i++){ //Marca los slots vacios
            loadedPages[i] = -1;
        }

        for (size_t i = 0; i < MAX_PAGES; i++){ 
            //Ninguna pagina se ha usado todavia es para el LRU
            lastUsed[i] = 0;
        }
    }

    PagedArray::~PagedArray(){

        //guardar paginas cargadas, para no perder datos
        for (size_t i = 0; i < MAX_PAGES; i++){

            //Si en ese slot hay una pagina
            if (loadedPages[i] != -1) {
                savePage(loadedPages[i]); 
            }
        }
        //Liberar memoria
        for(size_t i = 0; i < MAX_PAGES ; i++){
            delete[] pages[i];
        }

        file.close();
    }

    // Sobrecarga del operador [] si se escibre arr[indice] es esto
    int& operator[](size_t index){

        //que pag y posicion dentro
        size_t pageNum = getPageNum(index);
        size_t posPage = getPosPage(index);
        
        int slot = -1; //ninguno de momento 

        for(size_t i = 0; i < MAX_PAGES; i++)
        {
            //Aqui lo que hace es que si ya esta cargada cambia el slot
            if (loadedPages[i] == pageNum){ 
                slot = i;
                break; 
            }
        }

        // Si lo cambió entonces es por que si estaba cargada la pagina
        if(slot != -1) {

            pageHits++; 

        } else {
            //Si no estaba cargada:
            pageFaults++;
            loadPage(pageNum); //desde el disco

            // ** volver a buscar el slot donde se cargo
            for(size_t i = 0; i < MAX_PAGES; i++ ){
                if (loadedPages[i] == pageNum){
                    slot = i;
                    break;
                }
            }
        }
        LRUcounter++;
        lastUsed[slot] = LRUcounter;
        return pages[slot][posPage]; 

    }

    // Métodos para obtener estadísticas
    size_t getPageHits() const 
    { 
        return pageHits; 
    }
    size_t getPageFaults() const //solo devuelve el valor y no modifica el estado del objeto
    { 
        return pageFaults; 

    } //De ints
    size_t getSize() const 
    { 
        return intAmount; 
    }

private:
    //Esto solo lo ocupa paged array

    //Carga una página desde el archivo a RAM
    void loadPage(size_t PageNum){

        int slot = -1;

        //Si algun slot esta vacio usamos ese de una vez, esto se usa al inicio casi siempre
        for(size_t i = 0; i < MAX_PAGES; i++){
            if (loadedPages[i] == -1){
                slot = i;
                break;
            }
        }
        
        //Si todos los slots estan usados
        if (slot == -1){
            slot = replacePage();   //Reemplazar
        }

        //Calcular donde empieza la página.     
        size_t startingByte = PageNum * intperPage * sizeof(int);
        //ir donde empieza

        file.seekg(startingByte, std::ios::beg);
        //Leer la página desde el disco hacia RAM
        file.read((char*)pages[slot], intperPage * sizeof(int));
        
        //Esto es como para decir "Slot 2 ahora tiene la pagina 3"
        loadedPages[slot] = PageNum;
    }

    //Guardar una pagina de RAM al archivo
    void savePage(size_t PageNum){

        //Si no hay página
        if (PageNum == -1){
            return;
        }

        int slot = -1;
        //Buscar en que slot esta la pagina
        for(size_t i = 0 ; i < MAX_PAGES; i++){
            if(loadedPages[i] == PageNum){
                slot = i; //Que slot tiene esa pagina
                break;
            }

        }

        //Si no esta la pagina en la RAM
        if (slot == -1){
            return;
        }
        
        //Calcular posicion del archivo
        size_t StartingByte= PageNum * intperPage * sizeof(int);
        file.seekp(StartingByte, std::ios::beg);

        //Escribir de ram al archivo o disco
        file.write((char*)pages[slot], intperPage * sizeof(int)
    );
    }

    //Elegir que página cambiar con Last Recently used
    size_t replacePage() {

        //Primero suponemos que el slot 0 es el menor
        size_t LeastUsed = lastUsed[0];
        size_t slot = 0;

        for(size_t i = 1; i < MAX_PAGES; i++)
        {
            //Si es menor, se cambia
            if (lastUsed[i] < LeastUsed)
            {
                LeastUsed = lastUsed[i];
                slot = i;
            }
        }
        // guardar la pagina antes de reemplazar
        savePage(loadedPages[slot]);
        //Para cargar la nueva
        return slot; 
    } 

    //En que pagina esta pagedarray[indice]
    size_t getPageNum(size_t index) const 
    { 
        return index / intperPage; 
    }
    // y en que posicion dentro de esa pagina 
    size_t getPosPage(size_t index) const 
    { //No se hace con tamPagina por que eso esta en bytes
        return index % intperPage; 
    }

    //Variables
    std::fstream file;
    string filePath;         
    size_t pageBSize; //Bytes         
    size_t intAmount;     
    size_t intperPage;
    size_t LRUcounter; //Para Lru

    static const size_t MAX_PAGES = 4;

    /*Esto es punteros a las 4 pags
    que pagina esta cargada en cada espacio
    y para el lru
    */

    int* pages[MAX_PAGES]; 
    size_t loadedPages[MAX_PAGES];  
    size_t lastUsed[MAX_PAGES];   

    size_t pageHits;
    size_t pageFaults;  
}; 

