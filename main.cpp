#include <iostream>
#include <cstring>
#include "helpers.h"
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <sstream>
#include <fstream>
#include <locale>
#define TAMANIO_NOMBRE 50
#define TAMANIO_DESCRIPCION 180
#define TAMANIO_ESPECIALIDAD 35
#define TAMANIO_CODIGO 10
#define TAMANIO_FLOAT 10
#define NUMERO_ALEATORIO_MAXIMO 100
#define MAX_INDICES 30
#define ARCHIVO_DATOS "MENU.txt"
#define ARCHIVO_INDICE "INDICE.txt"
#define DELIMITADOR_CAMPOS "|"
#define DELIMITADOR_REGISTROS '\n'

using namespace std;

struct Producto {
    char codigo[TAMANIO_CODIGO+1];
    char nombre[TAMANIO_NOMBRE+1];
    char descripcion[TAMANIO_DESCRIPCION+1];
    float precio;
    char especialidad[TAMANIO_ESPECIALIDAD+1];
};

struct IndicePrimario{
    char codigo[TAMANIO_CODIGO+1];
    int NRR;
};

void llenarEspacios(char palabra[], int tamanioMaximo);
int menuPrincipal();
void agregarProducto(int& cantProductos);
void generarCodigo(Producto& producto);
void guardarProducto(Producto producto, int cantProductos);
void modificarIndice(char codigo[], int NRR);
void numeroAchar(char codigo[], int numero);
void escogerEspecialidad(char especialidad[]);
void consultarProducto(int cantProductos);
void cerrarPrograma();
void quicksort(int limiteIzquierdo, int limiteDerecho);
void imprimirIndices(int cantProductos);
void guardarIndice(int cantIndices);
bool tamanioNombrevalido(char nombre[]);
bool validarDescripcion(char descripcion[]);
bool validarPrecio(int precio);
void inicializar(int& cantProductos);
void flotanteAchar(char flotanteChar[], float numero);
string borrarEspacios(string cadena);

IndicePrimario indicesPrimarios[MAX_INDICES];

int main() {
    int opcionMenu;
    int cantProductos=0;
    system(CLEAR);
    inicializar(cantProductos);
    do{
        opcionMenu=menuPrincipal();
        switch(opcionMenu){
            case 1:    agregarProducto(cantProductos);  break;
            case 2:    consultarProducto(cantProductos);  break;
            case 3:    cerrarPrograma();     break;
            default:   cout << "Opción no valida" << endl;
        }
        if(opcionMenu==1){
            pausa();
            system(CLEAR);
        }
    }while(opcionMenu!=3);
}

void inicializar(int& cantProductos){
    ifstream archivo;
    string cadena, subCadena;
    archivo.open(ARCHIVO_INDICE);
    if(archivo.is_open()){
        while(!archivo.eof()){
            getline(archivo,cadena,DELIMITADOR_REGISTROS);
            if(cadena.length()>TAMANIO_CODIGO){
                subCadena=cadena.substr(0, TAMANIO_CODIGO); //Extraemos desde el inicio de la cadena hasta la cantidad de maxima de caracteres de un codigo
                strcpy(indicesPrimarios[cantProductos].codigo, subCadena.data()); //Asignamos lo de la subCadena a codigo del indice
                indicesPrimarios[cantProductos].NRR=stoi(cadena.substr(TAMANIO_CODIGO+1, sizeof(int))); //apartir de la posicion siguiente del codigo extraemos el NRR y lo conversimos a entero y lo pasamos al NRR del indice
                cantProductos++;
            }
        }
    }
}

string borrarEspacios(string cadena){
    int tamanioCadena, posFinalCadena;
    tamanioCadena=cadena.length(); //Obtenemos el tamaño de la cadena
    for(int i=0; i<tamanioCadena; i++){
        if(cadena[i]==' ' && cadena[i+1]==' '){ //Buscamos que se encuentren 2 espacios seguidos
            posFinalCadena=i; //asignamos i que es el primer espacio a una variable
            break;
        }
    }
    cadena.erase(posFinalCadena, tamanioCadena); //borramos desde el primer espacio hasta el final de la cadena
    return cadena;
}

