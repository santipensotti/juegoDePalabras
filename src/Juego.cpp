#include "Juego.h"
#include "Letra.h"
Juego::Juego(int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>,map<Letra, Nat>,set<vector<Letra>>> const
        &variante , std::queue<Letra> &repositorio) {
    /* Se agrega _fichasReponer  donde se guardan las fichas que recibió un jugador en caso de que se inicia el juego o
     * luego de ubicar una ocurrencia. Tiene long k por eso no afecta la complejidad
     * Se cambió la longitud del vector de cada jugador de fichasJugadores a 256 por como estan representadas las letras
     * en c++, como es una constante en O(1), se respeta la complejidad
     *
     */
    varv2 = variante;
    int n = get<0>(varv2);
    _tablero = vector<vector<tuple<bool , Letra , Nat >>>(n,vector<tuple<bool , Letra , Nat >>(n,{false,'a',0}));
    _turno = 0;
    _puntajePorSumar = vector<std::queue<std::tuple<int,int,Letra,int>>>(k,queue<std::tuple<int,int,Letra,int>>());
    _fichasJugadores = vector<vector<Nat>>(k, vector<Nat>(256,0));

    _fichasReponer = vector<multiset<Letra>> (k);
    for (int i = 0; i < k; i++){  //fichas de cada jugador
        for (int j = 0; j < get<1>(varv2); j ++){
            _fichasJugadores[i][repositorio.front()] ++; //desencolo cantidad get<1>(varv2) las fichas y las reparto
            Letra l = repositorio.front();
            _fichasReponer[i].insert(l);
            repositorio.pop();

        }
    }
    _puntaje = vector<Nat> (k,0);
    while(!repositorio.empty()){
        _repositorio.push(repositorio.front());
        repositorio.pop();
    }
    _puntajePorSumar = vector<std::queue<std::tuple<int,int,Letra,int>>>(k,queue<std::tuple<int,int,Letra,int>>());
}


Nat& Juego::puntaje(int cid) {
    /*
     *Se agrego para poder ver si hay un cambio de turno en la fichas que estan por sumar en un jugador. Se sacaron
     * las funciones que sumaban  el puntaje de las letras que estaban en el tablero y se reutilizan las mismas
     * funciones que suman el puntaje de las palabras, usandolas de manera inversa si es vertical se toma horizontal y
     * restando el valor de la letra en cuestión
     * */
    queue<tuple<int, int, Letra, int>> letra = _puntajePorSumar[cid];
    queue<tuple<int, int, Letra, int>> palabra = _puntajePorSumar[cid];
    queue<tuple<int, int, Letra, int>> siguiente = _puntajePorSumar[cid];

    tuple<int, int, Letra, int> anterior = palabra.front();
    palabra.pop();
    tuple<int, int, Letra, int> actual = palabra.front();

    bool avanzaVertcal = false;


    bool cambioTurno = true;
    palabra = letra;

    while (!palabra.empty()) {
        siguiente.pop();
        actual = palabra.front();
        if (!siguiente.empty() and  get<3>(anterior) != get<3>(actual)){
            cambioTurno = true;
        }
        if (cambioTurno){
            if (get<1>(actual) == get<1>(siguiente.front())){
                avanzaVertcal = true;
            }else{
                avanzaVertcal = false;
            }
            if (avanzaVertcal){
                _puntaje[cid] = _puntaje[cid] +puntajePorPalabraFormadaVertical(get<0>(palabra.front()),
                        get<1>(palabra.front()), get<2>(palabra.front()));
            }else{
                _puntaje[cid] = _puntaje[cid] +puntajePorPalabraFormadaHorizontal(get<0>(palabra.front()),
                        get<1>(palabra.front()), get<2>(palabra.front()));
            }
            cambioTurno = false;
        }

        if (get<3>(anterior) != get<3>(palabra.front())){
            if (avanzaVertcal) { //fijarse pq hay diferencias con la parte de disenio
                Nat punt = puntajePorPalabraFormadaVertical(get<0>(palabra.front()), get<1>(palabra.front()),
                                                            get<2>(palabra.front()));
                _puntaje[cid] = _puntaje[cid] +puntajePorPalabraFormadaVertical(get<0>(palabra.front()), get<1>(palabra.front()),
                                                                                get<2>(palabra.front()));
            } else {
                _puntaje[cid] = _puntaje[cid] +puntajePorPalabraFormadaHorizontal(get<0>(palabra.front()), get<1>(palabra.front()),
                                                                                  get<2>(palabra.front()));
            }
        }
        anterior = palabra.front();
        palabra.pop();

    }
    siguiente = letra;
    anterior = letra.front();
    cambioTurno = true;
    while (!letra.empty()){
        siguiente.pop();
        actual = letra.front();
        if (get<3>(anterior) != get<3>(actual)){
            cambioTurno = true;
        }
        if (cambioTurno){
            if (get<1>(actual) == get<1>(siguiente.front())){
                avanzaVertcal = true;
            }else{
                avanzaVertcal = false;
            }
            cambioTurno = false;
        }
        if (avanzaVertcal){
            _puntaje[cid] += puntajePorPalabraFormadaHorizontal(get<0>(letra.front()), get<1>(letra.front()),get<2>(letra.front()))
                            ; //Diferente al latex se reutiliza la funcion de sumar el puntaje
            // restando el valor de la letra que se ve el puntaje para no sumar repetidos
        }
        else{
            _puntaje[cid] += puntajePorPalabraFormadaVertical(get<0>(letra.front()), get<1>(letra.front()),get<2>(letra.front()))
                             ; //Diferente al latex se reutiliza la funcion de sumar el puntaje
        }


        letra.pop();

    }

    _puntajePorSumar[cid] = {};
    return _puntaje[cid];
}

