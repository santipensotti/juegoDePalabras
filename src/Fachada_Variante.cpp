#include "Fachada_Variante.h"
#include <vector>
Fachada_Variante::Fachada_Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat> &puntajes,
                                   const set<vector<Letra>> &palabrasLegitimas) {
    //_palabras = new conjDigital<Palabra>;
    for (auto it = palabrasLegitimas.begin(); it != palabrasLegitimas.end(); ++it) {
        Palabra palabra = *it;
        _repositorio.agregar(palabra);
    }
    tamano = tamanoTab;
    Fichas = cantFichas;
    puntajeFichas = vector<Nat> (256,1);
    auto iter = puntajes.begin();
    while (iter != puntajes.end()){
        puntajeFichas[iter->first] = iter->second;
        iter++;
    }
    _puntajes = puntajes;
    _palabrasLegitimas = palabrasLegitimas;
}

conjDigital<Palabra> Fachada_Variante::repo() {
    conjDigital<Palabra> copia = _repositorio;
    return copia;
}

Nat Fachada_Variante::tamanoTablero() const {
    return tamano;
}

vector<Nat> Fachada_Variante::puntFichas() {
    return puntajeFichas;
}

bool Fachada_Variante::palabraLegitima(const Palabra &palabra) const {
    return _repositorio.pertenece(palabra);
}

Nat Fachada_Variante::puntajeLetra(Letra l) const {
    return puntajeFichas[l];
}

Nat Fachada_Variante::fichas() const {
    return Fichas;
}

const map<Letra, Nat> &Fachada_Variante::puntajes() {
    return _puntajes;
}

const set<vector<Letra>> &Fachada_Variante::palabrasLegitimas() {
    return _palabrasLegitimas;
}

Fachada_Variante::~Fachada_Variante() {
    //delete _palabras;
}