void consultarProducto(int cantProductos){
    int pos1raComa, pos2daComa, NRR;
    string cadena, subCadena;
    bool band=false;
    Producto producto;
    ifstream archivo;
    char codigoBuscar[TAMANIO_CODIGO+1];
    if(cantProductos!=0){
        cout<<"------------"<<endl;
        cout<<"| Consulta |"<<endl;
        cout<<"------------"<<endl;
        cout<<"Ingrese el codigo del producto(letras en mayusculas):";
        cin.getline(codigoBuscar, TAMANIO_CODIGO+1); //Capturamos el codigo a buscar
        llenarEspacios(codigoBuscar, TAMANIO_CODIGO);
        for(int i=0; i<cantProductos; i++){
            if(!strcmpi(codigoBuscar, indicesPrimarios[i].codigo)){
                NRR=indicesPrimarios[i].NRR;
                band=true;
                break;
            }
        }
        if(band){
            archivo.open(ARCHIVO_DATOS);
            if(archivo.is_open()){
                for(int i=0; i<NRR; i++){
                    getline(archivo,cadena,DELIMITADOR_REGISTROS);
                }
                //Esto para extraer los cadenas del archivo
                pos1raComa=cadena.find_first_of(DELIMITADOR_CAMPOS, 0); //Buscamos el delimitador
                subCadena=cadena.substr(0, pos1raComa); //extraemos de la cadena el delimitador
                subCadena=borrarEspacios(subCadena); //Borramos los espacios de la subCadena
                strcpy(producto.codigo, subCadena.data()); //le asignamos lo de la subCadena al atributo del producto correspondiente

                pos2daComa=cadena.find_first_of(DELIMITADOR_CAMPOS, pos1raComa+1); //Buscamos el delimitador
                subCadena=cadena.substr(pos1raComa+1, pos2daComa-pos1raComa-1); //extraemos de la cadena el delimitador
                subCadena=borrarEspacios(subCadena); //Borramos los espacios de la subCadena
                strcpy(producto.nombre, subCadena.data()); //le asignamos lo de la subCadena al atributo del producto correspondiente

                pos1raComa=cadena.find_first_of(DELIMITADOR_CAMPOS, pos2daComa+1); //Buscamos el delimitador
                subCadena=cadena.substr(pos2daComa+1, pos1raComa-pos2daComa-1); //extraemos de la cadena el delimitador
                subCadena=borrarEspacios(subCadena); //Borramos los espacios de la subCadena
                strcpy(producto.descripcion, subCadena.data()); //le asignamos lo de la subCadena al atributo del producto correspondiente

                pos2daComa=cadena.find_first_of(DELIMITADOR_CAMPOS, pos1raComa+1); //Buscamos el delimitador
                subCadena=cadena.substr(pos1raComa+1, pos2daComa-pos1raComa-1); //extraemos de la cadena el delimitador
                subCadena=borrarEspacios(subCadena); //Borramos los espacios de la subCadena
                strcpy(producto.especialidad, subCadena.data()); //le asignamos lo de la subCadena al atributo del producto correspondiente
                //Esto para los flotantes(precio) del archivo

                pos1raComa=cadena.find_first_of(DELIMITADOR_CAMPOS, pos2daComa+1); //Buscamos el delimitador
                subCadena=cadena.substr(pos2daComa+1, pos1raComa-pos2daComa-1); //extraemos de la cadena el delimitador
                //archivo.seekg(tamanio+contador); //Nos movemos a la posicion del precio del NRR correspondiente
                producto.precio=stof(subCadena); //Extraemos el precio y lo asignamos
                cout<<"----------------------------"<<endl;
                cout<<"| Informacion del producto |"<<endl;
                cout<<"----------------------------"<<endl;
                cout<<"Codigo:"<<producto.codigo<<endl;
                cout<<"Nombre:"<<producto.nombre<<endl;
                cout<<"Descripcion:"<<producto.descripcion<<endl;
                cout<<"Especialidad:"<<producto.especialidad<<endl;
                cout<<"Precio:"<<producto.precio<<endl;
                archivo.close();
            }else{
                cout << "error" << endl;
            }
        }else{
            cout<<"No existe el producto . . ."<<endl;
        }
    }else{
        cout << "No hay ningun producto guardado..." << endl;
    }
    pausaSinBuffer();
    system(CLEAR);
}