Nat Juego::puntajePorPalabraFormadaVertical(int i, int j, int turno){
    int k = i+1;
    Nat puntajeParcial = 0;
    bool continuar = true;
    while (get<2>(_tablero[i][j]) <= turno && get<0>(_tablero[i][j]) and continuar){
        Nat letra = get<1>(_tablero[i][j]);

        puntajeParcial += get<2>(varv2)[get<1>(_tablero[i][j])];
        vector<Nat> p = get<2>(varv2);

        Nat puntos = p[letra];
        if (i==0){
            continuar = false;
        }else{
            i--;
        }
    }
    continuar = true;//En el latex aparece q es el elemento 2 de la tupla el bool
    while (get<2>(_tablero[k][j]) <= turno && get<0>(_tablero[k][j]) and  continuar){ //En el latex aparece tablero[i][j], pero creo q esta mal CREO
        puntajeParcial += get<2>(varv2)[get<1>(_tablero[k][j])];
        if (k ==0){
            continuar = false;
        }else{
            k++;
        }
    }
    return puntajeParcial;
}

Nat Juego::puntajePorPalabraFormadaHorizontal(int i, int j, int turno) {
    int k = j + 1;
    bool continuar = true;
    Nat puntajeParcial = 0;
    while (get<2>(_tablero[i][j]) <= turno && get<0>(_tablero[i][j]) and continuar){
        puntajeParcial += get<2>(varv2)[get<1>(_tablero[i][j])];
        if (j==0){
            continuar = false;
        }else{
            j--;
        }

    }
    while (get<2>(_tablero[i][k]) <= turno && get<0>(_tablero[i][k])){
        puntajeParcial += get<2>(varv2)[get<1>(_tablero[i][k])];

        k++;
    }
    return puntajeParcial;
}






int& Juego::turno() {
    int res = _turno % _puntaje.size();
    return _turno;
}

char Juego::contenidoTablero(int x, int y) {
    return get<1>(_tablero[x][y]);
}


