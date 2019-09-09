//
// Created by ItsJaan on 08/25/2019.
//

#ifndef BASEDATOSVIRTUAL_TABLE_H
#define BASEDATOSVIRTUAL_TABLE_H
#include <iostream>
#include "Structs.h"

using namespace std;
class Table {
public:
    char  name[15]={0};
    int n_column;
    int cont=0;
    int primeraColumna;

    void CrearTabla(string, int);
    int Buscar(string, string , int);
    int BuscarAux(string, string, int , int);
    void ElimTabla(string , int);
    void Insert(string , int);
    void Select(string , int);
    void EliminarRegistro(string , int);
    void ActualizarRegistro(string , int);
};


#endif //BASEDATOSVIRTUAL_TABLE_H
