//
// Created by ItsJaan on 08/25/2019.
//

#ifndef BASEDATOSVIRTUAL_STRUCTS_H
#define BASEDATOSVIRTUAL_STRUCTS_H


struct  DataBlock512{
    char data[512];/*
                  * TABLA TA
                  * COlUMNA CO,
                  * DATOS DA,
                  * NULL NU
                  */
};
struct  DataBlock1024{
    char data[1024];
};
struct  DataBlock2048{
    char data[2048];
};
struct  DataBlock3072{
    char data[3072];
};
struct  DataBlock4096{
    char data[4096];
};


struct DBMetadata{
    char  name[15];
    int tamano_bloque;
    int cantidad_bloques;
    int tamano_disco;
};
struct TableColumns{
    char nombreTabla[15];
    char nombre[15];
    char tipo[7];
    int longi;

};

#endif //BASEDATOSVIRTUAL_STRUCTS_H
