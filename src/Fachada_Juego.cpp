#include "Fachada_Juego.h"


Fachada_Juego::Fachada_Juego(Nat k, Fachada_Variante &v,  Repositorio &r) : _juego(Juego(k,make_tuple(v.tamanoTablero(),
                                                              v.fichas(), v.puntFichas(),v.repo(),v.puntajes(),
                                                              v.palabrasLegitimas()),pasar(r))){

    tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>, map<Letra, Nat>, set<vector<Letra>>> var = _juego.variante();
    _variante = new Fachada_Variante(get<0>(var), get<1>(var), get<4>(var), get<5>(var));

}

void Fachada_Juego::ubicar(const Ocurrencia &o) {
    _juego.ubicar(o);
}

IdCliente Fachada_Juego::turno() {
    return _juego.turno();
}

const Fachada_Variante &Fachada_Juego::variante() {
    return *_variante;
}

bool Fachada_Juego::jugadaValida(const Ocurrencia &o) {
    return _juego.jugadaValida(o);
}

bool Fachada_Juego::hayLetra(Nat x, Nat y) {
    return _juego.hayLetra(x,y);
}

Letra Fachada_Juego::letra(Nat i, Nat j) {
    return _juego.letraEn(i,j);
}

Nat Fachada_Juego::puntaje(IdCliente id) {
    return _juego.puntaje(id);
}

Nat Fachada_Juego::cantFicha(IdCliente id, Letra l) {
    return _juego.cantLetra(id,l);
}

queue<Letra> &Fachada_Juego::pasar(Repositorio r) {
    _repositorio = new queue<Letra>;
    auto itr = r.begin();
    while(itr != r.end()){
        _repositorio->push(*itr);
        itr++;
    }
    return  *_repositorio;
}

Fachada_Juego::~Fachada_Juego() {
    delete _variante;
    delete _repositorio;
}
