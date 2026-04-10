#include <string>
#include <cstddef>  //para usar size_t para tamanios es un entero sin signo
#include <iostream> //Imprimir
#include <fstream>  //Leer y escribir archivos
#include "PagedArray.h"

//Clase que utiliza un arreglo dividido em paginas en memoria
PagedArray::PagedArray(const std::string& filePath, size_t pageSize, size_t pageCount){

    this->filePath= filePath;
    this->pageSize = pageSize;
    this->pageCount = pageCount;

    //in para leer out escribir
    file.open(filePath, std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo desde PagedArray");
    }

    file.seekg(0, std::ios::end); 
    size_t sizeBytes = file.tellg();  // Donde estoy en bytes y el tamanio total
    file.seekg(0, std::ios::beg); 

    //Cantidad de enteros y cuantos hay por página.
    intAmount = sizeBytes / sizeof(int);
    intsPerPage = pageSize; // ahora es cantidad de enteros por pagina

    pageHits = 0;
    pageFaults = 0;
    LRUcounter = 0;

    //Crear estructuras dinámicas según pageCount
    pages = new int*[pageCount];
    loadedPages = new int[pageCount];
    lastUsed = new size_t[pageCount];
    dirtyPage = new bool[pageCount];

    //Crear las paginas en el heap con memoria reservada
    for(size_t i = 0 ; i < pageCount; i++){   
        pages[i] = new int[intsPerPage];
    }

    //Marca los slots vacios, no hay ninguna pagina cargada
    for (size_t i = 0; i < pageCount; i++){ 
        loadedPages[i] = -1;
    }

    //Ninguna pagina ha sido usada todavia
    for (size_t i = 0; i < pageCount; i++){ 
        lastUsed[i] = 0;
    }

    //Ninguna pagina ha sido modificada todavia
    for (size_t i = 0; i<pageCount; i++){
        dirtyPage[i] = false;
    }
}

//Destructor
PagedArray::~PagedArray(){

    //guardar paginas cargadas, para no perder datos
    for (size_t i = 0; i < pageCount; i++){

        //Si en ese slot hay una pagina
        if (loadedPages[i] != -1) {
            savePage(loadedPages[i]); 
        }
    }

    //Liberar memoria
    for(size_t i = 0; i < pageCount ; i++){
        delete[] pages[i];
    }

    delete[] pages;
    delete[] loadedPages;
    delete[] lastUsed;
    delete[] dirtyPage;

    file.close();
}

// Sobrecarga del operador [] si se escibre arr[indice] es esto
int& PagedArray::operator[](size_t index){
    
    if (index >= intAmount) {
        throw std::out_of_range("Error: indice fuera de rango");
    }

    //que pag y posicion dentro
    size_t pageNum = getPageNum(index);
    size_t posPage = getPosPage(index);
        
    int slot = -1; //ninguno de momento 

    for(size_t i = 0; i < pageCount; i++){
            //Aqui lo que hace es que si ya esta cargada cambia el slot
        if (loadedPages[i] == (int)pageNum){ 
            slot = i; //Para saber si hay pagehit 
            break; 
        }
    }

        // Si lo cambió entonces es por que si estaba cargada la pagina
    if(slot != -1) {
        pageHits++; 
    } 
    else{
        //Si no estaba cargada:
        pageFaults++;
        slot = loadPage(pageNum); //desde el disco


    }

    LRUcounter++;
    lastUsed[slot] = LRUcounter;
    dirtyPage[slot] = true;

    return pages[slot][posPage]; 
 }

//Metodo que retorna la cantidad de pageHits
size_t PagedArray::getPageHits() const 
{ 
    return pageHits; 
}

//Metodo que retorna la cantidad de pageFaults
size_t PagedArray::getPageFaults() const 
{ 
    return pageFaults; 
}

//Metodo que retorna la cantidad de ints por pagina
size_t PagedArray::getSize() const 
{ 
    return intAmount; 
}


//Metodo que carga una página desde el archivo (disco) a RAM
int PagedArray::loadPage(size_t PageNum){

    int slot = -1;

    //Si algun slot esta vacio usamos ese de una vez, esto se usa al inicio casi siempre
    for(size_t i = 0; i < pageCount; i++){
        if (loadedPages[i] == -1){
            slot = i;
            break;
        }
    }
        
    //Si todos los slots estan usados
    if (slot == -1){
        slot = replacePage();//Reemplazar
    }

    //Calcular donde empieza la página.     
    size_t startingByte = PageNum * intsPerPage * sizeof(int);

    //ir donde empieza
    file.seekg(startingByte, std::ios::beg);

    //Leer la página desde el disco hacia RAM
    size_t remainingInts = intAmount - (PageNum * intsPerPage);
    size_t intsToRead;

    if (intsPerPage < remainingInts) {
        intsToRead = intsPerPage;
    } else {
        intsToRead = remainingInts;
    }
    file.read((char*)pages[slot], intsToRead * sizeof(int));

    //Si se esta leyendo mas alla del final del archivo
    if (file.eof()) {
        //Deja el stream limpio, solo limpia el estado interno
        file.clear();
    }
    else if (!file) {
        throw std::runtime_error("Error leyendo archivo");
    }
        
    dirtyPage[slot] = false;

    //Esto es como para decir "Slot 2 ahora tiene la pagina 3"
    loadedPages[slot] = (int)PageNum;

    return slot;
}

//Metodo que guarda una pagina de memoria al archivo (disco)
void PagedArray::savePage(size_t PageNum){

    int slot = -1;

    //Buscar en que slot esta la pagina
    for(size_t i = 0 ; i < pageCount; i++){
        if(loadedPages[i] == (int)PageNum){
            slot = i; //Que slot tiene esa pagina
            break;
        }
    }

    //Si no esta la pagina en la RAM
    if (slot == -1){
        return;
    }

    if (!dirtyPage[slot]) {
        return;
    }

        
    //Calcular posicion del archivo
    size_t StartingByte= PageNum * intsPerPage * sizeof(int);
    file.seekp(StartingByte, std::ios::beg);

    //Escribir de ram al archivo o disco
    size_t remainingInts = intAmount - (PageNum * intsPerPage);

    size_t intsToWrite;
    //Si faltan
    if (intsPerPage < remainingInts) {
        intsToWrite = intsPerPage;
    } else {
        intsToWrite = remainingInts;
    }

    file.write((char*)pages[slot], intsToWrite * sizeof(int));
    if (!file) {
        throw std::runtime_error("Error escribiendo archivo");
    }
    dirtyPage[slot] = false;
}

//Metodo que elige que página cambiar con Last Recently Used
size_t PagedArray::replacePage() {

    //Primero suponemos que el slot 0 es el menor
    size_t LeastUsed = lastUsed[0];
    size_t slot = 0;

    for(size_t i = 1; i < pageCount; i++)
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

//Metodo que retorna en que pagina esta pagedarray[indice]
size_t PagedArray::getPageNum(size_t index) const 
{ 
    return index / intsPerPage; 
}

//Metodo que retorna en que posicion dentro de esa pagina 
size_t PagedArray::getPosPage(size_t index) const 
{ 
    return index % intsPerPage; 
}