#include <iostream>
#include "Tipos.h"
#include "Servidor.h"
#include "Juego.h"
#include "Notificacion.h"
#include "Letra.h"
using namespace std;

Servidor::Servidor(int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>,map<Letra, Nat>,set<vector<Letra>>> const
        &variante, queue<Letra> &repositorio)

: _juego(Juego(k, variante, repositorio))

{

    _notificaciones = vector<queue<Notificacion>>(k, queue<Notificacion>()); 
    _var = variante;                       
    _esperados = k;
    _conectados = 0;
    _consultoEmpezar = vector<bool>(k,false);      
    _repo = repositorio;                            
}

void Servidor::conectarCliente() {

    _notificaciones[_conectados].push(Notificacion::nuevaIdCliente(_conectados)); //
    _conectados ++;

    if (_conectados == _esperados){

        for (int i = 0; i < _esperados  ; i++){
            _notificaciones[i].push(Notificacion::nuevaEmpezar(get<0>(_var)));
            _notificaciones[i].push(Notificacion::nuevaTurnoDe(0));
            _notificaciones[i].push(Notificacion::nuevaReponer(_juego.respuestas(i)));
        }
    }
}


list<Notificacion> Servidor::consultar(IdCliente cid) {
    list<Notificacion> notDevueltas;
    while (!_notificaciones[cid].empty()){
        notDevueltas.push_back(_notificaciones[cid].front());
        _notificaciones[cid].pop();
    }
    return notDevueltas;
}

int Servidor::conectados() {
    return _conectados;
}
int Servidor::esperados() {
    return _esperados;
}

Juego Servidor::juegoActual() {
    return _juego;
}

bool Servidor::consultoEmpezar(IdCliente cid) {
    return _consultoEmpezar[cid];
}

 tuple<tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>>, Repositorio> Servidor::configuracion(){
    tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>> variante =make_tuple(get<0>(_var),get<1>(_var),get<2>(_var), get<3>(_var));
}



Servidor::Servidor( const Servidor &aCopiar (int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>> const &variante, queue<Letra> &repositorio)) : _juego(Juego(
        this->_esperados,this->_var, this->_repo)){

    *this = aCopiar;
}

void Servidor::recibirMensaje(IdCliente cid, Ocurrencia o) {

    bool tieneTodasFichas = true;
    vector<Nat> fichasParaJugar = vector<Nat>(256,0);
    bool esValida = false;
    Nat puntajeViejo = _juego.puntaje(cid);
    vector<Nat> fichasViejas = _juego.fichasJugador(cid);
    int turno = _juego.turno();
    for (auto i : o){
        fichasParaJugar[get<2>(i)] ++;
    }
    for (Nat j = 0; j<256; j++){
        if (_juego.fichasJugador(cid)[j] < fichasParaJugar[j]){
            tieneTodasFichas = false;
        }
    }
    if (turno == cid and _juego.jugadaValida(o) and tieneTodasFichas and _conectados == _esperados ) {
        esValida = true;
        _juego.ubicar(o);
    }
    if(o.size()> 0 and _conectados != _esperados){
        _notificaciones[cid].push(Notificacion::nuevaMal());
    }
    Nat nuevoPuntaje = _juego.puntaje(cid);
    for (int i = 0; i < _esperados; i++) {
        if (_conectados == _esperados) {
            if (esValida) {
                _notificaciones[i].push(Notificacion::nuevaUbicar(cid, o));
                Notificacion a = Notificacion::nuevaReponer({'a'});
                multiset<Letra> c =  a.reponer();
                _notificaciones[i].push(Notificacion::nuevaSumaPuntos(cid, nuevoPuntaje-puntajeViejo));
                if (i == cid){
                    _notificaciones[cid].push(Notificacion::nuevaReponer(_juego.respuestas(cid)));
                }
                _notificaciones[i].push(Notificacion::nuevaTurnoDe(cid+1));
            } else {
                _notificaciones[i].push(Notificacion::nuevaMal());
            }
        }
    }
}

Servidor &Servidor::operator=(const Servidor &aCopiar) {
    this->_conectados = aCopiar._conectados;
    this->notificaciones = aCopiar.notificaciones;
    this->_consultoEmpezar = aCopiar._consultoEmpezar;
    this->_esperados = aCopiar._esperados;
    this->_juego = aCopiar._juego;
    this->_notificaciones = aCopiar._notificaciones;
    this->_var = aCopiar._var;
    return  *this;
}









