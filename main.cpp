#include <iostream>
#include <fstream>
#include "DataBase.h"
using namespace std;
int main() {
    DataBase d;
    string n;

    int opc=0;
    cout<<"*****VIRTUAL DATABASE ******\n";
    do{
        cout<<"1. Crear Base Datos.\n";
        cout<<"2. Utilizar Base Existente.\n";
        cout<<"3. Eliminar Base de Datos\n";
        cout<<"4. Salir.\n";
        cin>>opc;
        if(opc==1){
            d.crear();
        }else if(opc==2) {
            cout << "Ingrese nombre Base de Datos:";
            cin >> n;
            if (d.existe(n)) {
                d.usar(n);
            } else {
                perror("No se encontro Base de Datos\n");

            }
        }else if(opc==3){
            d.EliminarDb();
        }
    }while(opc!=4);
    return 0;
}