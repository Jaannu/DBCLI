//
// Created by ItsJaan on 08/25/2019.
//

#include <cstring>
#include <fstream>
#include <sstream>
#include "Table.h"

using  namespace std;

void Table::CrearTabla(string dbname , int tamano_bloque) {
    NOMBRE:
    cout<<"Ingrese Nombre Tabla: ";
    cin>>name;


    if(Buscar(name , dbname , tamano_bloque)== 0){
        cout<<"Tabla Existente\n";
        goto NOMBRE;
    }
    cout<<"Ingrese Cantidad Columnas: ";
    cin>>n_column;
    int cont=n_column;
    fstream base(dbname+".dat" , ios::in|ios::out|ios::binary);
    if(!base){
        cout<<"Error\n";
        return;
    }

    DBMetadata *metadata = new DBMetadata();
    base.seekg(0 , ios::beg);
    base.read(reinterpret_cast<char*>(metadata), sizeof(DBMetadata));

    if(tamano_bloque==512){
        DataBlock512 bloq;
        base.read(reinterpret_cast<char *>(&bloq), 512 );
        cout<<"DATA ACTUAL: "<<bloq.data<<endl;
        string tmp , tmp2 , tmp3;
        tmp=bloq.data;
        bool tablaE=false;

            if(tmp.at(0)=='N' || tmp.at(0)=='T' ) {
                string col;
                stringstream ss;
                ss<<n_column;
                col=ss.str();
                tmp.at(0) = 'T';
                tmp = tmp + name + "," + col + ",";
                strcpy(bloq.data, tmp.c_str());

                cout << "DATA Nueva: " << bloq.data << endl;

                base.seekp((sizeof(DBMetadata)));
                base.write(reinterpret_cast<char *>(&bloq), 512);


                TableColumns * columna = new TableColumns();
                while(cont!=0) {
                    bool encontrado = false;
                    int disp = (sizeof(DBMetadata));
                    strcpy(columna->nombreTabla, name);
                    cout << "Ingrese Nombre a Columna -" << cont << "-: ";
                    cin >> columna->nombre;
                    BACK:
                    cout << "Ingrese Tipo a Columna -" << cont << "-: ";
                    cin >> columna->tipo;

                    if (strcmp(columna->tipo, "Int") == 0) {
                        columna->longi = 4;
                    } else if (strcmp(columna->tipo, "Double") == 0) {
                        columna->longi = 8;
                    } else if (strcmp(columna->tipo, "Varchar") == 0) {
                        cout << "Ingrese Longitud de Varchar(2000 Maximo):";
                        cin >> columna->longi;
                    } else goto BACK;
                    while (!base.eof() && encontrado != true) {
                        DataBlock512 bloq;
                        base.seekg(disp);
                        base.read(reinterpret_cast<char *>(&bloq), 512);
                        cout << "DATA ACTUAL: " << bloq.data << endl;
                        tmp2 = bloq.data;
                        string aux1 , aux2;
                        stringstream l1 ,l2;
                        if (tmp2.at(0) == 'C' || tmp2.at(0) == 'N') {
                            encontrado = true;
                            tmp2.at(0) = 'C';
                            l1<<columna->longi;
                            aux1= l1.str();
                            tmp2 = tmp2 + columna->nombreTabla + "," + columna->nombre + "," + columna->tipo + ","+aux1 +";";
                            strcpy(bloq.data, tmp2.c_str());
                            cout << "DATA ACTUAL: " << bloq.data << endl;
                            if (cont == n_column){
                                DataBlock512 bd;
                                base.seekp((sizeof(DBMetadata)));
                                base.read(reinterpret_cast<char *>(&bd), 512 );
                                primeraColumna = disp;
                                l2<<primeraColumna;
                                aux2 = l2.str();
                                tmp3=bd.data;
                                tmp3=tmp3+aux2+";";
                                strcpy(bd.data , tmp3.c_str());
                                cout<<"Primera Columna Actualizada\n"<<bd.data <<endl;

                                base.seekp((sizeof(DBMetadata)));
                                base.write(reinterpret_cast<char *>(&bd), 512);
                            }
                            base.seekp(disp);
                            base.write(reinterpret_cast<char *>(&bloq), 512);

                        } else disp = disp + 512;
                    }
                    cont--;
                }
        }
    }else if (metadata->tamano_bloque == 1024) {
        DataBlock1024 bloq;
        base.read(reinterpret_cast<char *>(&bloq), 1024 );
        cout<<"DATA ACTUAL: "<<bloq.data<<endl;
        string tmp , tmp2 , tmp3;
        tmp=bloq.data;
        bool tablaE=false;

        if(tmp.at(0)=='N' || tmp.at(0)=='T' ) {
            string col;
            stringstream ss;
            ss<<n_column;
            col=ss.str();
            tmp.at(0) = 'T';
            tmp = tmp + name + "," + col + ",";
            strcpy(bloq.data, tmp.c_str());

            cout << "DATA Nueva: " << bloq.data << endl;

            base.seekp((sizeof(DBMetadata)));
            base.write(reinterpret_cast<char *>(&bloq), 1024);


            TableColumns * columna = new TableColumns();
            while(cont!=0) {
                bool encontrado = false;
                int disp = (sizeof(DBMetadata));
                strcpy(columna->nombreTabla, name);
                cout << "Ingrese Nombre a Columna -" << cont << "-: ";
                cin >> columna->nombre;
                BACK1:
                cout << "Ingrese Tipo a Columna -" << cont << "-: ";
                cin >> columna->tipo;

                if (strcmp(columna->tipo, "Int") == 0) {
                    columna->longi = 4;
                } else if (strcmp(columna->tipo, "Double") == 0) {
                    columna->longi = 8;
                } else if (strcmp(columna->tipo, "Varchar") == 0) {
                    cout << "Ingrese Longitud de Varchar(2000 Maximo):";
                    cin >> columna->longi;
                } else goto BACK1;
                while (!base.eof() && encontrado != true) {
                    DataBlock1024 bloq;
                    base.seekg(disp);
                    base.read(reinterpret_cast<char *>(&bloq), 1024);
                    cout << "DATA ACTUAL: " << bloq.data << endl;
                    tmp2 = bloq.data;
                    string aux1 , aux2;
                    stringstream l1 ,l2;
                    if (tmp2.at(0) == 'C' || tmp2.at(0) == 'N') {
                        encontrado = true;
                        tmp2.at(0) = 'C';
                        l1<<columna->longi;
                        aux1= l1.str();
                        tmp2 = tmp2 + columna->nombreTabla + "," + columna->nombre + "," + columna->tipo + ","+aux1 +";";
                        strcpy(bloq.data, tmp2.c_str());
                        cout << "DATA ACTUAL: " << bloq.data << endl;
                        if (cont == n_column){
                            DataBlock1024 bd;
                            base.seekp((sizeof(DBMetadata)));
                            base.read(reinterpret_cast<char *>(&bd), 1024 );
                            primeraColumna = disp;
                            l2<<primeraColumna;
                            aux2 = l2.str();
                            tmp3=bd.data;
                            tmp3=tmp3+aux2+";";
                            strcpy(bd.data , tmp3.c_str());
                            cout<<"Primera Columna Actualizada\n"<<bd.data <<endl;

                            base.seekp((sizeof(DBMetadata)));
                            base.write(reinterpret_cast<char *>(&bd), 1024);
                        }
                        base.seekp(disp);
                        base.write(reinterpret_cast<char *>(&bloq), 1024);

                    } else disp = disp + 1024;
                }
                cont--;
            }
        }
    } else if (metadata->tamano_bloque == 2048) {
        DataBlock2048 bloq;
        base.read(reinterpret_cast<char *>(&bloq),2048 );
        cout<<"DATA ACTUAL: "<<bloq.data<<endl;
        string tmp , tmp2 , tmp3;
        tmp=bloq.data;
        bool tablaE=false;

        if(tmp.at(0)=='N' || tmp.at(0)=='T' ) {
            string col;
            stringstream ss;
            ss<<n_column;
            col=ss.str();
            tmp.at(0) = 'T';
            tmp = tmp + name + "," + col + ",";
            strcpy(bloq.data, tmp.c_str());

            cout << "DATA Nueva: " << bloq.data << endl;

            base.seekp((sizeof(DBMetadata)));
            base.write(reinterpret_cast<char *>(&bloq), 2048);


            TableColumns * columna = new TableColumns();
            while(cont!=0) {
                bool encontrado = false;
                int disp = (sizeof(DBMetadata));
                strcpy(columna->nombreTabla, name);
                cout << "Ingrese Nombre a Columna -" << cont << "-: ";
                cin >> columna->nombre;
                BACK2:
                cout << "Ingrese Tipo a Columna -" << cont << "-: ";
                cin >> columna->tipo;

                if (strcmp(columna->tipo, "Int") == 0) {
                    columna->longi = 4;
                } else if (strcmp(columna->tipo, "Double") == 0) {
                    columna->longi = 8;
                } else if (strcmp(columna->tipo, "Varchar") == 0) {
                    cout << "Ingrese Longitud de Varchar(2000 Maximo):";
                    cin >> columna->longi;
                } else goto BACK2;
                while (!base.eof() && encontrado != true) {
                    DataBlock2048 bloq;
                    base.seekg(disp);
                    base.read(reinterpret_cast<char *>(&bloq), 2048);
                    cout << "DATA ACTUAL: " << bloq.data << endl;
                    tmp2 = bloq.data;
                    string aux1 , aux2;
                    stringstream l1 ,l2;
                    if (tmp2.at(0) == 'C' || tmp2.at(0) == 'N') {
                        encontrado = true;
                        tmp2.at(0) = 'C';
                        l1<<columna->longi;
                        aux1= l1.str();
                        tmp2 = tmp2 + columna->nombreTabla + "," + columna->nombre + "," + columna->tipo + ","+aux1 +";";
                        strcpy(bloq.data, tmp2.c_str());
                        cout << "DATA ACTUAL: " << bloq.data << endl;
                        if (cont == n_column){
                            DataBlock2048 bd;
                            base.seekp((sizeof(DBMetadata)));
                            base.read(reinterpret_cast<char *>(&bd), 2048 );
                            primeraColumna = disp;
                            l2<<primeraColumna;
                            aux2 = l2.str();
                            tmp3=bd.data;
                            tmp3=tmp3+aux2+";";
                            strcpy(bd.data , tmp3.c_str());
                            cout<<"Primera Columna Actualizada\n"<<bd.data <<endl;

                            base.seekp((sizeof(DBMetadata)));
                            base.write(reinterpret_cast<char *>(&bd), 2048);
                        }
                        base.seekp(disp);
                        base.write(reinterpret_cast<char *>(&bloq), 2048);

                    } else disp = disp + 2048;
                }
                cont--;
            }
        }
    } else if (metadata->tamano_bloque == 3072) {
        DataBlock3072 bloq;
        base.read(reinterpret_cast<char *>(&bloq),3072 );
        cout<<"DATA ACTUAL: "<<bloq.data<<endl;
        string tmp , tmp2 , tmp3;
        tmp=bloq.data;
        bool tablaE=false;

        if(tmp.at(0)=='N' || tmp.at(0)=='T' ) {
            string col;
            stringstream ss;
            ss<<n_column;
            col=ss.str();
            tmp.at(0) = 'T';
            tmp = tmp + name + "," + col + ",";
            strcpy(bloq.data, tmp.c_str());

            cout << "DATA Nueva: " << bloq.data << endl;

            base.seekp((sizeof(DBMetadata)));
            base.write(reinterpret_cast<char *>(&bloq), 3072);


            TableColumns * columna = new TableColumns();
            while(cont!=0) {
                bool encontrado = false;
                int disp = (sizeof(DBMetadata));
                strcpy(columna->nombreTabla, name);
                cout << "Ingrese Nombre a Columna -" << cont << "-: ";
                cin >> columna->nombre;
                BACK3:
                cout << "Ingrese Tipo a Columna -" << cont << "-: ";
                cin >> columna->tipo;

                if (strcmp(columna->tipo, "Int") == 0) {
                    columna->longi = 4;
                } else if (strcmp(columna->tipo, "Double") == 0) {
                    columna->longi = 8;
                } else if (strcmp(columna->tipo, "Varchar") == 0) {
                    cout << "Ingrese Longitud de Varchar(2000 Maximo):";
                    cin >> columna->longi;
                } else goto BACK3;
                while (!base.eof() && encontrado != true) {
                    DataBlock3072 bloq;
                    base.seekg(disp);
                    base.read(reinterpret_cast<char *>(&bloq), 3072);
                    cout << "DATA ACTUAL: " << bloq.data << endl;
                    tmp2 = bloq.data;
                    string aux1 , aux2;
                    stringstream l1 ,l2;
                    if (tmp2.at(0) == 'C' || tmp2.at(0) == 'N') {
                        encontrado = true;
                        tmp2.at(0) = 'C';
                        l1<<columna->longi;
                        aux1= l1.str();
                        tmp2 = tmp2 + columna->nombreTabla + "," + columna->nombre + "," + columna->tipo + ","+aux1 +";";
                        strcpy(bloq.data, tmp2.c_str());
                        cout << "DATA ACTUAL: " << bloq.data << endl;
                        if (cont == n_column){
                            DataBlock3072 bd;
                            base.seekp((sizeof(DBMetadata)));
                            base.read(reinterpret_cast<char *>(&bd), 3072 );
                            primeraColumna = disp;
                            l2<<primeraColumna;
                            aux2 = l2.str();
                            tmp3=bd.data;
                            tmp3=tmp3+aux2+";";
                            strcpy(bd.data , tmp3.c_str());
                            cout<<"Primera Columna Actualizada\n"<<bd.data <<endl;

                            base.seekp((sizeof(DBMetadata)));
                            base.write(reinterpret_cast<char *>(&bd), 3072);
                        }
                        base.seekp(disp);
                        base.write(reinterpret_cast<char *>(&bloq), 3072);

                    } else disp = disp + 3072;
                }
                cont--;
            }
        }
    } else if (tamano_bloque == 4096) {
        DataBlock4096 bloq;
        base.read(reinterpret_cast<char *>(&bloq),4096 );
        cout<<"DATA ACTUAL: "<<bloq.data<<endl;
        string tmp , tmp2 , tmp3;
        tmp=bloq.data;
        bool tablaE=false;

        if(tmp.at(0)=='N' || tmp.at(0)=='T' ) {
            string col;
            stringstream ss;
            ss<<n_column;
            col=ss.str();
            tmp.at(0) = 'T';
            tmp = tmp + name + "," + col + ",";
            strcpy(bloq.data, tmp.c_str());

            cout << "DATA Nueva: " << bloq.data << endl;

            base.seekp((sizeof(DBMetadata)));
            base.write(reinterpret_cast<char *>(&bloq), 4096);


            TableColumns * columna = new TableColumns();
            while(cont!=0) {
                bool encontrado = false;
                int disp = (sizeof(DBMetadata));
                strcpy(columna->nombreTabla, name);
                cout << "Ingrese Nombre a Columna -" << cont << "-: ";
                cin >> columna->nombre;
                BACK4:
                cout << "Ingrese Tipo a Columna -" << cont << "-: ";
                cin >> columna->tipo;

                if (strcmp(columna->tipo, "Int") == 0) {
                    columna->longi = 4;
                } else if (strcmp(columna->tipo, "Double") == 0) {
                    columna->longi = 8;
                } else if (strcmp(columna->tipo, "Varchar") == 0) {
                    cout << "Ingrese Longitud de Varchar(2000 Maximo):";
                    cin >> columna->longi;
                } else goto BACK4;
                while (!base.eof() && encontrado != true) {
                    DataBlock4096 bloq;
                    base.seekg(disp);
                    base.read(reinterpret_cast<char *>(&bloq), 4096);
                    cout << "DATA ACTUAL: " << bloq.data << endl;
                    tmp2 = bloq.data;
                    string aux1 , aux2;
                    stringstream l1 ,l2;
                    if (tmp2.at(0) == 'C' || tmp2.at(0) == 'N') {
                        encontrado = true;
                        tmp2.at(0) = 'C';
                        l1<<columna->longi;
                        aux1= l1.str();
                        tmp2 = tmp2 + columna->nombreTabla + "," + columna->nombre + "," + columna->tipo + ","+aux1 +";";
                        strcpy(bloq.data, tmp2.c_str());
                        cout << "DATA ACTUAL: " << bloq.data << endl;
                        if (cont == n_column){
                            DataBlock4096 bd;
                            base.seekp((sizeof(DBMetadata)));
                            base.read(reinterpret_cast<char *>(&bd), 4096 );
                            primeraColumna = disp;
                            l2<<primeraColumna;
                            aux2 = l2.str();
                            tmp3=bd.data;
                            tmp3=tmp3+aux2+";";
                            strcpy(bd.data , tmp3.c_str());
                            cout<<"Primera Columna Actualizada\n"<<bd.data <<endl;

                            base.seekp((sizeof(DBMetadata)));
                            base.write(reinterpret_cast<char *>(&bd), 4096);
                        }
                        base.seekp(disp);
                        base.write(reinterpret_cast<char *>(&bloq), 4096);

                    } else disp = disp + 4096;
                }
                cont--;
            }
        }
    }else{
        cout<<"Error al Identificar Bloques\n";
        return;
    }
    cout << "Tabla Creada Correctamente\n\n";
}
int Table ::Buscar(string nom, string dbname , int n_col) {//0 se encontro , 1 no se encontro
    fstream base(dbname + ".dat", ios::in | ios::binary);
    if (!base) {
        puts("Error en base\n");
        return 0;
    }
    base.seekg((sizeof(DBMetadata)));

    if (n_col == 512) {
        DataBlock512 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 512);
        int i=1;
        WHILE:
        while (i < 512  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE;
        }
    }else if (n_col== 1024){
        DataBlock1024 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 1024);
        int i=1;
        WHILE1:
        while (i < 1024  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE1;
        }
    }else if(n_col == 2048){
        DataBlock2048 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 2048);
        int i=1;
        WHILE2:
        while (i < 2048  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE2;
        }
    }else if (n_col == 3072){
        DataBlock3072 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 3072);
        int i=1;
        WHILE3:
        while (i < 3072 && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE3;
        }
    }else{
        DataBlock4096 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 4096);
        int i=1;
        WHILE4:
        while (i < 4096  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE4;
        }
    }
}
int Table::BuscarAux(string nom , string dbname , int n_col  , int pos){
    fstream base(dbname + ".dat", ios::in | ios::binary);
    if (!base) {
        puts("Error en base\n");
        return 0;
    }
    base.seekg(pos);

    if (n_col == 512) {
        DataBlock512 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 512);
        int i=1;
        WHILE:
        while (i < 512  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE;
        }
    }else if(n_col==1024){
        DataBlock512 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 1024);
        int i=1;
        WHILE1:
        while (i < 1024  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE1;
        }
    }else if(n_col==2048){
        DataBlock2048 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 2048);
        int i=1;
        WHILE2:
        while (i < 2048  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE2;
        }
    }else if(n_col==3072){
        DataBlock3072 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 3072);
        int i=1;
        WHILE3:
        while (i < 3072  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto WHILE3;
        }
    }else if(n_col==4096){
        DataBlock4096 bloq;
        string tmp4;
        base.read(reinterpret_cast<char *>(&bloq), 4096);
        int i=1;
        WHILE4:
        while (i < 4096  && bloq.data[i]!=',' && bloq.data[i]!=';') {
            tmp4 = tmp4 + bloq.data[i];
            i++;
        }
        i++;
        if (nom == tmp4){
            cont=i;
            return 0;
        }else if(tmp4=="")  {
            return 1;
        }else {
            tmp4 = "";
            goto4 WHILE;
        }
    }
    return 0;
}
void Table:: ElimTabla(string dbname , int tamano_bloque){
    INICIO:
    string tmp1 , tmp2;
    string nombre;
    cout<<"Ingrese nombre Table:";
    cin>>nombre;

    fstream base(dbname + ".dat", ios::in |ios::out | ios::binary);
    if (!base) {
        puts("Error en base\n");
        return;
    }


    if(Buscar(nombre, dbname , tamano_bloque)!= 0){
        puts("No se encontro Tabla");
        goto INICIO;
    }else{
        cout<<"CONT: "<<cont<<endl;
        cout<<"MEDATADA: "<< sizeof(DBMetadata)<<endl;
        int y= cont - strlen(nombre.c_str()) ;
        int cont2=2;
        string aux;
        int i = sizeof(DBMetadata);
        y=y-1;
        cout<<"I: "<<i<<endl;
        ///**********************************************************BLOQUE 512
        if(tamano_bloque==512) {
            DataBlock512 bloq;
            base.seekg(i);
            base.read(reinterpret_cast<char*> (&bloq) , 512);
            tmp1=bloq.data;
            cout<<"DATA ACTUAL:\n"<<tmp1<<endl;
            while(tmp1.at(y)!=';'){
                if(tmp1.at(y)==','){
                    cont2--;
                }
                if(cont2==1){
                    tmp2=tmp2+ tmp1.at(y+1);
                }
                if(cont2==0){
                    aux=aux+tmp1.at(y);
                }
                tmp1.at(y)=' ';
                y++;
            }
            cout<<"AUX:"<<aux<<endl;
            cout<<"DATA NUEVA:\n" <<tmp1<<endl;
            strcpy(bloq.data , tmp1.c_str());
            base.seekp(sizeof(DBMetadata));
            base.write(reinterpret_cast<char*>(&bloq), 512);
            aux.at(0)='0';
            int pos =atoi(aux.c_str());
            cout<<"\nPOSICION COL: "<<pos<<endl;


            if(BuscarAux(nombre , dbname , 512 , pos)!=0){
                cout<<"No hay Columnas En Tabla\n";
                return;
            }else {
                int cont3=0;
                int x= cont - (strlen(nombre.c_str())+1);
                int cols=atoi(tmp2.c_str());
                base.seekg(pos);
                base.read(reinterpret_cast<char *>(&bloq), 512);
                string tmp3 = bloq.data;

                cout<<"DATA ACTUAL:\n"<<tmp3<<endl;
                cout<<"\nN COl: "<<cols<<endl;
                cout<<"\nPOS COL: "<<cont<<endl;
                while(cont3!=cols){
                    tmp3.at(x)=' ';
                    x++;
                    if(tmp3.at(x)==';'){
                        cont3++;
                    }
                }
                cout<<"DATA NUEVA:\n"<<tmp3<<endl;
                strcpy(bloq.data , tmp3.c_str());
                base.seekp(pos);
                base.write(reinterpret_cast<char*>(&bloq), 512);
            }
            EliminarRegistro(dbname , tamano_bloque);
            cout<<"Se elimino La tabla\n";
            ///**********************************************************BLOQUE 512
        }else if(tamano_bloque==1024) {
            DataBlock1024 bloq;
            base.seekg(i);
            base.read(reinterpret_cast<char*> (&bloq) , 1024);
            tmp1=bloq.data;
            cout<<"DATA ACTUAL:\n"<<tmp1<<endl;
            cout<<"Y : "<<y<<endl;
            while(tmp1.at(y)!=';'){
                if(tmp1.at(y)==','){
                    cont2--;
                }
                if(cont2==0){
                    aux=aux+tmp1.at(y);
                }
                tmp1.at(y)=' ';
                y++;
            }
            cout<<"AUX:"<<aux<<endl;
            cout<<"DATA NUEVA:\n" <<tmp1<<endl;
            strcpy(bloq.data , tmp1.c_str());
            base.seekp(sizeof(DBMetadata));
            base.write(reinterpret_cast<char*>(&bloq), 1024);
            aux.at(0)='0';
            int pos =atoi(aux.c_str());
            cout<<"\nPOSICION COL: "<<pos<<endl;


            if(BuscarAux(nombre , dbname , 1024 , pos)!=0){
                cout<<"No hay Columnas En Tabla\n";
                return;
            }else {
                int cont3=0;
                int x= cont - (strlen(nombre.c_str())+1);
                int cols=atoi(tmp2.c_str());
                base.seekg(pos);
                base.read(reinterpret_cast<char *>(&bloq), 1024);
                string tmp3 = bloq.data;

                cout<<"DATA ACTUAL:\n"<<tmp3<<endl;
                cout<<"\nN COl: "<<cols<<endl;
                cout<<"\nPOS COL: "<<cont<<endl;
                while(cont3!=cols){
                    tmp3.at(x)=' ';
                    x++;
                    if(tmp3.at(x)==';'){
                        cont3++;
                    }
                }
                cout<<"DATA NUEVA:\n"<<tmp3<<endl;
                strcpy(bloq.data , tmp3.c_str());
                base.seekp(pos);
                base.write(reinterpret_cast<char*>(&bloq), 1024);
            }
            EliminarRegistro(dbname , tamano_bloque);
            cout<<"Se elimino La tabla\n";
            ///**********************************************************BLOQUE 512
        }else if(tamano_bloque==2048) {
            DataBlock2048 bloq;
            base.seekg(i);
            base.read(reinterpret_cast<char*> (&bloq) , 2048);
            tmp1=bloq.data;
            cout<<"DATA ACTUAL:\n"<<tmp1<<endl;
            while(tmp1.at(y)!=';'){
                if(tmp1.at(y)==','){
                    cont2--;
                }
                if(cont2==0){
                    aux=aux+tmp1.at(y);
                }
                tmp1.at(y)=' ';
                y++;
            }
            cout<<"AUX:"<<aux<<endl;
            cout<<"DATA NUEVA:\n" <<tmp1<<endl;
            strcpy(bloq.data , tmp1.c_str());
            base.seekp(sizeof(DBMetadata));
            base.write(reinterpret_cast<char*>(&bloq), 2048);
            aux.at(0)='0';
            int pos =atoi(aux.c_str());
            cout<<"\nPOSICION COL: "<<pos<<endl;


            if(BuscarAux(nombre , dbname , 2048 , pos)!=0){
                cout<<"No hay Columnas En Tabla\n";
                return;
            }else {
                int cont3=0;
                int x= cont - (strlen(nombre.c_str())+1);
                int cols=atoi(tmp2.c_str());
                base.seekg(pos);
                base.read(reinterpret_cast<char *>(&bloq), 2048);
                string tmp3 = bloq.data;

                cout<<"DATA ACTUAL:\n"<<tmp3<<endl;
                cout<<"\nN COl: "<<cols<<endl;
                cout<<"\nPOS COL: "<<cont<<endl;
                while(cont3!=cols){
                    tmp3.at(x)=' ';
                    x++;
                    if(tmp3.at(x)==';'){
                        cont3++;
                    }
                }
                cout<<"DATA NUEVA:\n"<<tmp3<<endl;
                strcpy(bloq.data , tmp3.c_str());
                base.seekp(pos);
                base.write(reinterpret_cast<char*>(&bloq), 2048);
            }
            EliminarRegistro(dbname , tamano_bloque);
            cout<<"Se elimino La tabla\n";
///**********************************************************BLOQUE 512
        }else if(tamano_bloque==3072){
            DataBlock3072 bloq;
            base.seekg(i);
            base.read(reinterpret_cast<char*> (&bloq) , 3072);
            tmp1=bloq.data;
            cout<<"DATA ACTUAL:\n"<<tmp1<<endl;
            while(tmp1.at(y)!=';'){
                if(tmp1.at(y)==','){
                    cont2--;
                }
                if(cont2==0){
                    aux=aux+tmp1.at(y);
                }
                tmp1.at(y)=' ';
                y++;
            }
            cout<<"AUX:"<<aux<<endl;
            cout<<"DATA NUEVA:\n" <<tmp1<<endl;
            strcpy(bloq.data , tmp1.c_str());
            base.seekp(sizeof(DBMetadata));
            base.write(reinterpret_cast<char*>(&bloq), 3072);
            aux.at(0)='0';
            int pos =atoi(aux.c_str());
            cout<<"\nPOSICION COL: "<<pos<<endl;


            if(BuscarAux(nombre , dbname , 3072 , pos)!=0){
                cout<<"No hay Columnas En Tabla\n";
                return;
            }else {
                int cont3=0;
                int x= cont - (strlen(nombre.c_str())+1);
                int cols=atoi(tmp2.c_str());
                base.seekg(pos);
                base.read(reinterpret_cast<char *>(&bloq), 3072);
                string tmp3 = bloq.data;

                cout<<"DATA ACTUAL:\n"<<tmp3<<endl;
                cout<<"\nN COl: "<<cols<<endl;
                cout<<"\nPOS COL: "<<cont<<endl;
                while(cont3!=cols){
                    tmp3.at(x)=' ';
                    x++;
                    if(tmp3.at(x)==';'){
                        cont3++;
                    }
                }
                cout<<"DATA NUEVA:\n"<<tmp3<<endl;
                strcpy(bloq.data , tmp3.c_str());
                base.seekp(pos);
                base.write(reinterpret_cast<char*>(&bloq), 3072);
            }
            EliminarRegistro(dbname , tamano_bloque);
            cout<<"Se elimino La tabla\n";
            ///**********************************************************BLOQUE 512
        }else if(tamano_bloque==4096) {
            DataBlock4096 bloq;
            base.seekg(i);
            base.read(reinterpret_cast<char*> (&bloq) , 4096);
            tmp1=bloq.data;
            cout<<"DATA ACTUAL:\n"<<tmp1<<endl;
            while(tmp1.at(y)!=';'){
                if(tmp1.at(y)==','){
                    cont2--;
                }
                if(cont2==0){
                    aux=aux+tmp1.at(y);
                }
                tmp1.at(y)=' ';
                y++;
            }
            cout<<"AUX:"<<aux<<endl;
            cout<<"DATA NUEVA:\n" <<tmp1<<endl;
            strcpy(bloq.data , tmp1.c_str());
            base.seekp(sizeof(DBMetadata));
            base.write(reinterpret_cast<char*>(&bloq), 4096);
            aux.at(0)='0';
            int pos =atoi(aux.c_str());
            cout<<"\nPOSICION COL: "<<pos<<endl;


            if(BuscarAux(nombre , dbname , 4096 , pos)!=0){
                cout<<"No hay Columnas En Tabla\n";
                return;
            }else {
                int cont3=0;
                int x= cont - (strlen(nombre.c_str())+1);
                int cols=atoi(tmp2.c_str());
                base.seekg(pos);
                base.read(reinterpret_cast<char *>(&bloq), 4096);
                string tmp3 = bloq.data;

                cout<<"DATA ACTUAL:\n"<<tmp3<<endl;
                cout<<"\nN COl: "<<cols<<endl;
                cout<<"\nPOS COL: "<<cont<<endl;
                while(cont3!=cols){
                    tmp3.at(x)=' ';
                    x++;
                    if(tmp3.at(x)==';'){
                        cont3++;
                    }
                }
                cout<<"DATA NUEVA:\n"<<tmp3<<endl;
                strcpy(bloq.data , tmp3.c_str());
                base.seekp(pos);
                base.write(reinterpret_cast<char*>(&bloq), 4096);
            }
            EliminarRegistro(dbname , tamano_bloque);
            cout<<"Se elimino La tabla\n";
        }
    }
}
void Table::Insert(string dbname , int tamano_bloque) {
    string nombreTabla, auxColumnas, auxGuardar;
    int ncol = 0, pos_col = 0, cont1 = 0;
    cout << "Nombre Tabla:";
    cin >> nombreTabla;

    if (Buscar(nombreTabla, dbname, tamano_bloque) != 0) {
        cout << "Tabla No existe\n";
    } else {
        fstream base(dbname + ".dat", ios::in | ios::out | ios::binary);
        base.seekg((sizeof(DBMetadata)));
        int x = (cont - strlen(nombreTabla.c_str()) - 1);
        cout << "X: " << x << endl;
        if (tamano_bloque == 512) {
            DataBlock512 bloq;
            base.read(reinterpret_cast<char * >(&bloq), 512);
            string auxN_col, posCol;

            while (bloq.data[x] != ';') {
                if (bloq.data[x] == ',')
                    cont1++;
                if (cont1 == 1) {
                    auxN_col = auxN_col + bloq.data[x];
                }

                if (cont1 == 2) {
                    posCol = posCol + bloq.data[x];
                }
                x++;
            }
            posCol.at(0) = '0';
            auxN_col.at(0) = '0';

            ncol = atoi(auxN_col.c_str());
            pos_col = atoi(posCol.c_str());
            cout << "\nncol: " << ncol << endl;
            cout << "pos_col: " << pos_col << endl;

            int contWhile = 0;
            int y = 0;
            base.seekg(pos_col);
            base.read(reinterpret_cast<char *>(&bloq), 512);
            if (BuscarAux(nombreTabla, dbname, 512 , pos_col) != 0) {
                puts("No se encontro Columnas para tabla\n");
                return;
            } else
                y = cont - (strlen(nombreTabla.c_str())+1);

            cout << " Posicion Columna: " << y << endl;
            while (contWhile != ncol) {
                auxColumnas = auxColumnas + bloq.data[y];
                if (bloq.data[y] == ';')
                    contWhile++;
                y++;
            }
            cout << "DATA ACTUAL:" << auxColumnas << endl;
            contWhile = 0;
            int contAux = 0;
            string tmp;
            y = (strlen(nombreTabla.c_str() + 1));
            while (contWhile != ncol) {
                if (auxColumnas.at(y) == ',') {
                    contAux++;
                }
                if (contAux == 2) {
                    tmp = tmp + auxColumnas.at(y);
                }
                if (auxColumnas.at(y) == ';') {
                    contWhile++;
                    tmp.at(0) = ' ';
                    cout << "tmp:" << tmp << endl;
                    int x1 = 0;
                    double x2 = 0;
                    string x3 = "";


                    if (tmp == " Int") {
                        stringstream ax;
                        cout << "Ingrese Valor" << tmp << ": ";
                        cin >> x1;
                        ax << x1;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Double") {
                        ostringstream ax;
                        cout << "Ingrese valor" << tmp << ": ";
                        cin >> x2;
                        ax << x2;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Varchar") {
                        cout << "Ingrese" << tmp << ": ";
                        cin >> x3;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + x3;
                        else
                            auxGuardar = auxGuardar + x3 + ",";

                    } else cout << "ERROR" << endl;

                    contAux = 0;
                    tmp = "";
                }
                y++;
            }
            auxGuardar = auxGuardar + ";";
            cout << "Registro a Guardar:\n" << auxGuardar << endl;
            bool encontrado=false;
            int contAux1=sizeof(DBMetadata);
            WHILE:
            while (!base.eof() && encontrado != true) {
                DataBlock512 bloq;
                base.seekg(contAux1);
                base.read(reinterpret_cast<char *>(&bloq), 512);
                cout << "DATA ACTUAL: " << bloq.data << endl;
                tmp = bloq.data;
                if (tmp.at(0) == 'N' || tmp.at(0) == 'D') {
                    if(tmp.at(0)=='N'){
                        encontrado=true;
                        tmp = "D"+nombreTabla + ";";
                        cout<<"tmp: "<<tmp<<endl;
                    }
                    else if(tmp.at(0)=='D'){
                        if (BuscarAux(nombreTabla, dbname, 512, contAux1) == 0) {
                            encontrado=true;
                        } else {
                            contAux1 = contAux1 + 512;
                            goto WHILE;
                        }
                    }
                    tmp=tmp+auxGuardar;
                    cout<<"TMP Nuevo:"<<tmp<<endl;
                    strcpy(bloq.data , tmp.c_str());
                    base.seekp(contAux1);
                    base.write(reinterpret_cast<char *>(&bloq), 512);
                }else {
                    contAux1=contAux1+512;
                }
                tmp="";
            }
        }else if(tamano_bloque==1024){
            DataBlock1024 bloq;
            base.read(reinterpret_cast<char * >(&bloq), 1024);
            string auxN_col, posCol;

            while (bloq.data[x] != ';') {
                if (bloq.data[x] == ',')
                    cont1++;
                if (cont1 == 1) {
                    auxN_col = auxN_col + bloq.data[x];
                }

                if (cont1 == 2) {
                    posCol = posCol + bloq.data[x];
                }
                x++;
            }
            posCol.at(0) = '0';
            auxN_col.at(0) = '0';

            ncol = atoi(auxN_col.c_str());
            pos_col = atoi(posCol.c_str());
            cout << "\nncol: " << ncol << endl;
            cout << "pos_col: " << pos_col << endl;

            int contWhile = 0;
            int y = 0;
            base.seekg(pos_col);
            base.read(reinterpret_cast<char *>(&bloq), 1024);
            if (BuscarAux(nombreTabla, dbname, 1024 , pos_col) != 0) {
                puts("No se encontro Columnas para tabla\n");
                return;
            } else
                y = cont - (strlen(nombreTabla.c_str())+1);
            cout << " Posicion Columna: " << y << endl;

            while (contWhile != ncol) {
                auxColumnas = auxColumnas + bloq.data[y];
                if (bloq.data[y] == ';')
                    contWhile++;
                y++;
            }
            cout << "DATA ACTUAL:" << auxColumnas << endl;
            contWhile = 0;
            int contAux = 0;
            string tmp;
            y = (strlen(nombreTabla.c_str() + 1));
            while (contWhile != ncol) {
                if (auxColumnas.at(y) == ',') {
                    contAux++;
                }
                if (contAux == 2) {
                    tmp = tmp + auxColumnas.at(y);
                }
                if (auxColumnas.at(y) == ';') {
                    contWhile++;
                    tmp.at(0) = ' ';
                    cout << "tmp:" << tmp << endl;
                    int x1 = 0;
                    double x2 = 0;
                    string x3 = "";


                    if (tmp == " Int") {
                        stringstream ax;
                        cout << "Ingrese Valor" << tmp << ": ";
                        cin >> x1;
                        ax << x1;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Double") {
                        ostringstream ax;
                        cout << "Ingrese valor" << tmp << ": ";
                        cin >> x2;
                        ax << x2;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Varchar") {
                        cout << "Ingrese" << tmp << ": ";
                        cin >> x3;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + x3;
                        else
                            auxGuardar = auxGuardar + x3 + ",";

                    } else cout << "ERROR" << endl;

                    contAux = 0;
                    tmp = "";
                }
                y++;
            }
            auxGuardar = auxGuardar + ";";
            cout << "Registro a Guardar:\n" << auxGuardar << endl;
            bool encontrado=false;
            int contAux1=sizeof(DBMetadata);
            WHILE1:
            while (!base.eof() && encontrado != true) {
                DataBlock1024 bloq;
                base.seekg(contAux1);
                base.read(reinterpret_cast<char *>(&bloq), 1024 );
                cout << "DATA ACTUAL: " << bloq.data << endl;
                tmp = bloq.data;
                if (tmp.at(0) == 'N' || tmp.at(0) == 'D') {
                    if(tmp.at(0)=='N'){
                        encontrado=true;
                        tmp = "D"+nombreTabla + ";";
                        cout<<"tmp: "<<tmp<<endl;
                    }
                    else if(tmp.at(0)=='D'){
                        if (BuscarAux(nombreTabla, dbname, 1024, contAux1) == 0) {
                            encontrado=true;
                        } else {
                            contAux1 = contAux1 + 1024;
                            goto WHILE1;
                        }
                    }
                    tmp=tmp+auxGuardar;
                    cout<<"TMP Nuevo:"<<tmp<<endl;
                    strcpy(bloq.data , tmp.c_str());
                    base.seekp(contAux1);
                    base.write(reinterpret_cast<char *>(&bloq), 1024);
                }else {
                    contAux1=contAux1+1024;
                }
            }
        }else if(tamano_bloque==2048){
            DataBlock2048 bloq;
            base.read(reinterpret_cast<char * >(&bloq), 2048);
            string auxN_col, posCol;

            while (bloq.data[x] != ';') {
                if (bloq.data[x] == ',')
                    cont1++;
                if (cont1 == 1) {
                    auxN_col = auxN_col + bloq.data[x];
                }

                if (cont1 == 2) {
                    posCol = posCol + bloq.data[x];
                }
                x++;
            }
            posCol.at(0) = '0';
            auxN_col.at(0) = '0';

            ncol = atoi(auxN_col.c_str());
            pos_col = atoi(posCol.c_str());
            cout << "\nncol: " << ncol << endl;
            cout << "pos_col: " << pos_col << endl;

            int contWhile = 0;
            int y = 0;
            base.seekg(pos_col);
            base.read(reinterpret_cast<char *>(&bloq), 2048);
            if (BuscarAux(nombreTabla, dbname, 2048 , pos_col) != 0) {
                puts("No se encontro Columnas para tabla\n");
                return;
            } else
                y = cont - (strlen(nombreTabla.c_str())+1);
            cout << " Posicion Columna: " << y << endl;

            while (contWhile != ncol) {
                auxColumnas = auxColumnas + bloq.data[y];
                if (bloq.data[y] == ';')
                    contWhile++;
                y++;
            }
            cout << "DATA ACTUAL:" << auxColumnas << endl;
            contWhile = 0;
            int contAux = 0;
            string tmp;
            y = (strlen(nombreTabla.c_str() + 1));
            while (contWhile != ncol) {
                if (auxColumnas.at(y) == ',') {
                    contAux++;
                }
                if (contAux == 2) {
                    tmp = tmp + auxColumnas.at(y);
                }
                if (auxColumnas.at(y) == ';') {
                    contWhile++;
                    tmp.at(0) = ' ';
                    cout << "tmp:" << tmp << endl;
                    int x1 = 0;
                    double x2 = 0;
                    string x3 = "";


                    if (tmp == " Int") {
                        stringstream ax;
                        cout << "Ingrese Valor" << tmp << ": ";
                        cin >> x1;
                        ax << x1;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Double") {
                        ostringstream ax;
                        cout << "Ingrese valor" << tmp << ": ";
                        cin >> x2;
                        ax << x2;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Varchar") {
                        cout << "Ingrese" << tmp << ": ";
                        cin >> x3;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + x3;
                        else
                            auxGuardar = auxGuardar + x3 + ",";

                    } else cout << "ERROR" << endl;

                    contAux = 0;
                    tmp = "";
                }
                y++;
            }
            auxGuardar = auxGuardar + ";";
            cout << "Registro a Guardar:\n" << auxGuardar << endl;
            bool encontrado=false;
            int contAux1=sizeof(DBMetadata);
            WHILE2:
            while (!base.eof() && encontrado != true) {
                DataBlock2048 bloq;
                base.seekg(contAux1);
                base.read(reinterpret_cast<char *>(&bloq), 2048);
                cout << "DATA ACTUAL: " << bloq.data << endl;
                tmp = bloq.data;
                if (tmp.at(0) == 'N' || tmp.at(0) == 'D') {
                    if(tmp.at(0)=='N'){
                        encontrado=true;
                        tmp = "D"+nombreTabla + ";";
                        cout<<"tmp: "<<tmp<<endl;
                    }
                    else if(tmp.at(0)=='D'){
                        if (BuscarAux(nombreTabla, dbname, 2048, contAux1) == 0) {
                            encontrado=true;
                        } else {
                            contAux1 = contAux1 + 2048;
                            goto WHILE2;
                        }
                    }
                    tmp=tmp+auxGuardar;
                    cout<<"TMP Nuevo:"<<tmp<<endl;
                    strcpy(bloq.data , tmp.c_str());
                    base.seekp(contAux1);
                    base.write(reinterpret_cast<char *>(&bloq), 2048);
                }else {
                    contAux1=contAux1+2048;
                }
            }
        }else if(tamano_bloque==3072){
            DataBlock3072 bloq;
            base.read(reinterpret_cast<char * >(&bloq), 3072);
            string auxN_col, posCol;

            while (bloq.data[x] != ';') {
                if (bloq.data[x] == ',')
                    cont1++;
                if (cont1 == 1) {
                    auxN_col = auxN_col + bloq.data[x];
                }

                if (cont1 == 2) {
                    posCol = posCol + bloq.data[x];
                }
                x++;
            }
            posCol.at(0) = '0';
            auxN_col.at(0) = '0';

            ncol = atoi(auxN_col.c_str());
            pos_col = atoi(posCol.c_str());
            cout << "\nncol: " << ncol << endl;
            cout << "pos_col: " << pos_col << endl;

            int contWhile = 0;
            int y = 0;
            base.seekg(pos_col);
            base.read(reinterpret_cast<char *>(&bloq), 3072);
            if (BuscarAux(nombreTabla, dbname, 3072 , pos_col) != 0) {
                puts("No se encontro Columnas para tabla\n");
                return;
            } else
                y = cont - (strlen(nombreTabla.c_str())+1);
            cout << " Posicion Columna: " << y << endl;

            while (contWhile != ncol) {
                auxColumnas = auxColumnas + bloq.data[y];
                if (bloq.data[y] == ';')
                    contWhile++;
                y++;
            }
            cout << "DATA ACTUAL:" << auxColumnas << endl;
            contWhile = 0;
            int contAux = 0;
            string tmp;
            y = (strlen(nombreTabla.c_str() + 1));
            while (contWhile != ncol) {
                if (auxColumnas.at(y) == ',') {
                    contAux++;
                }
                if (contAux == 2) {
                    tmp = tmp + auxColumnas.at(y);
                }
                if (auxColumnas.at(y) == ';') {
                    contWhile++;
                    tmp.at(0) = ' ';
                    cout << "tmp:" << tmp << endl;
                    int x1 = 0;
                    double x2 = 0;
                    string x3 = "";


                    if (tmp == " Int") {
                        stringstream ax;
                        cout << "Ingrese Valor" << tmp << ": ";
                        cin >> x1;
                        ax << x1;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Double") {
                        ostringstream ax;
                        cout << "Ingrese valor" << tmp << ": ";
                        cin >> x2;
                        ax << x2;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Varchar") {
                        cout << "Ingrese" << tmp << ": ";
                        cin >> x3;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + x3;
                        else
                            auxGuardar = auxGuardar + x3 + ",";

                    } else cout << "ERROR" << endl;

                    contAux = 0;
                    tmp = "";
                }
                y++;
            }
            auxGuardar = auxGuardar + ";";
            cout << "Registro a Guardar:\n" << auxGuardar << endl;
            bool encontrado=false;
            int contAux1=sizeof(DBMetadata);
            WHILE3:
            while (!base.eof() && encontrado != true) {
                DataBlock3072 bloq;
                base.seekg(contAux1);
                base.read(reinterpret_cast<char *>(&bloq), 3072);
                cout << "DATA ACTUAL: " << bloq.data << endl;
                tmp = bloq.data;
                if (tmp.at(0) == 'N' || tmp.at(0) == 'D') {
                    if(tmp.at(0)=='N'){
                        encontrado=true;
                        tmp = "D"+nombreTabla + ";";
                        cout<<"tmp: "<<tmp<<endl;
                    }
                    else if(tmp.at(0)=='D'){
                        if (BuscarAux(nombreTabla, dbname,3072, contAux1) == 0) {
                            encontrado=true;
                        } else {
                            contAux1 = contAux1 + 3072;
                            goto WHILE3;
                        }
                    }
                    tmp=tmp+auxGuardar;
                    cout<<"TMP Nuevo:"<<tmp<<endl;
                    strcpy(bloq.data , tmp.c_str());
                    base.seekp(contAux1);
                    base.write(reinterpret_cast<char *>(&bloq), 3072);
                }else {
                    contAux1=contAux1+3072;
                }
            }
        }else{
            DataBlock4096 bloq;
            base.read(reinterpret_cast<char * >(&bloq), 4096);
            string auxN_col, posCol;

            while (bloq.data[x] != ';') {
                if (bloq.data[x] == ',')
                    cont1++;
                if (cont1 == 1) {
                    auxN_col = auxN_col + bloq.data[x];
                }

                if (cont1 == 2) {
                    posCol = posCol + bloq.data[x];
                }
                x++;
            }
            posCol.at(0) = '0';
            auxN_col.at(0) = '0';

            ncol = atoi(auxN_col.c_str());
            pos_col = atoi(posCol.c_str());
            cout << "\nncol: " << ncol << endl;
            cout << "pos_col: " << pos_col << endl;

            int contWhile = 0;
            int y = 0;
            base.seekg(pos_col);
            base.read(reinterpret_cast<char *>(&bloq), 4096);
            if (BuscarAux(nombreTabla, dbname, 4096 , pos_col) != 0) {
                puts("No se encontro Columnas para tabla\n");
                return;
            } else
                y = cont - (strlen(nombreTabla.c_str())+1);
            cout << " Posicion Columna: " << y << endl;

            while (contWhile != ncol) {
                auxColumnas = auxColumnas + bloq.data[y];
                if (bloq.data[y] == ';')
                    contWhile++;
                y++;
            }
            cout << "DATA ACTUAL:" << auxColumnas << endl;
            contWhile = 0;
            int contAux = 0;
            string tmp;
            y = (strlen(nombreTabla.c_str() + 1));
            while (contWhile != ncol) {
                if (auxColumnas.at(y) == ',') {
                    contAux++;
                }
                if (contAux == 2) {
                    tmp = tmp + auxColumnas.at(y);
                }
                if (auxColumnas.at(y) == ';') {
                    contWhile++;
                    tmp.at(0) = ' ';
                    cout << "tmp:" << tmp << endl;
                    int x1 = 0;
                    double x2 = 0;
                    string x3 = "";


                    if (tmp == " Int") {
                        stringstream ax;
                        cout << "Ingrese Valor" << tmp << ": ";
                        cin >> x1;
                        ax << x1;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Double") {
                        ostringstream ax;
                        cout << "Ingrese valor" << tmp << ": ";
                        cin >> x2;
                        ax << x2;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + ax.str();
                        else
                            auxGuardar = auxGuardar + ax.str() + ",";
                    } else if (tmp == " Varchar") {
                        cout << "Ingrese" << tmp << ": ";
                        cin >> x3;
                        if (contWhile == ncol)
                            auxGuardar = auxGuardar + x3;
                        else
                            auxGuardar = auxGuardar + x3 + ",";

                    } else cout << "ERROR" << endl;

                    contAux = 0;
                    tmp = "";
                }
                y++;
            }
            auxGuardar = auxGuardar + ";";
            cout << "Registro a Guardar:\n" << auxGuardar << endl;
            bool encontrado=false;
            int contAux1=sizeof(DBMetadata);
            WHILE4:
            while (!base.eof() && encontrado != true) {
                DataBlock4096 bloq;
                base.seekg(contAux1);
                base.read(reinterpret_cast<char *>(&bloq),4096);
                cout << "DATA ACTUAL: " << bloq.data << endl;
                tmp = bloq.data;
                if (tmp.at(0) == 'N' || tmp.at(0) == 'D') {
                    if(tmp.at(0)=='N'){
                        encontrado=true;
                        tmp = "D"+nombreTabla + ";";
                        cout<<"tmp: "<<tmp<<endl;
                    }
                    else if(tmp.at(0)=='D'){
                        if (BuscarAux(nombreTabla, dbname,4096, contAux1) == 0) {
                            encontrado=true;
                        } else {
                            contAux1 = contAux1 + 4096;
                            goto WHILE4;
                        }
                    }
                    tmp=tmp+auxGuardar;
                    cout<<"TMP Nuevo:"<<tmp<<endl;
                    strcpy(bloq.data , tmp.c_str());
                    base.seekp(contAux1);
                    base.write(reinterpret_cast<char *>(&bloq), 4096);
                }else {
                    contAux1=contAux1+4096;
                }
            }
        }
    }
}
void Table::Select(string dbname, int tamano_bloque ) {
    string tmp, tmpCol;
    int ncol;
    string nombreTabla;
    cout<<"Ingrese Nombre Tabla:";
    cin>>nombreTabla;
    if(tamano_bloque==512) {
        if (Buscar(nombreTabla, dbname, 512) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock512 bloq;
            fstream base(dbname+".dat" , ios::in | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,512);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMP:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,512,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else{
                tmp="";
                cout<<"contAux: "<<contAux<<endl;
                base.seekg(contAux);
                base.read(reinterpret_cast<char*>(&bloq) , 512);
                pos = cont-(strlen(nombreTabla.c_str())+1);
                cout<<"Pos Actual:"<<pos<<endl;
                int conWhile=0 , conComas=0;
                while(conWhile!=ncol){
                    if(bloq.data[pos]==',') {
                        conComas++;
                    }
                    if(conComas==1) {
                        tmp= tmp + bloq.data[pos];
                    }
                    if(bloq.data[pos]==';') {
                        tmp=tmp+"\n";
                        conComas=0;
                        conWhile++;
                    }
                    pos++;
                }
                tmp.at(0)=' ';
                int valor;
                cout<<"COLUMNAS:\n\t"<<tmp<<endl;
                cout<<"Ingrese Numero Columna (0 TODO):";
                cin>>valor;
                valor = valor-1;
                bool encontrado=false;
                int auxPos=(sizeof(DBMetadata));
                Back:
                while (!base.eof() && encontrado != true) {
                    tmp ="";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char*>(&bloq), 512);
                    if(bloq.data[0]=='D'){
                        if(BuscarAux(nombreTabla, dbname, 512, auxPos)!=0){
                            cout<<"No Hay Registro En Tabla\n";
                            auxPos=auxPos+512;
                            goto Back;
                        }else{
                            encontrado=true;
                            int pos=cont;

                            if(valor==-1) {
                                while (bloq.data[pos] != ' ') {
                                    if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                    } else
                                        tmp = tmp + bloq.data[pos];
                                    pos++;
                                }
                            }else{
                                tmp="";
                                int contExtra=0;
                                while(bloq.data[pos]!=' ') {
                                    if (contExtra == valor) {
                                        tmp = tmp + bloq.data[pos];
                                    }
                                    if (bloq.data[pos] == ',') {
                                        contExtra++;
                                    }else if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                        contExtra=0;
                                    }
                                    pos++;
                                }
                            }
                        }
                    }else
                        auxPos = auxPos + 512 ;
                }
                cout<<"-----------------DATOS------------------\n";
                cout<<tmp;
            }
        }
    }else if (tamano_bloque==1024){
        if (Buscar(nombreTabla, dbname, 1024) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock1024 bloq;
            fstream base(dbname+".dat" , ios::in | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,1024);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMP:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,1024,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else{
                tmp="";
                cout<<"contAux: "<<contAux<<endl;
                base.seekg(contAux);
                base.read(reinterpret_cast<char*>(&bloq) , 1024);
                pos = cont-(strlen(nombreTabla.c_str())+1);
                cout<<"Pos Actual:"<<pos<<endl;
                int conWhile=0 , conComas=0;
                while(conWhile!=ncol){
                    if(bloq.data[pos]==',') {
                        conComas++;
                    }
                    if(conComas==1) {
                        tmp= tmp + bloq.data[pos];
                    }
                    if(bloq.data[pos]==';') {
                        tmp=tmp+"\n";
                        conComas=0;
                        conWhile++;
                    }
                    pos++;
                }
                tmp.at(0)=' ';
                int valor;
                cout<<"COLUMNAS:\n\t"<<tmp<<endl;
                cout<<"Ingrese Numero Columna (0 TODO):";
                cin>>valor;
                valor = valor-1;
                bool encontrado=false;
                int auxPos=(sizeof(DBMetadata));
                Back1:
                while (!base.eof() && encontrado != true) {
                    tmp ="";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char*>(&bloq), 1024);
                    if(bloq.data[0]=='D'){
                        if(BuscarAux(nombreTabla, dbname, 1024, auxPos)!=0){
                            cout<<"No Hay Registro En Tabla\n";
                            auxPos=auxPos+1024;
                            goto Back1;
                        }else{
                            encontrado=true;
                            int pos=cont;

                            if(valor==-1) {
                                while (bloq.data[pos] != ' ') {
                                    if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                    } else
                                        tmp = tmp + bloq.data[pos];
                                    pos++;
                                }
                            }else{
                                tmp="";
                                int contExtra=0;
                                while(bloq.data[pos]!=' ') {
                                    if (contExtra == valor) {
                                        tmp = tmp + bloq.data[pos];
                                    }
                                    if (bloq.data[pos] == ',') {
                                        contExtra++;
                                    }else if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                        contExtra=0;
                                    }
                                    pos++;
                                }
                            }
                        }
                    }else
                        auxPos = auxPos +1024;
                }
                cout<<"-----------------DATOS------------------\n";
                cout<<tmp;
            }
        }
    }else if  (tamano_bloque==2048){
        if (Buscar(nombreTabla, dbname, 2048) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock2048 bloq;
            fstream base(dbname+".dat" , ios::in | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,2048);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMP:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,2048,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else{
                tmp="";
                cout<<"contAux: "<<contAux<<endl;
                base.seekg(contAux);
                base.read(reinterpret_cast<char*>(&bloq) , 2048);
                pos = cont-(strlen(nombreTabla.c_str())+1);
                cout<<"Pos Actual:"<<pos<<endl;
                int conWhile=0 , conComas=0;
                while(conWhile!=ncol){
                    if(bloq.data[pos]==',') {
                        conComas++;
                    }
                    if(conComas==1) {
                        tmp= tmp + bloq.data[pos];
                    }
                    if(bloq.data[pos]==';') {
                        tmp=tmp+"\n";
                        conComas=0;
                        conWhile++;
                    }
                    pos++;
                }
                tmp.at(0)=' ';
                int valor;
                cout<<"COLUMNAS:\n\t"<<tmp<<endl;
                cout<<"Ingrese Numero Columna (0 TODO):";
                cin>>valor;
                valor = valor-1;
                bool encontrado=false;
                int auxPos=(sizeof(DBMetadata));
                Back2:
                while (!base.eof() && encontrado != true) {
                    tmp ="";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char*>(&bloq), 2048);
                    if(bloq.data[0]=='D'){
                        if(BuscarAux(nombreTabla, dbname, 2048, auxPos)!=0){
                            cout<<"No Hay Registro En Tabla\n";
                            auxPos=auxPos+2048;
                            goto Back2;
                        }else{
                            encontrado=true;
                            int pos=cont;

                            if(valor==-1) {
                                while (bloq.data[pos] != ' ') {
                                    if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                    } else
                                        tmp = tmp + bloq.data[pos];
                                    pos++;
                                }
                            }else{
                                tmp="";
                                int contExtra=0;
                                while(bloq.data[pos]!=' ') {
                                    if (contExtra == valor) {
                                        tmp = tmp + bloq.data[pos];
                                    }
                                    if (bloq.data[pos] == ',') {
                                        contExtra++;
                                    }else if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                        contExtra=0;
                                    }
                                    pos++;
                                }
                            }
                        }
                    }else
                        auxPos = auxPos + 2048;
                }
                cout<<"-----------------DATOS------------------\n";
                cout<<tmp;
            }
        }
    }else if(tamano_bloque==3072){
        if (Buscar(nombreTabla, dbname, 3072) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock3072 bloq;
            fstream base(dbname+".dat" , ios::in | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,3072);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMP:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,3072,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else{
                tmp="";
                cout<<"contAux: "<<contAux<<endl;
                base.seekg(contAux);
                base.read(reinterpret_cast<char*>(&bloq) , 3072);
                pos = cont-(strlen(nombreTabla.c_str())+1);
                cout<<"Pos Actual:"<<pos<<endl;
                int conWhile=0 , conComas=0;
                while(conWhile!=ncol){
                    if(bloq.data[pos]==',') {
                        conComas++;
                    }
                    if(conComas==1) {
                        tmp= tmp + bloq.data[pos];
                    }
                    if(bloq.data[pos]==';') {
                        tmp=tmp+"\n";
                        conComas=0;
                        conWhile++;
                    }
                    pos++;
                }
                tmp.at(0)=' ';
                int valor;
                cout<<"COLUMNAS:\n\t"<<tmp<<endl;
                cout<<"Ingrese Numero Columna (0 TODO):";
                cin>>valor;
                valor = valor-1;
                bool encontrado=false;
                int auxPos=(sizeof(DBMetadata));
                Back3:
                while (!base.eof() && encontrado != true) {
                    tmp ="";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char*>(&bloq), 3072);
                    if(bloq.data[0]=='D'){
                        if(BuscarAux(nombreTabla, dbname, 3072, auxPos)!=0){
                            cout<<"No Hay Registro En Tabla\n";
                            auxPos=auxPos+3072;
                            goto Back3;
                        }else{
                            encontrado=true;
                            int pos=cont;

                            if(valor==-1) {
                                while (bloq.data[pos] != ' ') {
                                    if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                    } else
                                        tmp = tmp + bloq.data[pos];
                                    pos++;
                                }
                            }else{
                                tmp="";
                                int contExtra=0;
                                while(bloq.data[pos]!=' ') {
                                    if (contExtra == valor) {
                                        tmp = tmp + bloq.data[pos];
                                    }
                                    if (bloq.data[pos] == ',') {
                                        contExtra++;
                                    }else if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                        contExtra=0;
                                    }
                                    pos++;
                                }
                            }
                        }
                    }else
                        auxPos = auxPos + 3072 ;
                }
                cout<<"-----------------DATOS------------------\n";
                cout<<tmp;
            }
        }
    }else {
        if (Buscar(nombreTabla, dbname, 4096) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock4096 bloq;
            fstream base(dbname+".dat" , ios::in | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,4096);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMP:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,4096,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else{
                tmp="";
                cout<<"contAux: "<<contAux<<endl;
                base.seekg(contAux);
                base.read(reinterpret_cast<char*>(&bloq) , 4096);
                pos = cont-(strlen(nombreTabla.c_str())+1);
                cout<<"Pos Actual:"<<pos<<endl;
                int conWhile=0 , conComas=0;
                while(conWhile!=ncol){
                    if(bloq.data[pos]==',') {
                        conComas++;
                    }
                    if(conComas==1) {
                        tmp= tmp + bloq.data[pos];
                    }
                    if(bloq.data[pos]==';') {
                        tmp=tmp+"\n";
                        conComas=0;
                        conWhile++;
                    }
                    pos++;
                }
                tmp.at(0)=' ';
                int valor;
                cout<<"COLUMNAS:\n\t"<<tmp<<endl;
                cout<<"Ingrese Numero Columna (0 TODO):";
                cin>>valor;
                valor = valor-1;
                bool encontrado=false;
                int auxPos=(sizeof(DBMetadata));
                Back4:
                while (!base.eof() && encontrado != true) {
                    tmp ="";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char*>(&bloq), 4096);
                    if(bloq.data[0]=='D'){
                        if(BuscarAux(nombreTabla, dbname, 4096, auxPos)!=0){
                            cout<<"No Hay Registro En Tabla\n";
                            auxPos=auxPos+512;
                            goto Back4;
                        }else{
                            encontrado=true;
                            int pos=cont;

                            if(valor==-1) {
                                while (bloq.data[pos] != ' ') {
                                    if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                    } else
                                        tmp = tmp + bloq.data[pos];
                                    pos++;
                                }
                            }else{
                                tmp="";
                                int contExtra=0;
                                while(bloq.data[pos]!=' ') {
                                    if (contExtra == valor) {
                                        tmp = tmp + bloq.data[pos];
                                    }
                                    if (bloq.data[pos] == ',') {
                                        contExtra++;
                                    }else if (bloq.data[pos] == ';') {
                                        tmp = tmp + "\n";
                                        contExtra=0;
                                    }
                                    pos++;
                                }
                            }
                        }
                    }else
                        auxPos = auxPos +4096;
                }
                cout<<"-----------------DATOS------------------\n";
                cout<<tmp;
            }
        }
    }
}
void Table::EliminarRegistro(string dbname , int tamano_bloque) {
    string tmp, tmpCol;
    int ncol;
    string nombreTabla;
    cout<<"Ingrese Nombre Tabla:";
    cin>>nombreTabla;
    if(tamano_bloque==512) {
        if (Buscar(nombreTabla, dbname, 512) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock512 bloq;
            fstream base(dbname+".dat" , ios::in| ios::out | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,512);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMPCol:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,512,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else {
                bool encontrado = false;
                int auxPos = (sizeof(DBMetadata));
                Back:
                while (!base.eof() && encontrado != true) {
                    tmp = "";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char *>(&bloq), 512);
                    if (bloq.data[0] == 'D') {
                        if (BuscarAux(nombreTabla, dbname, 512, auxPos) != 0) {
                            cout << "No Hay Registro En Tabla\n";
                            auxPos = auxPos + 512;
                            goto Back;
                        } else {
                            encontrado = true;
                            int pos = 1;
                            bloq.data[0]='N';
                            while(pos<512) {
                                bloq.data[pos]=' ';
                                pos++;
                            }
                            cout<<"Bloque:\n"<<bloq.data<<endl;
                            base.seekp(auxPos);
                            base.write(reinterpret_cast<char*>(&bloq), 512);
                        }
                    }else
                        auxPos = auxPos + 512 ;
                }
            }
            cout<<"Registros Eliminados\n";
        }
    }else if(tamano_bloque==1024){
        if (Buscar(nombreTabla, dbname, 1024) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock1024 bloq;
            fstream base(dbname+".dat" , ios::in| ios::out | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,1024);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMPCol:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,1024,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else {
                bool encontrado = false;
                int auxPos = (sizeof(DBMetadata));
                Back1:
                while (!base.eof() && encontrado != true) {
                    tmp = "";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char *>(&bloq), 1024);
                    if (bloq.data[0] == 'D') {
                        if (BuscarAux(nombreTabla, dbname, 1024, auxPos) != 0) {
                            cout << "No Hay Registro En Tabla\n";
                            auxPos = auxPos + 1024;
                            goto Back1;
                        } else {
                            encontrado = true;
                            int pos = 1;
                            bloq.data[0]='N';
                            while(pos<1024) {
                                bloq.data[pos]=' ';
                                pos++;
                            }
                            cout<<"Bloque:\n"<<bloq.data<<endl;
                            base.seekp(auxPos);
                            base.write(reinterpret_cast<char*>(&bloq), 1024);
                        }
                    }else
                        auxPos = auxPos + 1024;
                }
            }
            cout<<"Registros Eliminados\n";
        }
    }else if(tamano_bloque==2048){
        if (Buscar(nombreTabla, dbname, 2048) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock2048 bloq;
            fstream base(dbname+".dat" , ios::in| ios::out | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,2048);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMPCol:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,2048,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else {
                bool encontrado = false;
                int auxPos = (sizeof(DBMetadata));
                Back2:
                while (!base.eof() && encontrado != true) {
                    tmp = "";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char *>(&bloq), 2048);
                    if (bloq.data[0] == 'D') {
                        if (BuscarAux(nombreTabla, dbname, 2048, auxPos) != 0) {
                            cout << "No Hay Registro En Tabla\n";
                            auxPos = auxPos + 2048;
                            goto Back2;
                        } else {
                            encontrado = true;
                            int pos = 1;
                            bloq.data[0]='N';
                            while(pos<2048) {
                                bloq.data[pos]=' ';
                                pos++;
                            }
                            cout<<"Bloque:\n"<<bloq.data<<endl;
                            base.seekp(auxPos);
                            base.write(reinterpret_cast<char*>(&bloq), 2048);
                        }
                    }else
                        auxPos = auxPos + 2048;
                }
            }
            cout<<"Registros Eliminados\n";
        }
    }else if(tamano_bloque==3072){
        if (Buscar(nombreTabla, dbname, 3072) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock3072 bloq;
            fstream base(dbname+".dat" , ios::in| ios::out | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,3072);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMPCol:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,3072,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else {
                bool encontrado = false;
                int auxPos = (sizeof(DBMetadata));
                Back3:
                while (!base.eof() && encontrado != true) {
                    tmp = "";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char *>(&bloq), 3072);
                    if (bloq.data[0] == 'D') {
                        if (BuscarAux(nombreTabla, dbname, 3072, auxPos) != 0) {
                            cout << "No Hay Registro En Tabla\n";
                            auxPos = auxPos + 3072;
                            goto Back3;
                        } else {
                            encontrado = true;
                            int pos = 1;
                            bloq.data[0]='N';
                            while(pos<3072) {
                                bloq.data[pos]=' ';
                                pos++;
                            }
                            cout<<"Bloque:\n"<<bloq.data<<endl;
                            base.seekp(auxPos);
                            base.write(reinterpret_cast<char*>(&bloq), 3072);
                        }
                    }else
                        auxPos = auxPos + 3072;
                }
            }
            cout<<"Registros Eliminados\n";
        }
    }else{
        if (Buscar(nombreTabla, dbname, 4096) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock4096 bloq;
            fstream base(dbname+".dat" , ios::in| ios::out | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,4096);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMPCol:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            if (BuscarAux(nombreTabla, dbname,4096,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else {
                bool encontrado = false;
                int auxPos = (sizeof(DBMetadata));
                Back4:
                while (!base.eof() && encontrado != true) {
                    tmp = "";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char *>(&bloq), 4096);
                    if (bloq.data[0] == 'D') {
                        if (BuscarAux(nombreTabla, dbname, 4096, auxPos) != 0) {
                            cout << "No Hay Registro En Tabla\n";
                            auxPos = auxPos + 4096;
                            goto Back4;
                        } else {
                            encontrado = true;
                            int pos = 1;
                            bloq.data[0]='N';
                            while(pos<4096) {
                                bloq.data[pos]=' ';
                                pos++;
                            }
                            cout<<"Bloque:\n"<<bloq.data<<endl;
                            base.seekp(auxPos);
                            base.write(reinterpret_cast<char*>(&bloq), 4096);
                        }
                    }else
                        auxPos = auxPos + 4096;
                }
            }
            cout<<"Registros Eliminados\n";
        }
    }
}
void Table::ActualizarRegistro(string dbname , int tamano_bloque){
    string tmp,tmp2 ,tmpCol , auxGuardar ,auxColumnas , condicion;
    int ncol, contWhile=0 , cantRegistros=0 , colMod;
    string nombreTabla;
    cout<<"Ingrese Nombre Tabla:";
    cin>>nombreTabla;
    if(tamano_bloque==512) {
        if (Buscar(nombreTabla, dbname, 512) != 0)
            cout << "Tabla No existe\n";
        else {
            int pos = cont - (strlen(nombreTabla.c_str()) + 1);
            int contAux=0;
            DataBlock512 bloq;
            fstream base(dbname+".dat" , ios::in| ios::out | ios::binary);
            base.seekg((sizeof(DBMetadata)));
            base.read(reinterpret_cast<char*>(&bloq) ,512);
            while(bloq.data[pos]!=';') {
                if(bloq.data[pos]==','){
                    contAux++;
                }
                if(contAux==1){
                    tmpCol+=bloq.data[pos];
                }
                if(contAux==2){
                    tmp=tmp+bloq.data[pos];
                }
                pos++;
            }
            tmpCol.at(0)=' ';
            cout<<"TMPCol:"<<tmpCol<<endl;
            ncol=stoi(tmpCol);
            tmp.at(0)=' ';
            cout<<"TMP:"<<tmp<<endl;
            contAux=stoi(tmp);

            base.seekg(contAux);
            base.read(reinterpret_cast<char *>(&bloq), 512);
            if (BuscarAux(nombreTabla, dbname,512,contAux) != 0)
                cout << "Tabla sin Columnas\n";
            else {
                pos =cont - (strlen(nombreTabla.c_str())+1);
                cout << " Posicion Columna: " << pos << endl;

                while (contWhile != ncol) {
                    auxColumnas = auxColumnas + bloq.data[pos];
                    if (bloq.data[pos] == ';') {
                        auxColumnas=auxColumnas+"\n";
                        contWhile++;
                    }
                    pos++;
                }
                cout<<"--------------COLUMNAS------------\n"<<auxColumnas;
                cout<<"-----------------------------------\n";
                cout<<"Columna A modificar (0 TODAS): ";
                cin>>colMod;
                colMod= colMod-1;

                bool encontrado = false;
                int auxPos = (sizeof(DBMetadata));
                Back:
                while (!base.eof() && encontrado != true) {
                    tmp = "";
                    base.seekg(auxPos);
                    base.read(reinterpret_cast<char *>(&bloq), 512);
                    if (bloq.data[0] == 'D'){
                        if (BuscarAux(nombreTabla, dbname, 512, auxPos) != 0) {
                            cout << "No Hay Registro En Tabla\n";
                            auxPos = auxPos + 512;
                            goto Back;
                        } else {
                            encontrado=true;
                            tmp=bloq.data;
                            cout<<"---------------------DATA ACTUAL---------------------\n"<<tmp<<endl;
                            cout<<"-------------------------------\n";
                            string opc;
                            cout<<"Desea poner Condicion?:";
                            cin>>opc;
                            if(opc=="si"){
                                cout<<"Datos Condicionar: ";
                                cin>>condicion;
                            }
                            contWhile=0;
                            int contwhile2=0;
                            while(contwhile2<512){
                                if(bloq.data[contwhile2]==' ')
                                    break;
                                if(bloq.data[contwhile2]==';')
                                    cantRegistros++;
                                contwhile2++;
                            }
                            pos=cont;
                            cantRegistros=cantRegistros-1;
                            cout<<"Cantidad Registros: "<<cantRegistros<<endl;
                            while(contWhile<cantRegistros){
                                tmp2 =tmp2 + tmp.at(pos);
                                if(tmp.at(pos)==';'){
                                    contWhile++;
                                    cout<<"TMP2:\n"<<tmp2<<endl;
                                }
                                pos++;
                            }
                            if(colMod==-1){
                                if(opc=="si"){
                                    int contCommas=0, contExtra=0 , pos1=0 , pos2=0;
                                    string ver;
                                    int x1=0;
                                    double x2=0;
                                    string x3=" ";
                                    while (contExtra<ncol){
                                        if(auxColumnas.at(pos1)==','){
                                            contCommas++;
                                        }if(contCommas==2){
                                            ver=ver+auxColumnas.at(pos1);
                                        }
                                        if(auxColumnas.at(pos1)==';'){
                                            /*cout<<ver<<endl;
                                            if(ver==",Int"){
                                                cout<<"Ingrese Int:";
                                                cin>>x1;
                                            }else if(ver==" ,Double:"){
                                                cout<<"Ingrese Double";
                                                cin>>x2;
                                            }else{
                                                cout << "Ingrese Varchar:";
                                                cin >> x3;
                                            }
*/
                                            ver=" ";
                                            contExtra++;
                                            contCommas=0;
                                        }
                                        pos1++;
                                    }
                                    int contB=0;
                                    string ver2;
                                    while(contB<cantRegistros) {
                                        if (ver2 == condicion) {
                                            cout<<"Registro Encontrado: "<<contB<<endl;

                                            break;
                                        }
                                        ver2 = ver2 + tmp2.at(pos2);
                                        if(tmp2.at(pos2)==';'){
                                                ver2 = "";
                                                contB++;
                                        }
                                        if(tmp2.at(pos2) == ','){
                                                ver2="";
                                        }


                                        pos2++;
                                    }

                                }
                            }else{
                                if(opc=="si"){

                                }
                            }
                        }
                    }else
                        auxPos = auxPos + 512;
                }
            }
        }
    }
}
