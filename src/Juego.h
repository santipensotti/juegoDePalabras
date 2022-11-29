
#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H
using namespace std;
#include <queue>
#include "Tipos.h"
#include "conjDigital.h"

class Juego{
public:
    Juego(int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>,map<Letra, Nat>,set<vector<Letra>>> const &variante , std::queue<Letra> &repositorio);
    ~Juego();
    const tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>, map<Letra, Nat>, set<vector<Letra>>>& variante();
    Nat& puntaje(int cid);

    int& turno();

    Juego(const Juego& aCopiar (int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>> const &variante, queue<Letra> &repositorio));

    Juego& operator= (const Juego& aCopiar);

    Letra contenidoTablero(int x, int y);

    Nat cantLetra(IdCliente id, Letra l);

    vector<Nat> fichasJugador(IdCliente cid);

    bool hayLetra(Nat x, Nat y);

    bool jugadaValida(Ocurrencia o);
     //Aca se estaba pasando la ocurrencia por referencia const cuando el latex dice por copia
    void ubicar(Ocurrencia o);

    Letra  letraEn(Nat x, Nat y);//hay q fijarse q las funciones auxiliares se pasen por copia o por referencia

    multiset<Letra> respuestas(IdCliente cid); // No esta en latex pero para acomodar notificacion en recibir mensaje

private:
    int _turno;
    queue<Letra> _repositorio;
    vector<Nat> _puntaje;
    tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>, map<Letra, Nat>, set<vector<Letra>>> varv2;
    vector<vector<std::tuple<bool , Letra , Nat >>> _tablero;
    vector<vector<Nat>> _fichasJugadores;
    vector<queue<tuple<int,int,Letra,int>>> _puntajePorSumar;
    vector<multiset<Letra>> _fichasReponer; //Necesario para las notificaciones de reponer, no esta en latex


    tuple<bool,list<tuple<Nat,Nat,Letra>>, bool> cumpleOcurrencia(Ocurrencia o);
    list<tuple<Nat,Nat,Letra>> ordenar(list<tuple<Nat,Nat,Letra>>, bool );
    tuple<bool,list<tuple<Nat,Nat,Letra>>, bool> completarEspaciosOcurrencia(std::list<tuple<Nat,Nat,Letra>> o , bool );
    tuple<bool,list<tuple<Nat,Nat,Letra>>> completarEspaciosAtrasAdelante(list<tuple<Nat,Nat,Letra>> o,bool );

    Nat puntajePorPalabraFormadaVertical(int i, int j, int turno);
    Nat puntajePorPalabraFormadaHorizontal(int i, int j, int turno);
    Nat puntajePorLetraVertical(int i, int j, int turno);
    Nat puntajePorLetraHorizontal(int i, int j, int turno);

};
#endif //TP_JUEGODEPALABRAS_JUEGO_H
