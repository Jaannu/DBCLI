//
// Created by ItsJaan on 08/25/2019.
//

#ifndef BASEDATOSVIRTUAL_DATABASE_H
#define BASEDATOSVIRTUAL_DATABASE_H

#include <iostream>
#include <fstream>
using namespace std;

class DataBase {
private:
    int tam;
public:
    char dbname[15];
    void crear();
    bool existe(string);
    void usar(string);
    bool is_file(string file);
    void eliminar(string ruta);
    void EliminarDb();
};


#endif //BASEDATOSVIRTUAL_DATABASE_H
