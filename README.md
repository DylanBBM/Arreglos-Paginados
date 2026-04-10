# Arreglos Paginados

Proyecto #1 — Algoritmos y Estructuras de Datos II (CE 1103)  
Instituto Tecnológico de Costa Rica  
I Semestre 2026
Dylan Bonilla Barquero
---

## Estructura del proyecto

```
La estructura del proyecto de la siguiente: 
En algorithms viven los algoritmos de ordenamiento
En generator vive el generador de archivos binarios de enteros aleatorios.
En sorter vive el ordenador (sorter) y PagedArray
ARREGLOS-PAGINADOS/
├── algorithms/               
│   ├── HeapSort.cpp / HeapSort.h
│   ├── MergeSort.cpp / MergeSort.h
│   ├── QuickSort.cpp / QuickSort.h
│   ├── RadixSort.cpp / RadixSort.h
│   └── CombSort.cpp / CombSort.h
├── generator/
│   └── generator.cpp
└── sorter/
    ├── PagedArray.cpp / PagedArray.h
    └── sorter.cpp
```

---

## Compilación
Antes de ejecutar los programas es necesaria la compilación de ciertas partes, si no se compila el programa no funcionará.
Es necesario contar con el compilador g++, instalado en el equipo.
### Generator

Para compilar generator se debe navegar a la carpeta "generator". Si partimos de la carpeta base del proyecto, el comando seria el siguiente:

``` bash
cd generator
```

Una vez ubicados en la carpeta "generator" se ejecuta el siguiente comando:

``` bash
g++ -o generator.exe generator.cpp -O2
```

Con esto se tendría generator compilado.

### Sorter
Para compilar sorter se debe navegar a la "sorter". Si se parte de la carpeta base del proyecto, el comando sería el siguiente:

``` bash
cd sorter
```

Una vez que se encuentre en la carpeta "sorter" ejecutar el siguiente comando:

```bash
g++ -o sorter.exe sorter.cpp PagedArray.cpp ../algorithms/QuickSort.cpp ../algorithms/MergeSort.cpp ../algorithms/HeapSort.cpp ../algorithms/RadixSort.cpp ../algorithms/CombSort.cpp -O2
```
Con esto se tendria sorter compilado.

Los demas archivos no se compilan.
---

## Ejecución

### Generator

Desde la carpeta `generator`, se ejecuta de la siguiente manera:

```bash
.\generator.exe -size SIZE -output OUTPUT_FILE_PATH
```

Los parámetros a cambiar corresponden a SIZE y OUTPUT_FILE_PATG
**Parámetros:**

| Parámetro | Descripción |
|-----------|-------------|
| `-size`   | Tamaño del archivo a generar. Valores válidos: `small`, `medium`, `large` |
| `-output` | Ruta de la carpeta donde se guardará el archivo generado, esta carpeta debe existir en el equipo. |

**Tamaños disponibles:**

| Valor | Tamaño |
|-------|--------|
| `small` | 256 MB |
| `medium`| 512 MB |
| `large` | 1024 MB |

Cualquier otro tamaño no es válido.

**Ejemplo:**

```bash
.\generator.exe -size medium -output "C:\Users\Usuario\Desktop\Archivos\carpeta"
```

Al finalizar la ejecución satisfactoria, generator imprime:
Archivo generado correctamente en "Ruta de archivo" con size de "cantidad" enteros aleatorios.
El nombre del archivo generado correpsonde BinaryIntFile.dat
---

### Sorter

Desde la carpeta `sorter`, se ejecuta de la siguiente manera:

```bash
.\sorter.exe -input <INPUT_FILE_PATH> -output <OUTPUT_FILE_PATH> -alg <ALGORITMO> -pageSize <PAGE_SIZE> -pageCount <PAGE_COUNT>
```

**Parámetros:**

| Parámetro | Descripción |
|-----------|-------------|
| `-input`     | Ruta del archivo binario a ordenar|
| `-output`    | Ruta donde se guardará el archivo ordenado|
| `-alg`       | Algoritmo de ordenamiento a utilizar|
| `-pageSize`  | Cantidad máxima de enteros por página|
| `-pageCount` | Cantidad de páginas en memoria máximas simultáneamente|

**Algoritmos disponibles:**

| Valor | Algoritmo |
|-------|-----------|
| `quick` | QuickSort |
| `merge` | MergeSort |
| `heap` | HeapSort |
| `radix` | RadixSort |
| `comb` | CombSort |

**Ejemplo:**

```bash
.\sorter.exe -input "C:\Users\Dylan\Desktop\Archivos\carpeta\BinaryIntFile.dat" -output "C:\Users\Dylan\Desktop\Archivos\carpeta\Sorted.dat" -alg quick -pageSize 4096 -pageCount 16
```
Si se ejecuta correctamente, se imprime la cantidad de enteros del archivo de input.

Al finalizar la ejecución, el sorter imprime:
- Tiempo de ejecución
- Algoritmo utilizado
- Cantidad de Page Hits y Page Faults

Se generan dos archivos de salida: uno en formato binario, que corresponde a una copia del archivo de entrada, apartir de este es que se ordenan los enteros y uno en formato de texto legible con los enteros separados por comas.