Nat Juego::cantLetra(IdCliente id, Letra l) {
    return _fichasJugadores[id][l];
}


bool Juego::hayLetra(Nat x, Nat y) {
    return get<0>(_tablero[x][y]);
}

bool Juego::jugadaValida(Ocurrencia o) {
    if (o.size() ==0 ){
        return  true;
    }
    int max = get<3>(varv2).lMax();
    bool formaVertical;
    if (o.size() > max){
        return false;
    }
    std::tuple<bool, list<tuple<Nat,Nat,Letra>>, bool> cumpleOc = cumpleOcurrencia(o);
    if  (!get<0>(cumpleOc))
    {
        return false;
    }
    bool esVertical = get<2>(cumpleOc);
    list<tuple<Nat,Nat,Letra>> listaOcurrencia = get<1>(cumpleOc);
    list<tuple<Nat,Nat,Letra>>  ocurrenciaOrdenada = listaOcurrencia;
    cumpleOc = completarEspaciosOcurrencia(ocurrenciaOrdenada,esVertical);
    ocurrenciaOrdenada = get<1>(cumpleOc);
    if  (!get<0>(cumpleOc))
    {
        return false;
    }
    std::tuple<bool, list<tuple<Nat,Nat,Letra>>> palabraTerminada = completarEspaciosAtrasAdelante(ocurrenciaOrdenada,esVertical);
    ocurrenciaOrdenada = get<1>(palabraTerminada);
    if  (!get<0>(palabraTerminada))
    {
        return false;
    }
    Palabra potencialPalabra(ocurrenciaOrdenada.size());
    list<tuple<Nat,Nat,Letra>>::iterator itr;
    int i = 0;
    for (itr = ocurrenciaOrdenada.begin(); itr != ocurrenciaOrdenada.end(); itr++) {
        potencialPalabra[i] = get<2>(*itr);
        i++;
    }

    if (get<3>(varv2).pertenece(potencialPalabra) ){
        return  true;
    }
    return false;
}




std::tuple<bool, list<tuple<Nat,Nat,Letra>>, bool> Juego::cumpleOcurrencia(Ocurrencia o) {
    std::list<std::tuple<Nat,Nat,Letra>> listaOcurrencia;
    tuple<Nat,Nat,Letra> lista [o.size()];
    set<tuple<Nat,Nat,Letra>>::iterator itr;
    bool formaVertical;
    int v = 0;
    int h = 0;
    std::tuple<int,int,Letra> actual;
    for (itr = o.begin(); itr != o.end(); itr++) {
        std::list<Letra> potencialPalabraLetraUbicada;
        if (get<0>(_tablero[get<0>(*itr)][get<1>(*itr)]))
            return make_tuple(false,listaOcurrencia, false);
        actual = *itr;
        itr++;
        if (itr != o.end()){
            if (get<0>(actual) != get<0>(*itr) and get<1>(actual) != get<1>(*itr) ){
                return make_tuple(false,listaOcurrencia, false);
            }
            if (get<1>(*itr) == get<1>(actual)){
                formaVertical = true;
                v++;

            }else{
                formaVertical = false;
                h++;
            }
        }else{
            h++;
            v++;
        }

        itr--;

        int j=1;
        int i = 1;
        potencialPalabraLetraUbicada.push_front(get<2>(*itr));
        if (formaVertical){
             lista[v-1] = *itr;
             while(get<0>(*itr)-i > -1 and get<0>(_tablero[get<0>(*itr)-i][get<1>(*itr)])){ //Nunca va a entrar porque la posicion del tablero
                 //nucnca esta ocupada
                 potencialPalabraLetraUbicada.push_front(get<1>(_tablero[get<0>(*itr)-i][get<1>(*itr)]));
                 i++;
             }

            while(get<0>(*itr) +j < get<0>(varv2) +1 and get<0>(_tablero[get<0>(*itr)+j][get<1>(*itr)])){
                potencialPalabraLetraUbicada.push_back(get<1>(_tablero[get<0>(*itr)+j][get<1>(*itr)]));
                j++;
            }
        }else{
            lista[h-1] = *itr;
            int number =get<0>(*itr);
            while(number -i > -1 and get<0>(_tablero[get<0>(*itr)-i][get<1>(*itr)])){
                potencialPalabraLetraUbicada.push_front(get<1>(_tablero[get<0>(*itr)-i][get<1>(*itr)]));
                i++;
                number =get<0>(*itr);

            }

            while(get<1>(*itr) +j < get<0>(varv2) +1 and get<0>(_tablero[get<0>(*itr)][get<1>(*itr)+j])){
                potencialPalabraLetraUbicada.push_back(get<1>(_tablero[get<0>(*itr)][get<1>(*itr)+j]));
                j++;
            }
        }
        vector<Letra> potencialPalabra;
        for (auto i:potencialPalabraLetraUbicada ){
            potencialPalabra.push_back(i);
        }
        // potencialPalabra = list2Array(potencialPalabraLetraUbicada);
        if (!get<3>(varv2).pertenece(potencialPalabra) and potencialPalabra.size() > 1){
            return make_tuple(false,listaOcurrencia, false);
        }

    }
    if (h!= o.size() and v!= o.size() and o.size() > 1){
        return make_tuple(false,listaOcurrencia, false);
    }
   //Pasar de lista array a lista enlazada o ver como hacer que una funcion devuelva array
    for (tuple<Nat,Nat,Letra> i : lista){
        listaOcurrencia.push_back(i);
    }
    return make_tuple(true,listaOcurrencia,formaVertical);
}

