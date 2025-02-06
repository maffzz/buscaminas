
#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;
using namespace chrono;

class cPartida {
private:
    string mNombre;
    int mTamanio;
    int mBombas;
    vector<int> mX_bombas;
    vector<int> mY_bombas;
    int **mTablero;
    int **mPrintTablero;
    bool mContinuarPartida;
    int mBanderasColocadas;
    time_point<high_resolution_clock> mStart; 
    time_point<high_resolution_clock> mEnd;
    double mTiempo;

public:
    cPartida (int i, string Nombre) {
        mNombre = Nombre;
        if (i == 1) {mTamanio = 6; mBombas = 3 + rand() % 3;}
        else if (i == 2) {mTamanio = 8; mBombas = 8 + rand() % 5;}
        else if (i == 3) {mTamanio = 10; mBombas = 15 + rand() % 6;}

        mTablero = new int*[mTamanio];
        mPrintTablero = new int*[mTamanio];
        for (int e = 0; e < mTamanio; e++){
            mTablero[e] = new int[mTamanio];
            mPrintTablero[e] = new int[mTamanio];}

        for (int u = 0; u < mTamanio; u++) {
            for (int j = 0; j < mTamanio; j++) {
                mTablero[u][j] = 0;
                mPrintTablero[u][j] = -3;}}
    }

    void Iniciarjuego () {
        cout << "     GAME START ╰(*°▽°*)╯    " << endl << endl ;
        mStart = high_resolution_clock::now();

        ImprimirPrintTablero();
        cout << endl;
        int x, y;
        do {
            cout <<"→ ingrese la fila de la primera jugada: " ;
            cin >> x;
            if (cin.fail()){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                x = -1;}
            else{x -= 1;}
        } while (x < 0 || x >= mTamanio);

        do {
            cout <<  "→ ingrese la columna de la primera jugada: " ;
            cin >> y;
            if (cin.fail()){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                y = -1;}
            else {y -= 1;}
        } while (y < 0 || y >= mTamanio || typeid(y) != typeid(int));

        LlenarBombas(x,y);
        ColocarNumEnTablero();
        Jugada(x, y);
        Continuar_Jugando();
    }

    void ImprimirPrintTablero () {
    cout << endl << endl << endl;
    if (mContinuarPartida){
        cout << " ─  ─  ─  ─  ─ ୨ৎ ─  ─  ─  ─  ─" << endl;
        cout << "   Minas: " << mBombas << endl;
        cout << "   Banderas: " << mBanderasColocadas << "/" << mBombas << endl;
        cout << endl;}

    cout << "    ";
    for (int i = 0; i < mTamanio; i++) {cout << setw(3) << i + 1;}
    cout << endl;
    cout << "   ╭";
    for (int i = 0; i < mTamanio; i++) {cout << "───";}
    cout << "─" << endl;

    for (int i = 0; i < mTamanio; i++) {
        cout << setw(2) << i + 1 << " |"; // índice de filas
        for (int j = 0; j < mTamanio; j++) {
            if (mPrintTablero[i][j] == -3) {
                cout << "  " << "▢";}
            else if (mPrintTablero[i][j] == -1) { cout << "  " << "\033[1m\033[31mB\033[0m";}
            else if (mPrintTablero[i][j] == -2 && !mContinuarPartida) { cout << "  " << "★";}
            else {
                if (mPrintTablero[i][j] == 0) { cout << "  " << "\033[90m0\033[0m";} // gris
                if (mPrintTablero[i][j] == 1) { cout << "  " << "\033[34m1\033[0m";} // azul
                if (mPrintTablero[i][j] == 2) { cout << "  " << "\033[32m2\033[0m";} // verde
                if (mPrintTablero[i][j] == 3) { cout << "  " << "\033[95m3\033[0m";} // rosado
                if (mPrintTablero[i][j] == 4) { cout << "  " << "\033[0;33m4\033[0m";} // naranja
                if (mPrintTablero[i][j] == 5) { cout << "  " << "\033[33m5\033[0m";} // amarillo
                if (mPrintTablero[i][j] == 6) { cout << "  " << "\033[31m6\033[0m";} // rojo
                if (mPrintTablero[i][j] == 7) { cout << "  " << "\033[95m7\033[0m";} // celeste
                if (mPrintTablero[i][j] == 8) { cout << "  " << "\033[33m8\033[0m";} // azul
                if (mPrintTablero[i][j] == 9) { cout << "  " << "\033[33m9\033[0m";} // negro
        }} cout << endl;}}

