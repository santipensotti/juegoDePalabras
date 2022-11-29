#include "Fachada_Servidor.h"
#include "Fachada_Servidor.h"

Fachada_Servidor::Fachada_Servidor(Nat cantJugadores, Fachada_Variante &variante,  Repositorio &r):
        _servidor(cantJugadores,make_tuple(variante.tamanoTablero(),variante.fichas(), variante.puntFichas(),
                                           variante.repo(), variante.puntajes(), variante.palabrasLegitimas()),
                  pasarA(r)) {

}


IdCliente Fachada_Servidor::conectarCliente() {
    _servidor.conectarCliente();
    return _servidor.conectados() -1;
}

void Fachada_Servidor::recibirMensaje(IdCliente id, const Ocurrencia &o) {
    _servidor.recibirMensaje(id,o);
}

Nat Fachada_Servidor::jugadoresEsperados() {
    return _servidor.esperados();
}

Nat Fachada_Servidor::jugadoresConectados() {
    return _servidor.conectados();
}

std::list<Notificacion> Fachada_Servidor::notificaciones(IdCliente id) {
    list<Notificacion> res = _servidor.consultar(id);
    /*
    auto it = res.begin();
    it++;

    if (_servidor.conectados() == _servidor.esperados() and res.front().tipoNotificacion() == TipoNotificacion::IdCliente){
        res.push_back(Notificacion::nuevaReponer(_servidor.juegoActual().respuestas(id)));
    }*/
    return res;
}

queue<Letra> & Fachada_Servidor::pasarA(Repositorio r) {
    _repositorio = new queue<Letra>;
    auto itr = r.begin();
    while(itr != r.end()){
        _repositorio->push(*itr);
        itr++;
    }
    return  *_repositorio;
}

Fachada_Servidor::~Fachada_Servidor() {
    delete _repositorio;
}