int menuPrincipal(){
    int opcion;
    cout << "TELEPIZZA" << endl;
    cout << "1-Insertar registro." << endl;
    cout << "2-Mostrar un registro por su Codigo" << endl;
    cout << "3-Salir" << endl;
    cout << "Opcion: ";
    cin >> opcion;
    cin.get();
    system(CLEAR);
    return opcion;
}

void cerrarPrograma(){
    cout << "Cerrando el registro de productos..." << endl;
}

void imprimirIndices(int cantProductos){
    for(int i=0; i<cantProductos; i++){
        cout << "Codigo: " << indicesPrimarios[i].codigo << endl;
        cout << "NRR: " << indicesPrimarios[i].NRR << endl;
    }
}

bool tamanioNombrevalido(char nombre[]){
    int tamanioNombre;
    tamanioNombre=strlen(nombre); //obtenemos la longitud del nombre
    if(tamanioNombre>=3){ //si el tamaño de nombre es mayor que 3 entonces retorna verdadero
        return true;
    }else{ //si el tamaño de nombre es mayor que 3 entonces retorna verdadero
        cout << "Nombre no valido" << endl;
        pausaSinBuffer();
        system(CLEAR);
        return false;
    }
}

bool validarDescripcion(char descripcion[]){
    int tamanioNombre;
    tamanioNombre=strlen(descripcion); //obtenemos la longitud de la descripcion
    if(tamanioNombre==1 && descripcion[0]==' '){ // comparamos en que si el primer y unico carater es un espacio entonces retorne falso
        cout << "Descripcion no valida, no introduzca un espacio solo" << endl;
        pausaSinBuffer();
        system(CLEAR);
        return false;
    }else{
        return true;
    }
}

bool validarPrecio(int precio){
    if(precio < 0){// si el precio es menor que 0 entonces retorna falso
        cout << "Precio no valido" << endl;
        pausa();
        system(CLEAR);
        return false;
    }else{
        return true;
    }
}

void agregarProducto(int& cantProductos){
    system(CLEAR);
    Producto producto; //Creamos una variable producto del mismo tipo
    //Pedimos los datos correspodientes(El getline es para que podamos capturar espacios)
    do{
        cout << "Dame el nombre del producto(minimo 3 caracteres): ";
        cin.getline(producto.nombre, TAMANIO_NOMBRE+1);
    }while(!tamanioNombrevalido(producto.nombre)); //Do while que es repite mientras la funcion de validacion retorne falso
    do{
        cout << "Dame la descripcion del producto: ";
        cin.getline(producto.descripcion, TAMANIO_DESCRIPCION+1);
    }while(!validarDescripcion(producto.descripcion));//Do while que es repite mientras la funcion de validacion retorne falso
    escogerEspecialidad(producto.especialidad);
    do{
        cout << "Dame el precio del producto: ";
        cin >> producto.precio;
    }while(!validarPrecio(producto.precio));//Do while que es repite mientras la funcion de validacion retorne falso
    generarCodigo(producto); //Esta función genera el codigo a partir de lo que capturamos
    //llenamos los espacios de cada producto
    llenarEspacios(producto.nombre, TAMANIO_NOMBRE);
    llenarEspacios(producto.descripcion, TAMANIO_DESCRIPCION);
    llenarEspacios(producto.especialidad, TAMANIO_ESPECIALIDAD);
    llenarEspacios(producto.codigo, TAMANIO_CODIGO);
    guardarProducto(producto, cantProductos);
    cantProductos++;
    //Entramos en la función modificar pasando el codigo y la cantidad de productos guardados
    modificarIndice(producto.codigo, cantProductos);
    cout << "Producto agregado" << endl;
}

void modificarIndice(char codigo[], int NRR){
    system(CLEAR);
    //Le restamos una posicion al NRR para guardar en la posicion anterior
    strcpy(indicesPrimarios[NRR-1].codigo, codigo);
    indicesPrimarios[NRR-1].NRR=NRR; //aqui no restamos el NRR que le asignamos el NRR en la posición porque debemos pasar el numero de registro correspondiente
    //Se pasa la posicion inicial del arreglo y la final para comenzar a ordenar
    quicksort(0, NRR-1);
    guardarIndice(NRR);
}