    void LlenarBombas (int x1, int y1) {
        srand(time(nullptr));
        int bombas_colocadas = 0;

        while (bombas_colocadas < mBombas) {
            int x = rand() % mTamanio;
            int y = rand() % mTamanio;
            bool noAlrededores = !(x >= x1 - 1 && x <= x1 + 1 && y >= y1 - 1 && y <= y1 + 1);

            if (mTablero[x][y] != -2 && noAlrededores) {
                mTablero[x][y] = -2;
                mX_bombas.push_back(x);
                mY_bombas.push_back(y);
                bombas_colocadas++;}}}

    void ColocarNumEnTablero () {
        for(int i = 0; i < mBombas; i++) {
            int x = mX_bombas[i];
            int y = mY_bombas[i];
            if(x-1<mTamanio && x-1>=0 && y-1<mTamanio && y-1>=0 && (mTablero[x-1][y-1]!=-2)){mTablero[x-1][y-1] += 1;}
            if(x-1<mTamanio && x-1>=0 && y<mTamanio && y>=0 && (mTablero[x-1][y]!=-2)){mTablero[x-1][y] += 1;}
            if(x-1<mTamanio && x-1>=0 && y+1<mTamanio && y+1>=0 && (mTablero[x-1][y+1]!=-2)){mTablero[x-1][y+1] += 1;}
            if(x<mTamanio && x>=0 && y-1<mTamanio && y-1>=0 && (mTablero[x][y-1]!=-2)){mTablero[x][y-1] += 1;}
            if(x<mTamanio && x>=0 && y+1<mTamanio && y+1>=0 && (mTablero[x][y+1]!=-2)){mTablero[x][y+1] += 1;}
            if(x+1<mTamanio && x+1>=0 && y-1<mTamanio && y-1>=0 && (mTablero[x+1][y-1]!=-2)){mTablero[x+1][y-1] += 1;}
            if(x+1<mTamanio && x+1>=0 && y<mTamanio && y>=0 && (mTablero[x+1][y]!=-2) ) {mTablero[x+1][y] += 1;}
            if(x+1<mTamanio && x+1>=0 && y+1<mTamanio && y+1>=0 && (mTablero[x+1][y+1]!=-2)) {mTablero[x+1][y+1] += 1;}}}

    int PedirCoordenada(const string &mensaje) {
    int coord;
    do {
        cout << mensaje;
        cin >> coord;
        if (coord == 100){
            return coord;}
        else if (cin.fail()){
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                coord = -10;}
        else {coord -= 1;}
    } while (coord <= -1 || coord >= mTamanio ); 
    return coord;}

    void Jugada (int x, int y) {
        if (mTablero[x][y] == -2) {
            MedirTiempo();
            MostrarBombas();
            mContinuarPartida = false;
            ImprimirPrintTablero();
            cout << endl << "   ¡boom! has encontrado una bomba ╚(x . x)╝" << endl;
            cout << "  ╭════════════════════════════════════════╮" << endl;
            cout << "  |            GAME OVER (╥﹏╥)            |" <<  endl;
            cout << "  |          perdiste en " << mTiempo << "s"  << endl;
            cout << "  ╰════════════════════════════════════════╯" << endl;
        }

        //Bandera
        else if (mTablero[x][y] == -1) { // si es una bandera
                cout << "Aquí hay una bandera. Quítala para descubrir la zona ∧,,,∧" << endl;
                mContinuarPartida = true;} 

        //Número
        else{
            if (mTablero[x][y] == 0) {DespejarZona(x, y);}  // si la celda seleccionada es un 0, despejar la zona
            else { // de lo contrario, revelar solo la celda seleccionada
                mPrintTablero[x][y] = mTablero[x][y];}
                ImprimirPrintTablero();}}

