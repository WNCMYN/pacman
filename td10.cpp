#include <iostream>
#include <vector>

using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

typedef vector <char> CVLine; // un type représentant une ligne de la grille
typedef vector <CVLine> CMatrix; // un type représentant la grille
typedef pair   <unsigned, unsigned> CPosition; // un type représentant une coordonnée dans la grille

const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty        = '+';

void  showMatrix (const CMatrix & Mat){
    clearScreen();
    for(auto i=0; i<Mat.size(); ++i){
        for(auto j=0; j<Mat[i].size(); ++j){
            if (Mat[i][j]== kTokenPlayer1){
                couleur(KRouge);
                cout << kTokenPlayer1;
                couleur (KReset);
            }
            else if (Mat[i][j]== kTokenPlayer2){
                couleur(KBleu);
                cout << kTokenPlayer2;
                couleur (KReset);
            }
            else{
                cout << kEmpty;
            }
        }
        cout << endl;
    }
}

void initMat (CMatrix & Mat, unsigned nbLine, unsigned nbColumn, CPosition & posPlayer1, CPosition & posPlayer2){
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
}

void moveToken (CMatrix & Mat, char move, CPosition  & pos){
    char element = Mat[pos.first][pos.second];
    Mat[pos.first][pos.second] = kEmpty;
    switch(tolower(move))
    {
    case 'a':
        if(!(pos.first < 1 || pos.second < 1)){
            pos.first -= 1;
            pos.second -= 1;
        }
        break;
    case 'z':
        if(pos.first < 1){
            pos.first = Mat.size()-1;
        }
        else{
            pos.first -= 1;
        }
        break;
    case 'e':
        if(!(pos.first < 1 || pos.second > Mat[0].size()-2)){
            pos.first -= 1;
            pos.second += 1;
        }
        break;
    case 'q':
        if(pos.second < 1){
            pos.second = Mat[0].size()-1;
        }
        else{
            pos.second -= 1;
        }
        break;
    case 'd':
        if(pos.second > Mat[0].size()-2){
            pos.second = 0;
        }
        else{
            pos.second += 1;
        }
        break;
    case 'w':
        if(!(pos.first > Mat.size()-2 || pos.second < 1)){
            pos.first += 1;
            pos.second -= 1;
        }
        break;
    case 'x':
        if(pos.first > Mat.size()-2){
            pos.first = 0;
        }
        else{
            pos.first += 1;
        }
        break;
    case 'c':
        if(!(pos.first > Mat.size()-2 || pos.second > Mat[0].size()-2)){
            pos.first += 1;
            pos.second += 1;
        }
        break;
    }
    Mat[pos.first][pos.second] = element;
}

int ppal(){
    CMatrix Mat;
    unsigned nbLine;
    unsigned nbColumn;
    cout << "Rentrez la largeur de la carte (en ligne) : ";
    cin >> nbLine;
    cout << "Rentrez la hauteur de la carte (en colonne) : ";
    cin >> nbColumn;
    CPosition posPlayer1;
    CPosition posPlayer2;
    posPlayer1 = CPosition(0, 0);
    posPlayer2 = CPosition(nbLine-1, nbColumn-1);
    const unsigned nbMax = 20;
    unsigned nbCoup = 1;
    bool victoire = false;
    int joueur = 1;
    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2);
    showMatrix(Mat);
    string move;
    while(nbCoup<nbMax && !victoire){
        cout << "Coup numero " << nbCoup << endl;
        cout << "Coup du joueur " << joueur << " : ";
        getline(cin, move);
        cout << endl;
        if(nbCoup%2 == 1){
            moveToken(Mat, move[0], posPlayer1);
            joueur = 2;
        }
        else{
            moveToken(Mat, move[0], posPlayer2);
            joueur = 1;
        }
        showMatrix(Mat);
        if(posPlayer1 == posPlayer2)
            victoire = true;
        ++nbCoup;
    }
    if (victoire){
        cout<<"Victoire Royale"<<endl;
    }
    else{
        cout<<"match nul!"<<endl;
    }
    return 0;
}

int main() {
    ppal();
    return 0;
}
