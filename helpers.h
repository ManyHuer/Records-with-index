#include <iostream>
#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED
#ifdef _WIN32
    #define CLEAR "cls"
#elif defined(unix)
    #define CLEAR "clear"
#else
    #error "S0 no soportado para limpiar pantalla"
#endif

using namespace std;

void pausa(){
    cin.get();
    cout << "Presione enter para continuar...";
    cin.get();
}

void pausaSinBuffer(){
    cout << "Presione enter para continuar . . .";
    cin.get();
}

#endif // HELPERS_H_INCLUDED
