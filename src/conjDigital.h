
#ifndef TP_JUEGODEPALABRAS_CONJDIGITAL_H
#define TP_JUEGODEPALABRAS_CONJDIGITAL_H


template<typename T>
class conjDigital{
public:

    conjDigital();
    ~conjDigital();

    conjDigital(const conjDigital<T>& aCopiar);
    conjDigital& operator =(const conjDigital& d);

    void agregar(Palabra);
    int lMax();
    bool pertenece(Palabra palabra) const;

private:
    struct Nodo {
        std::vector<Nodo*> siguientes;
        bool esPalabra;
        Nodo() : siguientes(256, nullptr), esPalabra(false){}
        Nodo(T* def) : siguientes(256, nullptr), esPalabra(true){}
    };
    Nodo* raiz;
    int _lmax;
    void destruir(Nodo*);
    void copiarAux(Nodo* actual, Nodo* copiado);

};



#include "conjDigital.hpp"
#endif //TP_JUEGODEPALABRAS_CONJDIGITAL_H