    void Continuar_Jugando() {
        while (mContinuarPartida) {
            int x,y;
            if (VerificarVictoria()) {
                cout << endl;
                MedirTiempo();
                GuardarResultado();

                cout << "╭══════════════•   •✧๑♡๑✧•   •═══════════════╮" << endl;
                cout << "|                                            |" << endl;
                cout << "|         ¡felicidades, " <<  mNombre  << "!" << endl;
                cout << "|      has ganado el juego en " << mTiempo <<  "s"  <<  endl;
                cout << "|              ٩(๑･ิᴗ･ิ)۶٩(･ิᴗ･ิ๑)۶              |" << endl;
                cout << "|                                            |" << endl;
                cout << "╰══════════════•   •✧๑♡๑✧•   •═══════════════╯" << endl;
                cout << endl << endl<< " ⋆.˚✮ regresando al menú... "  << endl << endl;
                this_thread::sleep_for(chrono::seconds(3));
                break;}

            char accion;
            cout <<  endl;
            cout << "   ☆ jugada(j)" << endl;
            cout << "   ☆ colocar bandera(c)" << endl;
            cout << "   ☆ quitar bandera(q)" << endl;
            cout << "   ☆ salir(s)" << endl;
            cout << "   opción: " ;
            cin >> accion;

            if (accion == 'c') {
                x = PedirCoordenada("   → ingrese la fila: ");
                if (x == 100){Continuar_Jugando(); break;}

                y = PedirCoordenada("   → ingrese la columna: ");
                if (y == 100){Continuar_Jugando(); break;}

                ColocarBandera(x, y);
                ImprimirPrintTablero();}

            else if (accion == 'q') {
                x = PedirCoordenada("   → ingrese la fila: ");
                if (x == 100){Continuar_Jugando(); break;}

                y = PedirCoordenada("   → ingrese la columna: ");
                if (y == 100){Continuar_Jugando(); break;}

                QuitarBandera(x, y);
                ImprimirPrintTablero();}

            else if (accion == 'j') {
                x = PedirCoordenada("   → ingrese la fila: ");
                if (x == 100){Continuar_Jugando(); break;}

                y = PedirCoordenada("   → ingrese la columna: ");
                if (y == 100){Continuar_Jugando(); break;}

                Jugada(x, y);}

            else if (accion == 's') {
                mContinuarPartida = false;
                MedirTiempo();}

            else { cout << endl << "opción inválida." << endl << endl;}

            if (!mContinuarPartida) {
                cout << endl << endl<< " ⋆.˚✮ regresando al menú... "  << endl << endl;
                this_thread::sleep_for(chrono::seconds(1));
                break;}
        }
    }

    void DespejarZona(int x, int y) {
        if (x < 0 || x >= mTamanio || y < 0 || y >= mTamanio) return;
        if (mPrintTablero[x][y] != -3 || mTablero[x][y] == -2) return;
        mPrintTablero[x][y] = mTablero[x][y];
        if (mTablero[x][y] != 0)return;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int newX = x + dx;
                int newY = y + dy;
                if ((dx != 0 || dy != 0) && newX >= 0 && newX < mTamanio && newY >= 0 && newY < mTamanio) {
                    DespejarZona(newX, newY);}}}}

    bool VerificarVictoria() {
        for (int i = 0; i < mTamanio; i++) {
            for (int j = 0; j < mTamanio; j++) {
                if (mPrintTablero[i][j] == -3 && mTablero[i][j] != -2) {
                    return false;}}}
        return true;
    }

    void ColocarBandera (int x, int y) {
        if (mPrintTablero[x][y] == -1) {
            cout << endl << "  ⚠️  ADVERTENCIA: ya hay una bandera en la posición (" << x + 1 << "," << y + 1 << ")" << endl;}

        else if (mPrintTablero[x][y] == -3) {
            mPrintTablero[x][y] = -1;
            mBanderasColocadas++;
            cout << endl <<  "   ☑ bandera colocada en (" << x + 1 << ", " << y + 1 << ")." << endl;}

        else if ((mPrintTablero[x][y]!=-1) && (mPrintTablero[x][y]!=-2) && (mPrintTablero[x][y]!=-3)) {
            cout <<  endl <<  "  ⚠️  ADVERTENCIA: no se puede colocar una bandera en una celda ya descubierta" << endl;}
    }

    void QuitarBandera (int x, int y) {
        if (mPrintTablero[x][y] == -1) {
            mPrintTablero[x][y] = -3;
            mBanderasColocadas--;
            cout << endl <<  "  ☑ bandera quitada de (" << x + 1 << "," << y + 1 << ")" << endl;}
        else {cout << endl << "  ⚠️  ADVERTENCIA: no hay una bandera en (" << x + 1 << "," << y + 1 << ")" << endl;}}

    void MostrarBombas() {
        for (int x=0; x<mTamanio; x++){
            for (int y=0; y<mTamanio; y++){
                if (mTablero[x][y] == -2){
                    mPrintTablero[x][y]=-2;}}}}

    void MedirTiempo() {
        mEnd = high_resolution_clock::now();
        duration<double> elapsed = mEnd - mStart;
        mTiempo = elapsed.count();
        mTiempo = round(mTiempo*100) /100;        
    }

    string GetDificultad() const {
        if (mTamanio == 6) return "facil.txt";
        else if (mTamanio == 8) return "medio.txt";
        else if (mTamanio == 10) return "dificil.txt";
        else return "error";}

    void GuardarResultado() {
        string archivoNombre = GetDificultad();
        ofstream archivo(archivoNombre, ios::app);
        if (archivo.is_open()) {
            archivo << mNombre << " " << mTiempo << endl;
            archivo.close();
        } else {
            cerr << "no se pudo abrir el archivo " << archivoNombre << " para guardar los resultados." << endl;
        }
    }

    ~cPartida() {
        for (int i = 0; i < mTamanio; i++) {
            delete[] mTablero[i];
            delete[] mPrintTablero[i];}
        delete[] mTablero;
        delete[] mPrintTablero;
    }
};

