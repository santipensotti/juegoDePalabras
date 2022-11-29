#include "conjDigital.h"
#include "Tipos.h"

template<typename T>
conjDigital<T>::conjDigital():raiz(NULL) {
    _lmax = 0;
}

template<typename T>
conjDigital<T>::~conjDigital() {
    destruir(raiz);
}
template<typename T>
void conjDigital<T>::destruir(conjDigital::Nodo * actual) {

    if (actual != nullptr){
         for (int i = 0; i < 256 ; i++) {
             destruir(actual->siguientes[i]);
         }
         delete actual;
     }

}
template<typename T>
void conjDigital<T>::agregar(Palabra palabra) {
    if (raiz ==NULL){
        raiz = new Nodo();
    }
    Nodo* actual= raiz;

    for (int i = 0; i < palabra.size(); i++){
        char c = palabra[i];
        if (actual->siguientes[c] == nullptr){
            actual->siguientes[c] = new Nodo();
        }
        actual = actual->siguientes[c];

    }
    actual->esPalabra = true;
    if (palabra.size() > _lmax){
        _lmax = palabra.size();
    }

}

template<typename T>
int conjDigital<T>::lMax() {
    return _lmax;
}

template<typename T>
bool conjDigital<T>::pertenece( Palabra palabra) const {
    Nodo* actual = raiz;
    for (int i = 0; i < palabra.size(); i++) {
        actual = actual->siguientes[palabra[i]];
        if (actual == nullptr){
            return false;
        }
    }
    return actual->esPalabra;
}



template<typename T>
conjDigital<T>::conjDigital(const conjDigital<T> &aCopiar) {
    *this = aCopiar;
}

template<typename T>
conjDigital<T>& conjDigital<T>::operator=(const conjDigital &d) {

    Nodo* copiado = new Nodo();
    copiarAux(d.raiz, copiado);
    //destruir(this->raiz);
    raiz = copiado;
    _lmax = d._lmax;
    return  *this;
}

template<typename T>
void conjDigital<T>::copiarAux(conjDigital::Nodo *actual, conjDigital::Nodo *copiado) {
    bool tieneHijos = false;
    for (int i = 0; i < 256; ++i) {
        if (actual->siguientes[i] != nullptr){
            tieneHijos = true;
        }
    }
    if (tieneHijos){
        for(int i = 0; i<256; i++){
            if (actual->siguientes[i]!= nullptr){
                if (copiado->siguientes[i] == NULL){
                    copiado->siguientes[i] = new Nodo();
                }
                if (actual->siguientes[i]->esPalabra== true){
                    copiado->siguientes[i]->esPalabra  =true;
                }
                //copiado->siguientes[i]->esPalabra = actual->siguientes[i]->esPalabra;
                copiarAux(actual->siguientes[i],copiado->siguientes[i]);
            }
        }
    }
}