void quicksort(int limiteIzquierdo, int limiteDerecho){
    int izquierdo, derecho;
    //Creamos una variable pivote del tipo indice primario para guardar el indice del centro y una variable temporal del mismo tipo para realizar el intercambio
    IndicePrimario pivote, temporal;
    int valido;
    //Establemos variables a partir de los limites de cada lado ya que estos no los modificares y las nuevas variables nos ayudar a recorrer el arreglo de indices
    izquierdo=limiteIzquierdo;
    derecho=limiteDerecho;
    //asignamos al pivote del dato del centro
    pivote=indicesPrimarios[(izquierdo+derecho)/2];
    do{
        //si el indice primario(cadena del lado izquierdo) es menor que el pivote(cadena del lado derecho) entonces devuelve un numero menor que 0
        valido=strcmp(indicesPrimarios[izquierdo].codigo, pivote.codigo);
        while(valido<0){
            izquierdo++;
            //Misma comparación que en el anterior para ver si el bucle se repite o termina
            valido=strcmp(indicesPrimarios[izquierdo].codigo, pivote.codigo);
        }
        //si el indice primario(cadena del lado izquierdo) es mayor que el pivote(cadena del lado derecho) entonces devuelve un numero mayor que 0
        valido=strcmp(indicesPrimarios[derecho].codigo, pivote.codigo);
        while(valido>0){
            derecho--;
            //Misma comparación que en el anterior para ver si el bucle se repite o termina
            valido=strcmp(indicesPrimarios[derecho].codigo, pivote.codigo);
        }
        //Si el contador izquierdo es menor o igual que el derecho entonces se realiza el intercambio
        if(izquierdo <= derecho){
            temporal=indicesPrimarios[izquierdo];
            indicesPrimarios[izquierdo]=indicesPrimarios[derecho];
            indicesPrimarios[derecho]=temporal;
            izquierdo++;
            derecho--;
        }
        //Si el contador izquierdo es menor o igual que el derecho entonces se repite el bucle
    }while(izquierdo<=derecho);
    //Si el limite del lado izquierdo es menor que el contador derecho entonces se vuelve a llamar a la funcion ahora pasando los valores antes mencionados
    if(limiteIzquierdo < derecho){
        quicksort(limiteIzquierdo, derecho);
    }
    //Si el limite del lado derecho es mayor que el contador izquierdo entonces se vuelve a llamar a la funcion ahora pasando los valores antes mencionados
    if(limiteDerecho > izquierdo){
        quicksort(izquierdo, limiteDerecho);
    }
}

void guardarIndice(int cantIndices){
    ofstream archivo;
    //Abrimos el archivo y lo reescribimos
    archivo.open(ARCHIVO_INDICE, ios::out);
    if(archivo.is_open()){
        //Con el for recorremos todo el indicePrimario y vamos guardando uno por uno
        for(int i=0; i<cantIndices; i++){
            //Si es el primer producto a guardar entonces no daremos un salto de linea
            if(i!=0){
                archivo << DELIMITADOR_REGISTROS;
            }
            //Guardamos cada atributo del indice primario en el archivo seguido de un delimitador de campos a excepción del ultimo que no debe llevarlo
            archivo << indicesPrimarios[i].codigo << DELIMITADOR_CAMPOS;
            archivo << indicesPrimarios[i].NRR;
        }
        archivo.close();
    }else{
        cout << "Error al abrir archivo" << endl;
    }
}

void flotanteAchar(char flotanteChar[], float numero){
    string cadena;
    stringstream conversor;
    conversor << numero; //Pasamos el numero a un simulador de buffer
    conversor >> cadena; //Y despues a un string
    strcpy(flotanteChar, cadena.data()); //copiamos lo del string a la variable codigo
}