list<tuple<Nat,Nat,Letra>> Juego::ordenar(list<tuple<Nat,Nat,Letra>> o, bool) {

    return o;
}

std::tuple<bool, list<tuple<Nat,Nat,Letra>>, bool> Juego::completarEspaciosOcurrencia(std::list<tuple<Nat,Nat,Letra>> o,bool esVertical) {
    bool formaVertical;
    if (o.size() < 2){
        return make_tuple(true,o,true);
    }
    list<tuple<Nat,Nat,Letra>>::iterator it = o.begin();
    while (it != o.end()) {
        tuple<Nat, Nat, Letra> actual = *it;
        it++;
        if (!esVertical){
            if (get<1>(actual)+1 != get<1>(*it) and it!=o.end()){
                if (!get<0>(_tablero[get<0>(actual)][get<1>(actual) + 1])){
                    return make_tuple(false,o, false);
                } else{
                    it++;
                    tuple<Nat, Nat, Letra> value ; //Tiene que ser el el elemento del tablero siguiente
                    o.insert(it, make_tuple(get<0>(actual), get<1>(actual)+1,
                            get<1>(_tablero[get<0>(actual)][get<1>(actual)+1])));
                }
            }
        }else{
            if (get<0>(actual)+1 != get<0>(*it) and it!=o.end()){
                if (!get<0>(_tablero[get<0>(actual)+1][get<1>(actual)])){
                    return make_tuple(false,o, false);
                } else{
                    it++;
                    tuple<Nat, Nat, Letra> value ; //Tiene que ser el el elemento del tablero siguiente
                    o.insert(it, make_tuple(get<0>(actual)+1, get<1>(actual),
                            get<1>(_tablero[get<0>(actual)+1][get<1>(actual)])));
                }
            }
        }
    }
    return make_tuple(true,o,esVertical);
}

