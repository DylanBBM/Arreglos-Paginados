#include <iostream> 
#include <fstream> //Se usa para escribir en un archivo ifstream, ofstream
#include <string>
using namespace std;

int main(){
    fstream fout;
    int x = 10;

    fout.open("file.dat", ios::out | ios::binary);

    if(fout){
        fout.write((char*)&x, sizeof(int));
        fout.close();
    }
    else{
        cout << "Error abriendo el archivo\n";
    }
    return 0;
}

    /*Para leerlo
        fout.open("file.dat", ios::out | ios::binary );
    if(fout){
        unsigned short y;
        fout.read(reinterpret_cast<char*>(&y), sizeof(unsigned short));
        fout.close();
        cout << y << endl;
    }
    else
        cout <<"Error abriendo el archivo 2"<< endl;


    */
