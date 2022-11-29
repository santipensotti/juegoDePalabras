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
/*Inicializar el servidor, para una mayor facilidad cuando se consulta la variante, se recibe tambien un map letra nat
 * y un set con las palabras validas. Esto no afecta la complejidad ni se usa durante el programa, pero
 * para crear una Fachada Variante se necesita un map y set*/
{

    _notificaciones = vector<queue<Notificacion>>(k, queue<Notificacion>()); // O(K)
    _var = variante;                        //Como se pasa por referencia es O(1)
    _esperados = k;
    _conectados = 0;
    _consultoEmpezar = vector<bool>(k,false);       //O(k)
    _repo = repositorio;                            //O(1)
    //configuracion no esta pero nunca lo usamos
}

void Servidor::conectarCliente() {

    _notificaciones[_conectados].push(Notificacion::nuevaIdCliente(_conectados)); //
    _conectados ++;

    if (_conectados == _esperados){
        /* Tuvimos que agregar esto en relacion al  TP1 porque cuando los jugadores son iguales a los conectados
         * se tienen que enviar las notificaciones correspondientes. Es complejidad O(K) porque a cada jugador se le
         * agregar 3 notificaciones,que son O(1)
         * */
        for (int i = 0; i < _esperados  ; i++){
            _notificaciones[i].push(Notificacion::nuevaEmpezar(get<0>(_var)));
            _notificaciones[i].push(Notificacion::nuevaTurnoDe(0));
            _notificaciones[i].push(Notificacion::nuevaReponer(_juego.respuestas(i)));
        }
    }
}


list<Notificacion> Servidor::consultar(IdCliente cid) {
    list<Notificacion> notDevueltas;
    /* Como se inicializa el juego en conectar con sus respectivas notificaciones, no hay que agregar una notificacion
     * de empezar, sino que directamente se pasa la cola de notificaciones a una lista
    if (!_notificaciones[cid].empty()){
        notDevueltas.push_back(_notificaciones[cid].front());
        _notificaciones[cid].pop();
    } Con la nueva complejidad, se pone el empezar en conectar cliente
    if (_conectados == _esperados){
        if (!consultoEmpezar(cid)){
            //notDevueltas.push_back(Notificacion::nuevaEmpezar(get<0>(_var)));
            _consultoEmpezar[cid] = true;
           // notDevueltas.push_back(Notificacion::nuevaReponer(_juego.respuestas(cid)));
            //notDevueltas.push_back(Notificacion::nuevaTurnoDe(_juego.turno()));
        }

    } */
    while (!_notificaciones[cid].empty()){
        notDevueltas.push_back(_notificaciones[cid].front());
        _notificaciones[cid].pop();
    }
    return notDevueltas;
}

int Servidor::conectados() {
    return _conectados;
}// aca no hay q iniciar el  j
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
    //return make_tuple(variante, _repo);
}



Servidor::Servidor( const Servidor &aCopiar (int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>> const &variante, queue<Letra> &repositorio)) : _juego(Juego(
        this->_esperados,this->_var, this->_repo)){

    *this = aCopiar;
}

void Servidor::recibirMensaje(IdCliente cid, Ocurrencia o) {
    /* Hay cambios respecto al TP1:
     * -Se agrego tieneTodasFichas para que devuelva si la jugada no es valida si el jugador no
     * tiene las fichas que dice en la ocurrencia,
     * - Se sacaron del for puntajeViejo, fichasViejas,  _turno porque son variables que no cambian y no hace falta
     * llamar a esas funciones en cada repeticion. Tambien el ubicar se saco del for porque no cumplia la precondicion
     * ya que si se vuelven a ubicar las fichas, no seria una jugada valida.
     * Tambien se agrega la notifiacion mal si se quiere jugar pero el juego no empezó.
     * No se usa la funcion fichasRespuestas sino que se agregó en juego un vector de long
     * cantJugadores donde estan las fichas que se reponen.
     * */
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

Servidor::~Servidor() {

}