std::tuple<bool, list<tuple<Nat, Nat, Letra>>>
Juego::completarEspaciosAtrasAdelante(list<tuple<Nat, Nat, Letra>> o, bool esVertical) {
    if(!esVertical){
        while (get<1>(o.front()) > 0 and get<0>(_tablero[get<0>(o.front())][get<1>(o.front())-1])){
            if (o.size() > get<3>(varv2).lMax()){
                return make_tuple(false,o);
            }else{
                o.push_front(make_tuple(get<0>(o.front()), get<1>(o.front())-1, get<1>
                        (_tablero[get<0>(o.front())][get<1>(o.front())-1])));
            }
        }
        while (get<1>(o.front())+1 < get<0>(varv2) and get<0>(_tablero[get<0>(o.front())][get<1>(o.front())+1])){
            if (o.size() > get<3>(varv2).lMax()){
                return make_tuple(false,o);
            }else{
                o.push_back(make_tuple(get<0>(o.front()), get<1>(o.front())+1, get<1>
                        (_tablero[get<0>(o.front())][get<1>(o.front())+1])));
            }
        }
    }else{
        Nat frente = get<0>(o.front());
        frente --;
        while (get<0>(o.front()) > 0 and get<0>(_tablero[get<0>(o.front())-1][get<1>(o.front())])){
            if (o.size() > get<3>(varv2).lMax()){
                return make_tuple(false,o);
            }else{
                o.push_front(make_tuple(get<0>(o.front())-1, get<1>(o.front()), get<1>
                        (_tablero[get<0>(o.front())-1][get<1>(o.front())])));
            }
        }
        while (get<0>(o.front())+1 < get<0>(varv2)  and get<0>(_tablero[get<0>(o.front())+1][get<1>(o.front())])){
            if (o.size() > get<3>(varv2).lMax()){
                return make_tuple(false,o);
            }else{
                o.push_back(make_tuple(get<0>(o.front())+1, get<1>(o.front()), get<1>
                        (_tablero[get<0>(o.front())+1][get<1>(o.front())+1])));
            }
        }
    }
    return make_tuple(true,o);
}


void Juego::ubicar(Ocurrencia o) {
    Ocurrencia::iterator it;

    int turnoDe = _turno % _puntaje.size();
    _fichasReponer[turnoDe].clear();
    for (it = o.begin(); it != o.end(); it++){
        tuple<Nat, Nat, Letra> siguiente = *it;
        _tablero[get<0>(siguiente)][get<1>(siguiente)] = make_tuple(true, get<2>(siguiente), _turno);
        _fichasJugadores[turnoDe][get<2>(siguiente)] --;
    }

    Ocurrencia ::iterator itr = o.begin();
    while (itr!= o.end()){
        _fichasJugadores[turnoDe][_repositorio.front()]++;
        _fichasReponer[turnoDe].insert(_repositorio.front());
        _repositorio.pop();

        _puntajePorSumar[turnoDe].push(make_tuple(get<0>(*itr), get<1>(*itr), get<2>(*itr),_turno));
        itr++;
    }
    _turno ++;
    //puntaje(turnoDe);

}

vector<Nat> Juego::fichasJugador(IdCliente cid) {
    return _fichasJugadores[cid];
}

const tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>, map<Letra, Nat>, set<vector<Letra>>>& Juego::variante() {
    return varv2;
}

Letra Juego::letraEn(Nat x, Nat y) {
    return get<1>(_tablero[x][y]);
}

Juego::Juego(const Juego &aCopiar(int k, tuple<Nat, Nat, vector<Nat>, conjDigital<Palabra>> const &variante , std::queue<Letra> &repositorio)) {
    *this = aCopiar;
}
Juego &Juego::operator=(const Juego &d) {
    this->_tablero = d._tablero;
    this->_puntaje = d._puntaje;
    this->_fichasJugadores = d._fichasJugadores;
    this->_puntajePorSumar = d._puntajePorSumar;
    this->_puntaje = d._puntaje;
    this->_turno = d._turno;
    this->varv2 = d.varv2;
    return *this;
}

multiset<Letra> Juego::respuestas(IdCliente cid) {
    multiset<Letra> res = _fichasReponer[cid];
    _fichasReponer[cid].clear();
    return res;
}

Juego::~Juego() {

}