void Menu();

void Tutorial() {
    cout <<  endl << endl << "         ⋆｡‧˚ʚ TUTORIAL ɞ˚‧｡⋆      " << endl << endl << endl;
    cout << " ☆ OBJETIVO ☆" <<  endl << endl;
    cout << "   descubrir todas las casillas libres de minas\n   sin hacer explotar ninguna." << endl << endl << endl <<  endl;
    cout << " ☆ ¿CÓMO SE JUEGA? ☆"  <<  endl << endl;
    cout << "    1. selecciona cualquier casilla. esto\n       abrirá un área inicial del tablero, revelando \n       algunos números y casillas en blanco." << endl << endl;
    cout << "       EJEMPLO: ingresaste la posición (4,1)" << endl << endl;
    cout << "         " <<"     1  2  3  4  " << endl;
    cout << "         " <<"  ╭───────────── " << endl;
    cout << "         " <<"1 |  ▢  1  ▢  ▢  " << endl;
    cout << "         " <<"2 |  1  0  2  ▢  " << endl;
    cout << "         " <<"3 |  0  0  1  ▢  " << endl;
    cout << "         " <<"4 |  0  0  1  ▢  " << endl << endl << endl ;
    cout << "    2. cada número en una casilla indica cuántas\n       minas hay en las casillas adyacentes \n       (horizontal, vertical o diagonal)." << endl <<  endl;
    //cout << "       EJEMPLOS:" << endl << endl; 
    cout << "      → si ves un '1', significa que hay una mina \n        en alguna de las ocho casillas alrededor." << endl << endl;
    cout << "         " <<  "    1  2  3  4  " << endl;
    cout << "         " << "  ╭───────────── " << endl;
    cout << "         " << "1 |  ?  ?  ?  ▢  " << endl;
    cout << "         " << "2 |  ?  1  ?  ▢  " << endl;
    cout << "         " << "3 |  ?  ?  ?  ▢  " << endl;
    cout << "         " << "4 |  ▢  ▢  ▢  ▢  " << endl << endl  ;
    cout << "      → un '0' significa que no hay minas en las\n        casillas cercanas, por lo que estas se\n        abrirán automáticamente." << endl << endl;
    cout << "         " <<  "    1  2  3  4  " << endl;
    cout << "         " << "  ╭───────────── " << endl;
    cout << "         " << "1 |  0  1  ▢  ▢  " << endl;
    cout << "         " << "2 |  1  1  ▢  ▢  " << endl;
    cout << "         " << "3 |  ▢  ▢  ▢  ▢  " << endl;
    cout << "         " << "4 |  ▢  ▢  ▢  ▢  " << endl << endl << endl;
    cout << "    3. si descubres una mina, pierdes el juego\n        y explotan todas las minas de tablero" << endl << endl;
    cout << "       EJEMPLO: ingresaste la posición (2,2)" << endl << endl;
    cout << "         " <<"     1  2  3  4  " << endl;
    cout << "         " <<"  ╭───────────── " << endl;
    cout << "         " <<"1 |  ★  ▢  ▢  ★  " << endl;
    cout << "         " <<"2 |  2  ★  ▢  ▢  " << endl;
    cout << "         " <<"3 |  1  1  2  ▢  " << endl;
    cout << "         " <<"4 |  0  0  1  ★  " << endl << endl;
    cout << "         " <<"  -- GAME OVER -- "<< endl ;

    this_thread::sleep_for(chrono::seconds(7));
    Menu();
}

