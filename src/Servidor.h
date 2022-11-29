#ifndef TP_JUEGODEPALABRAS_SERVIDOR_H
#define TP_JUEGODEPALABRAS_SERVIDOR_H
using namespace std;
#include "Juego.h"
#include "Notificacion.h"
#include "Tipos.h"

class Servidor {
public:
    Servidor(int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>,map<Letra, Nat>,set<vector<Letra>>> const &variante, queue<Letra> &repositorio);
    ~Servidor();

    void conectarCliente();
    Servidor(const Servidor&  aCopiar(int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>> const &variante, queue<Letra> &repositorio));
    Servidor& operator= (const Servidor& aCopiar);
    list<Notificacion> consultar(IdCliente cid);
    int conectados();
    int esperados();
    vector<queue<Notificacion>> notificaciones;
    bool consultoEmpezar(IdCliente cid);
    Juego juegoActual();
    void recibirMensaje(IdCliente cid, Ocurrencia o);

     tuple<tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>>, Repositorio> configuracion();

private:
    Nat _esperados;
    Nat  _conectados;
    Juego _juego;
    vector<queue<Notificacion>> _notificaciones;
    queue<Letra> _repo;
    vector<bool> _consultoEmpezar;
    tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>,map<Letra, Nat>,set<vector<Letra>>> _var;
   // tuple<tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>, Repositorio> _configuracion;
   multiset<Letra> fichasRepuestas;
   conjDigital<Palabra> destruirConj(conjDigital<Palabra>);

};


#endif //TP_JUEGODEPALABRAS_SERVIDOR_H