void guardarProducto(Producto producto, int cantProductos){
    ofstream archivo;
    char flotanteChar[10];
    //Abrimos el archivo y lo ponemos en ios::app para no reescribirlo y podernos desplazarnos en este
    archivo.open(ARCHIVO_DATOS, ios::app);
    if(archivo.is_open()){
        //Aqui va el seek para ir al final del archivo
        archivo.seekp(SEEK_END);// es para pocisionar el cursor en el archivo, donde se requiera
        //Si es el primer producto a guardar entonces no daremos un salto de linea
        if(cantProductos!=0){
            archivo << DELIMITADOR_REGISTROS;
        }
        //Guardamos cada atributo del producto en el archivo seguido de un delimitador de campos a excepción del ultimo que no debe llevarlo
        archivo << producto.codigo << DELIMITADOR_CAMPOS;
        archivo << producto.nombre << DELIMITADOR_CAMPOS;
        archivo << producto.descripcion << DELIMITADOR_CAMPOS;
        archivo << producto.especialidad << DELIMITADOR_CAMPOS;
        flotanteAchar(flotanteChar, producto.precio);
        archivo.write(flotanteChar, TAMANIO_FLOAT);
        //archivo << producto.precio;
        archivo.close();
    }else{
        cout << "Error al abrir archivo" << endl;
    }
}

void numeroAchar(char codigo[], int numero){
    string cadena;
    stringstream conversor;
    conversor << numero; //Pasamos el numero a un simulador de buffer
    conversor >> cadena; //Y despues a un string
    strcpy(codigo, cadena.data()); //copiamos lo del string a la variable codigo
}

void generarCodigo(Producto &producto){
    char codigo[TAMANIO_CODIGO];
    char letrasIniciales[4];
    int numeroAleatorio;
    srand(time(NULL)); //Dependiendo de la hora del sistema comienza a generar el numero aleatorio
    numeroAleatorio=10+rand()%(NUMERO_ALEATORIO_MAXIMO-10); //establecemos el rango del numero aleatorio
    numeroAchar(codigo, numeroAleatorio); // convertimos el numero a char
    if(!strcmp(producto.especialidad, "PARA PICAR Y COMPARTIR")){
        strcat(codigo, "PI"); //Concatenamos los caracteres iniciales al codigo
    }else{
        letrasIniciales[0]=producto.especialidad[0]; //pasamos el caracter inicial a un char
        strcat(codigo, letrasIniciales); //Concatenamos los caracteres iniciales al codigo
    }
    //Pasamos los tres caracteres iniciales del nombre a un char
    letrasIniciales[0]=toupper(producto.nombre[0]);
    letrasIniciales[1]=toupper(producto.nombre[1]);
    letrasIniciales[2]=toupper(producto.nombre[2]);
    letrasIniciales[3]='\0';
    strcat(codigo, letrasIniciales); //Concatenamos los caracteres iniciales al codigo
    strcpy(producto.codigo, codigo); //Copiamos lo de nuestra variable codigo al codigo del producto
}

void escogerEspecialidad(char especialidad[]){
    int opcion;
    bool opcionValida;
    do{
        opcionValida=true;
        cout << "Ingresa el numero de la especialidad a la que pertenece" << endl;
        cout << "1-PIZZAS" << endl;
        cout << "2-PARA PICAR Y COMPARTIR" << endl;
        cout << "3-HELADOS" << endl;
        cout << "4-BEBIDAS" << endl;
        cout << "-> ";
        cin >> opcion;
        //Dependiendo de la opción escogida se copiara la cadena a la especialidad y en caso de no existir la opción se repetira el bucle
        switch(opcion){
            case 1:    strcpy(especialidad, "PIZZAS");    break;
            case 2:    strcpy(especialidad, "PARA PICAR Y COMPARTIR");   break;
            case 3:    strcpy(especialidad, "HELADOS");                  break;
            case 4:    strcpy(especialidad, "BEBIDAS");                  break;
            default:
                cout << "Opcion no valida" << endl;
                pausa();
                system(CLEAR);
                opcionValida=false;
        }
    }while(!opcionValida);

}

void llenarEspacios(char palabra[], int tamanioMaximo){
    int longitudPalabra;
    longitudPalabra=strlen(palabra); //se obtiene la logitud de la palabra
    //A partir de la longitud comenzamos a añadirle espacios
    for(int i=longitudPalabra; i<tamanioMaximo; i++){
        strcat(palabra, " "); //concatenamos espacios a la palabra
    }
}
