//
// Created by ItsJaan on 08/25/2019.
//
#include "DataBase.h"
#include "DataBase.h"
#include "Structs.h"
#include "Table.h"

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

bool DataBase::existe(string n) {
    fstream lectura(n + ".dat" , ios::in | ios::out | ios::binary);

    if(!lectura){
        return false;
    }
    return true;

}
void DataBase::crear() {
    DBMetadata metadata;

    cout<<"Ingrese el nombre: ";
    cin>>dbname;
    string n(dbname);

    if(existe(dbname)==false) {
        ofstream basedatos(n+ ".dat", ios::out | ios::binary);
        cout<<"Ingrese Tamano de Base de Datos: ";
        cin >> metadata.tamano_disco;
        strcpy(metadata.name, dbname);

        BACK:
        cout <<"\tIngrese Tamano de Bloques de Datos\n512 || 1024 || 2048 || 3072 || 4096 : ";
        cin >> metadata.tamano_bloque;
        if(metadata.tamano_bloque!=512 && metadata.tamano_bloque!=1024 && metadata.tamano_bloque!=2048 &&
                metadata.tamano_bloque!=3072 && metadata.tamano_bloque!=4096){
            cout<<"Opcion Invalida\n";
            goto BACK;

        }else {
            if (metadata.tamano_bloque == 512)
                metadata.cantidad_bloques = ((metadata.tamano_disco * 1024) / (.5));//paso ambos a KBS
            else
                metadata.cantidad_bloques = ((metadata.tamano_disco * 1024) / (metadata.tamano_bloque / 1024));
            cout << "--------------------------------------------------------\n";
            cout << "Tamano Bloque: " << metadata.tamano_bloque << endl;
            cout << "Tamano Base Datos: " << metadata.tamano_disco <<" Mbs"<< endl;
            cout << "Cantidad Bloques: " << metadata.cantidad_bloques << endl;
            cout << endl;

            basedatos.seekp(0, ios::end);
            basedatos.write(reinterpret_cast<char *>(&metadata), sizeof(DBMetadata));
            //basedatos.write(reinterpret_cast<char *>(&map), sizeof(Bitmap));

            if (metadata.tamano_bloque == 512) {
                for (int i = 0; i < metadata.cantidad_bloques; i++) {
                    DataBlock512 * bloq = new DataBlock512();
                    strcpy(bloq->data, "N");
                    basedatos.write(reinterpret_cast<char *>(bloq), sizeof(DataBlock512));

                }
            } else if (metadata.tamano_bloque == 1024) {
                for (int i = 0; i < metadata.cantidad_bloques; i++) {
                    DataBlock1024 *bloq = new DataBlock1024();
                    strcpy(bloq->data, "N");
                    basedatos.write((char *) bloq, sizeof(DataBlock1024));
                }
            } else if (metadata.tamano_bloque == 2048) {
                for (int i = 0; i < metadata.cantidad_bloques; i++) {
                    DataBlock2048 *bloq = new DataBlock2048();
                    strcpy(bloq->data, "N");
                    basedatos.write((char *) bloq, sizeof(DataBlock2048));
                }
            } else if (metadata.tamano_bloque == 3072) {
                for (int i = 0; i < metadata.cantidad_bloques; i++) {
                    DataBlock3072 *bloq = new DataBlock3072();
                    strcpy(bloq->data, "N");
                    basedatos.write((char *) bloq, sizeof(DataBlock3072));
                }
            } else if (metadata.tamano_bloque == 4096) {
                for (int i = 0; i < metadata.cantidad_bloques; i++) {
                    DataBlock4096 *bloq = new DataBlock4096();
                    strcpy(bloq->data, "N");
                    basedatos.write((char *) bloq, sizeof(DataBlock4096));
                }
            }
            cout << "Listo! \n";
            cout << "--------------------------------------------------------\n";
        }
        basedatos.close();

    }else
        cout<<"Disco existente";
}
void DataBase::usar(string n) {
    fstream base(n + ".dat", ios::in | ios::out | ios::binary);
    if (!base) {
        perror("No se encontro Base De Datos \n");
    }
    string palabra;
    DBMetadata *metadata=new DBMetadata();
    base.seekg(0,ios::beg);
    base.read(reinterpret_cast<char *>(metadata), sizeof(DBMetadata));
    int tam = metadata->tamano_bloque;
    cout<<"\n\n **** METADATA ***\n";
    cout<<"Nombre del Disco: "<<n<<"\n";
    cout<<"Peso Individual de cada bloque: "<<metadata->tamano_bloque<<" Bytes"<<"\n";
    cout<<"Cantidad de bloques en el disco: "<<metadata->cantidad_bloques<<"\n";
    cout<<"Capacidad del Disco: "<<metadata->tamano_disco<<" mb"<<"\n";
    strcpy(dbname, metadata->name);


    int opc;
    do {
        cout << "\nBASE DE DATOS :  " << dbname << "\n\t" << endl;
        cout<<"\t1. Crear Tabla\n";
        cout<<"\t2. Borrar Tabla\n";
        cout<<"\t3. Insertar Registros en  Tabla\n";
        cout<<"\t4. Eliminar Registros en Tabla\n";
        cout<<"\t5. Actualizar Registos en  Tabla\n";
        cout<<"\t6. Select de Registros en Tabla\n";
        Table *tabla = new Table();
        cin>>opc;
        switch(opc){
            case 1:
                tabla->CrearTabla(dbname , metadata->tamano_bloque);
                break;
            case 2:
                tabla->ElimTabla(dbname , metadata->tamano_bloque);
                break;
            case 3:
                tabla->Insert(dbname , metadata->tamano_bloque);
                break;
            case 4:
                tabla->EliminarRegistro(dbname  , metadata->tamano_bloque);
                break;
            case 5:
                tabla->ActualizarRegistro(dbname, metadata->tamano_bloque);
                break;
            case 6:
                tabla->Select(dbname ,metadata->tamano_bloque);
                break;
            case -1:
                opc=-1;
                break;
        }
    }while(opc!=-1);
}
bool DataBase::is_file(string file){ //Existe o no existe el archivo
    FILE * archivo;
    if (archivo = fopen(file.c_str(), "r")){ //r = read
        fclose(archivo);
        return true;
    }else{
        return false;
    }
}
void DataBase::eliminar(string ruta){
    string comando; //del
    if (is_file(ruta)){
        comando = "del " + ruta;
        system(comando.c_str());
        cout<<"Se elimino existosamente"<<endl;
    }else{
        cerr<< "La base datos no Existe!"<<endl;
    }
}
void DataBase::EliminarDb(){
    string n;
    cout << "Nombre de la Base de datos a Eliminar: ";
    cin>>n;
    string ruta =n+".dat";
    eliminar(ruta);
}