void MostrarMejoresJugadores(const string& archivoNombre) {
    ifstream archivoLectura(archivoNombre);
    if (!archivoLectura.is_open()) {
        cerr << "no se pudo abrir el archivo: " << archivoNombre << endl;
        return;}

    map<string, double> mejoresTiempos;
    string nombre;
    double tiempo;
    string linea;

    while (getline(archivoLectura, linea)) {
        istringstream iss(linea);
        if (iss >> nombre >> tiempo) {
            if (mejoresTiempos.find(nombre) == mejoresTiempos.end() || mejoresTiempos[nombre] > tiempo) {
                mejoresTiempos[nombre] = tiempo;}}
        else {cerr << "línea con formato incorrecto ignorada: " << linea << endl;}}

    archivoLectura.close();

    if (mejoresTiempos.empty()) {
        cout << endl << "  no hay jugadores registrados en " << archivoNombre << endl;
        return;}

    vector<pair<string, double> > jugadores(mejoresTiempos.begin(), mejoresTiempos.end());

    sort(jugadores.begin(), jugadores.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;});

    cout << " ╭─ •✧ mejores Jugadores (" << setw(11) << archivoNombre << ") ✧• ─╮" << endl ;
    cout << " |                                         | " << endl;
    for (size_t i = 0; i < jugadores.size(); ++i) {
        cout << " |  " << i + 1 << ". " << jugadores[i].first << " - " << jugadores[i].second << "s" << endl;
    }
    cout << " |                                         | " << endl;
    cout << " ╰────────────────── •✧✧• ─────────────────╯" << endl << endl;
}

void Menu() {
    int k;
        cout << endl;
        cout << " ╔═.✵..════════════════════════╗" << endl;
        cout << " ║                             ║" << endl;
        cout << " ║            MENU             ║" << endl;
        cout << " ║                             ║" << endl;
        cout << " ║  1. jugar                   ║" << endl;
        cout << " ║  2. mejores jugadores       ║" << endl;
        cout << " ║  3. tutorial                ║" << endl;
        cout << " ║  4. salir                   ║" << endl;
        cout << " ║                             ║" << endl; 
        do {
            cout << " ║  escoge la opcion: ";
            cin >> k;
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                k = -1; }
        } while (k < 1 || k > 4); 
    cout << " ║                             ║" << endl;
    cout << " ╚═══════════════════════..✵.══╝" << endl << endl;

    //Jugar
    if (k == 1) {
        int i;
        string n;
        cout << " ╭──────────── •✧✧• ────────────╮" << endl;
        cout << " |                              | " << endl;
        cout << " |            JUGAR             | " << endl;
        cout << " |                              | " << endl;
        cout << " |  selecciona la dificultad    |" << endl;
        cout << " |   1. facil                   | " << endl;
        cout << " |   2. medio                   |" << endl;
        cout << " |   3. dificil                 |" << endl;
        do {
            cout << " |  ingrese la opcion: ";
            cin >> i;
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                i = -1; }
        } while (i < 1 || i > 3); 
        cout << " |  ingrese tu nombre: ";
        cin >> n;
        cout << " |                              | " << endl;
        cout << " ╰──────────── •✧✧• ────────────╯" << endl << endl;
        cout << "  ingresa '100' para escojer otra opción" << endl << endl << endl << endl << endl;
        cPartida partida(i, n);
        partida.Iniciarjuego();

        Menu(); } 

    else if (k == 2) {
        cout <<  endl <<  endl;
        MostrarMejoresJugadores("facil.txt");
        MostrarMejoresJugadores("medio.txt");
        MostrarMejoresJugadores("dificil.txt");
        int regresar;
        cout <<  " ╭════════════════════════════════════════════╮" << endl;
        do {
            cout <<" | ingresa un número para regresar al menú: ";
            cin >> regresar;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');}
            else {break;}
        } while (true);
        cout << " ╰════════════════════════════════════════════╯" << endl << endl ;
        Menu();}

    else if (k == 3) {
        Tutorial();
        Menu();} 

    else if (k == 4) {
        cout << endl << " muchas gracias por jugar" << endl;
        cout << endl << "  ヾ(￣3￣) byeee~";}
}

int main() {
    cout << endl;
    cout << "BIENVENIDO A BUSCAMINAS (∩^o^)⊃-☆" << endl << endl;
    Menu();
    return 0;}
