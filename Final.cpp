#define FPS_LIMIT 60

#include <iostream>
#include <thread>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include "mingl/include/mingl/mingl.h"
#include "mingl/include/mingl/gui/text.h"
#include "mingl/include/mingl/shape/line.h"
#include "mingl/include/mingl/shape/rectangle.h"

#include "source/AStar.hpp"

using namespace std;
vector <size_t> posWall;

nsGraphics::Vec2D rectPosJ1;
nsGraphics::Vec2D rectPosJ2;

bool playercollision = false;

const unsigned KSpeed = 2;

bool isGameOver = false;

const int Kheight = 640; //taille jeu

//Position mur
const nsGraphics::Vec2D PosWallY(100, 100);
const nsGraphics::Vec2D PosWallX(110, 140);
const nsGraphics::Vec2D PosWallY2(330, 120);
const nsGraphics::Vec2D PosWallX2(340, 180);
const nsGraphics::Vec2D PosWallY3(470, 350);
const nsGraphics::Vec2D PosWallX3(475, 500);
const nsGraphics::Vec2D PosWallY4(200, 400);
const nsGraphics::Vec2D PosWallX4(260, 410);
const nsGraphics::Vec2D PosWallY5(80, 300);
const nsGraphics::Vec2D PosWallX5(150, 310);
const nsGraphics::Vec2D PosWallY6(500, 100);
const nsGraphics::Vec2D PosWallX6(550, 115);
const nsGraphics::Vec2D PosWallY7(230, 540);
const nsGraphics::Vec2D PosWallX7(240, 600);

//Position buisson
const nsGraphics::Vec2D bush1X(300, 300);
const nsGraphics::Vec2D bush1Y(350, 350);
const nsGraphics::Vec2D bush2X(400, 200);
const nsGraphics::Vec2D bush2Y(450, 250);
const nsGraphics::Vec2D bush3X(150, 150);
const nsGraphics::Vec2D bush3Y(200, 200);
const nsGraphics::Vec2D bush4X(400, 520);
const nsGraphics::Vec2D bush4Y(480, 600);

chrono::time_point<chrono::steady_clock> startTime;

bool areRectanglesColliding(const nsGraphics::Vec2D &rect1TopLeft, const nsGraphics::Vec2D &rect1BottomRight,
                            const nsGraphics::Vec2D &rect2TopLeft, const nsGraphics::Vec2D &rect2BottomRight)
{
    return !(rect1BottomRight.getX() <= rect2TopLeft.getX() ||  // J1 est à gauche de J2
             rect1TopLeft.getX() >= rect2BottomRight.getX() ||  // J1 est à droite de J2
             rect1BottomRight.getY() <= rect2TopLeft.getY() ||  // J1 est au-dessus de J2
             rect1TopLeft.getY() >= rect2BottomRight.getY());   // J1 est en dessous de J2
}
bool detectCollisionWithWalls(const nsGraphics::Vec2D &playerTopLeft, const nsGraphics::Vec2D &playerBottomRight)
{
    // Liste mur
    vector<pair<nsGraphics::Vec2D, nsGraphics::Vec2D>> walls = {
        {PosWallY, PosWallX},
        {PosWallY2, PosWallX2},
        {PosWallY3, PosWallX3},
        {PosWallY4, PosWallX4},
        {PosWallY5, PosWallX5},
        {PosWallY6, PosWallX6},
        {PosWallY7, PosWallX7}
    };
    // Vérifie collisions murs
    for (size_t i = 0; i < walls.size(); ++i) {
        if (areRectanglesColliding(playerTopLeft, playerBottomRight, walls[i].first, walls[i].second)){
            return true;
        }
    }
    return false; // Pas de collision
}
void Player1(MinGL &window){
    //Deplacement J1
    int posFutur;

    if (window.isPressed({'z', false})) { // Haut
        posFutur = rectPosJ1.getY() - KSpeed;
        if (posFutur >= 0) {
            nsGraphics::Vec2D futurPos(rectPosJ1.getX(), posFutur);
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ1.setY(posFutur);
        }
    }
    if (window.isPressed({'s', false})) { // Bas
        posFutur = rectPosJ1.getY() + KSpeed;
        if (posFutur + 20 <= Kheight) {
            nsGraphics::Vec2D futurPos(rectPosJ1.getX(), posFutur);
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ1.setY(posFutur);
        }
    }

    if (window.isPressed({'q', false})) { // Gauche
        posFutur = rectPosJ1.getX() - KSpeed;
        if (posFutur >= 0) {
            nsGraphics::Vec2D futurPos(posFutur, rectPosJ1.getY());
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ1.setX(posFutur);
        }
    }

    if (window.isPressed({'d', false})) { // Droite
        posFutur = rectPosJ1.getX() + KSpeed;
        if (posFutur + 20 <= Kheight) {
            nsGraphics::Vec2D futurPos(posFutur, rectPosJ1.getY());
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ1.setX(posFutur);
        }
    }
}
void Player2(MinGL &window){
    //Deplacement J2
    int posFutur;

    if (window.isPressed({'o', false})) { // Haut
        posFutur = rectPosJ2.getY() - KSpeed;
        if (posFutur >= 0) {
            nsGraphics::Vec2D futurPos(rectPosJ2.getX(), posFutur);
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ2.setY(posFutur);
        }
    }

    if (window.isPressed({'l', false})) { // Bas
        posFutur = rectPosJ2.getY() + KSpeed;
        if (posFutur + 20 <= Kheight) {
            nsGraphics::Vec2D futurPos(rectPosJ2.getX(), posFutur);
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ2.setY(posFutur);
        }
    }

    if (window.isPressed({'k', false})) { // Gauche
        posFutur = rectPosJ2.getX() - KSpeed;
        if (posFutur >= 0) {
            nsGraphics::Vec2D futurPos(posFutur, rectPosJ2.getY());
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ2.setX(posFutur);
        }
    }

    if (window.isPressed({'m', false})) { // Droite
        posFutur = rectPosJ2.getX() + KSpeed;
        if (posFutur + 20 <= Kheight) {
            nsGraphics::Vec2D futurPos(posFutur, rectPosJ2.getY());
            nsGraphics::Vec2D futurBottomRight = futurPos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurPos, futurBottomRight))
                rectPosJ2.setX(posFutur);
        }
    }
}
// Aléatoire des murs et des buissons SOON !!!!!
// unsigned randomRange (const unsigned& min, const unsigned& max) {
//     return min + rand() % (max-min);
// }
// void Pos(){
//     for (int i = 0; i < 20; ++i) {
//         posWall.push_back(randomRange(10, Kheight-40));
//     }
// }
void draw(MinGL &window)
{
    // DessinJ1
    window << nsShape::Rectangle(rectPosJ1, rectPosJ1 + nsGraphics::Vec2D(20, 20), nsGraphics::KCyan);

    // Dessin J2
    window << nsShape::Rectangle(rectPosJ2, rectPosJ2 + nsGraphics::Vec2D(20, 20), nsGraphics::KRed);

    // draw les obstacles et buissons

    window << nsShape::Rectangle(bush1X, bush1Y, nsGraphics::KGreen);
    window << nsShape::Rectangle(bush2X, bush2Y, nsGraphics::KGreen);
    window << nsShape::Rectangle(bush3X, bush3Y, nsGraphics::KGreen);
    window << nsShape::Rectangle(bush4X, bush4Y, nsGraphics::KGreen);

    window << nsShape::Rectangle(PosWallX, PosWallY, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX2, PosWallY2, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX3, PosWallY3, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX4, PosWallY4, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX5, PosWallY5, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX6, PosWallY6, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX7, PosWallY7, nsGraphics::KBlack);
}
bool detectPlayerCollision(){
    string collision;
    bool playercollision = false;

    nsGraphics::Vec2D rect1BottomRight = rectPosJ1 + nsGraphics::Vec2D(20, 20);
    nsGraphics::Vec2D rect2BottomRight = rectPosJ2 + nsGraphics::Vec2D(20, 20);

    // Vérifie si J1 sur J2
    if (areRectanglesColliding(rectPosJ1, rect1BottomRight, rectPosJ2, rect2BottomRight)){
        collision = "Collision entre les players !";
        playercollision = true;
    }
    else{
        collision = "Pas de collision entre les players !";
        playercollision = false;
    }
    return playercollision;
}
void ShowEnd(MinGL &window, string &timerText){
    int textWidth = 220;
    int textHeight = 50;
    int offsetX = 220;
    int offsetY = 150;
    int padding = 10;

    // Fond txt
    window << nsShape::Rectangle(
        nsGraphics::Vec2D(offsetX - padding, offsetY - padding),
        nsGraphics::Vec2D(offsetX + textWidth + padding, offsetY + textHeight + padding),
        nsGraphics::KBlack
        );

    // Affiche le temps
    window << nsGui::Text(nsGraphics::Vec2D(230, 200), timerText, nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_12);
    window << nsGui::Text(nsGraphics::Vec2D(250, 160), "victory du joueur 1", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_18);
}
void Replay(MinGL &window){
    // Dimensions txt
    int textWidth = 280;
    int textHeight = 50;
    int offsetX = 200;
    int offsetY = 420;
    int padding = 10;

    // draw un rectangle de fond derrière le texte
    window << nsShape::Rectangle(
        nsGraphics::Vec2D(offsetX - padding, offsetY - padding),
        nsGraphics::Vec2D(offsetX + textWidth + padding, offsetY + textHeight + padding),
        nsGraphics::KBlue
        );

    window << nsGui::Text(nsGraphics::Vec2D(200, 450), "Appuyez sur Espace pour rejouer", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_18);
}
void resetGame() {
    rectPosJ1.setX(0);
    rectPosJ1.setY(0);

    rectPosJ2.setX(Kheight - 20);
    rectPosJ2.setY(Kheight - 20);

    startTime = chrono::steady_clock::now(); // Réinitialisation du timer
    isGameOver = false; // Le jeu redémarre
}
void actionFrame(MinGL &window){
    bool collision = detectPlayerCollision();

    static long lastElapsedTime = 0; //Variable chrono
    string timerText = "Le joueur 2 a survecu pendant : " + to_string(lastElapsedTime)  + "s !";

    if (!collision) {
        // MAJ position player
        Player1(window);
        Player2(window);

        // Calcule le temps écoulé
        auto currentTime = chrono::steady_clock::now();
        lastElapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

        // Affiche le temps
        timerText = "Temps: " + to_string(lastElapsedTime) + "s";
        window << nsGui::Text(nsGraphics::Vec2D(20, 50), timerText, nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15);
    }

    // Affichage du jeu
    draw(window);
    if (collision){
        isGameOver = true;
        ShowEnd(window, timerText);
        Replay(window);

        if (window.isPressed({' ', false})) {
            resetGame();
        }
    }
}
void GraphicGame(){
    MinGL window("MinGL", nsGraphics::Vec2D(Kheight, Kheight), nsGraphics::Vec2D(128, 128), nsGraphics::KGray); //Nom fenetre, Dimension fenetre, Emplacement de la fenetre au lancement
    window.initGlut();
    window.initGraphic();

    startTime = chrono::steady_clock::now(); // Initialisation du timer

    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();

    rectPosJ1.setX(0);
    rectPosJ1.setY(0);

    rectPosJ2.setX(Kheight - 20);
    rectPosJ2.setY(Kheight - 20);

    // Tant que la fenetre est ouverte:
    while (window.isOpen()){
        // Récupèration chrono
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

        window.clearScreen();

        actionFrame(window);

        window.finishFrame();

        window.getEventManager().clearEvents();

        // Si la valeur est baissée le jeu va plus vite, attention des fois ça lag quand on met trop petit
        this_thread::sleep_for(chrono::milliseconds(900 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
    }
}
void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KBlack    (30);
const unsigned KRed   (31);
const unsigned KGreen    (32);
const unsigned KYellow   (33);
const unsigned KBlue    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

void color (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

typedef vector <char> CVLine; // un type représentant une ligne de la grille
typedef vector <CVLine> CMatrix; // un type représentant la grille
typedef pair   <unsigned, unsigned> CPosition; // un type représentant une coordonnée dans la grille

void configureTerminal(bool enable) {
    struct termios touch;
    tcgetattr(STDIN_FILENO, &touch);
    if(enable) {
        touch.c_lflag |= ICANON;
        touch.c_lflag |= ECHO;
    } else {
        touch.c_lflag &= ~ICANON;
        touch.c_lflag &= ~ECHO;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &touch);
}

int entree(char c) {
    read(STDIN_FILENO, &c, 1); // lire caractère
    return c;
}

const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty        = '+';
const char kWall         = '/';
const char kBonus        = '*';

void  showMatrix (const CMatrix & Mat,bool & replay){
    clearScreen();
    for(auto i=0; i<Mat.size(); ++i){
        for(auto j=0; j<Mat[i].size(); ++j){
            if (Mat[i][j]== kTokenPlayer1){
                color(KRed);
                cout << kTokenPlayer1;
                color (KReset);
            }
            else if (Mat[i][j]== kTokenPlayer2){
                color(KBlue);
                cout << kTokenPlayer2;
                color (KReset);
            }
            else if (Mat[i][j]==kWall){
                color(KYellow);
                cout << kWall;
                color(KReset);
            }
            else if (Mat[i][j] == kBonus) {
                color(KMAgenta);
                cout << kBonus;
                color(KReset);
            }
            else{
                cout << kEmpty;
            }
        }
        cout << endl;
    }
    if(replay){
        cout <<"BONUS : Veuillez replay !"<<endl ;
    }
}
unsigned randomRange (const unsigned& min, const unsigned& max) {
    return min + rand() % (max-min);
}

CPosition ia(CMatrix & Mat, CPosition & posPlayer2, CPosition & posPlayer1){
    AStar::Generator generator;
    generator.setWorldSize({Mat.size(), Mat[0].size()});
    generator.setDiagonalMovement(true);
    auto coup = generator.findPath({posPlayer1.first, posPlayer1.second}, {posPlayer2.first, posPlayer2.second});
    return {coup[1].x, coup[1].y};
}

void iaMove(CMatrix & Mat, CPosition & posPlayer2, CPosition & posPlayer1){
    char element = Mat[posPlayer2.first][posPlayer2.second];
    Mat[posPlayer2.first][posPlayer2.second] = kEmpty;
    posPlayer2=ia(Mat, posPlayer2, posPlayer1);
    Mat[posPlayer2.first][posPlayer2.second] = element;
}

void initMat (CMatrix & Mat, unsigned nbLine, unsigned nbColumn, CPosition & posPlayer1, CPosition & posPlayer2){
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));

    unsigned borneMinRandom = 1;
    unsigned borneMaxRandom = 6;

    // Remplir aléatoirement avec des kWall
    for (unsigned i = 0; i < nbLine; ++i) {
        for (unsigned j = 0; j < nbColumn; ++j) {
            if (randomRange(borneMinRandom, borneMaxRandom) == randomRange(borneMinRandom, borneMaxRandom)) {
                Mat[i][j] = kWall;
            }
        }
    }
    borneMinRandom = 1;
    borneMaxRandom = 10;
    for (unsigned i = 0; i < nbLine; ++i) {
        for (unsigned j = 0; j < nbColumn; ++j) {
            if (randomRange(borneMinRandom, borneMaxRandom) == randomRange(borneMinRandom, borneMaxRandom)) {
                Mat[i][j] = kBonus;
            }
        }
    }
    // Placer les players
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
}


void moveToken(CMatrix &Mat, char move, CPosition &pos, bool &replay) {
    char element = Mat[pos.first][pos.second];
    Mat[pos.first][pos.second] = kEmpty;
    switch(tolower(move)) {
    case 'a': // Diagonal haut-gauche
        if (pos.first > 0 && pos.second > 0 && Mat[pos.first - 1][pos.second - 1] != kWall) {
            pos.first -= 1;
            pos.second -= 1;
        }
        break;
    case 'z': // Haut
        if (pos.first == 0) {
            if (Mat[Mat.size()-1][pos.second]!=kWall){
                pos.first = Mat.size() - 1;
            }
        }
        else if (Mat[pos.first - 1][pos.second] != kWall) {
            pos.first -= 1;
        }
        break;
    case 'e': // Diagonal haut-droite
        if (pos.first > 0 && pos.second < Mat[0].size() - 1 && Mat[pos.first - 1][pos.second + 1] != kWall) {
            pos.first -= 1;
            pos.second += 1;
        }
        break;
    case 'q': // Gauche
        if (pos.second == 0) {
            if(Mat[pos.first][Mat[0].size()-1]!=kWall){
                pos.second = Mat[0].size() - 1;
            }
        }
        else if (Mat[pos.first][pos.second - 1] != kWall) {
            pos.second -= 1;
        }
        break;
    case 'd': // Droite
        if (pos.second == Mat[0].size() - 1) {
            if (Mat[pos.first][0]!=kWall){
                pos.second = 0;
            }
        }
        else if (Mat[pos.first][pos.second + 1] != kWall) {
            pos.second += 1;
        }
        break;
    case 'w': // Diagonal bas-gauche
        if ((pos.first < Mat.size() - 1 && pos.second > 0) && Mat[pos.first + 1][pos.second - 1] != kWall){
            pos.first += 1;
            pos.second -= 1;
        }
        break;
    case 'x': // Bas
        if (pos.first == Mat.size() - 1) {
            if (Mat[0][pos.second]!=kWall){
                pos.first = 0;
            }
        }
        else if (Mat[pos.first + 1][pos.second] != kWall) {
            pos.first += 1;
        }
        break;
    case 'c': // Diagonal bas-droite
        if (pos.first < Mat.size() - 1 && pos.second < Mat[0].size() - 1 && Mat[pos.first + 1][pos.second + 1] != kWall) {
            pos.first += 1;
            pos.second += 1;
        }
        break;
    }
    if (Mat[pos.first][pos.second] == kBonus) {
        replay = true;
        Mat[pos.first][pos.second] = kEmpty;
    } else {
        replay = false;
    }
    Mat[pos.first][pos.second] = element;
}

void showRules() {
    cout << endl << "------------------------" << endl << "Règles supplémentaires :\n" << "------------------------" << endl
         << "z : se déplacer vers le haut" << endl
         << "q : se déplacer vers la gauche"<< endl
         << "x : se déplacer vers le bas" << endl
         << "d : se déplacer vers la droite" << endl
         << "a : se déplacer en haut à gauche" << endl
         << "w : se déplacer en bas à gauche"<< endl
         << "c : se  déplacer en bas à droite"<< endl
         << "e : se déplacer en haut à droite" <<endl
         << "o : se déplacer en haut (joueur 2 interface graphique)"<<endl
         <<"k : se déplacer à gauche (joueur 2 interface graphique)"<<endl
         <<"l : se déplacer en bas (joueur 2 interface graphique)"<<endl
         <<"m : se déplacer à droite (joueur 2 interface graphique)"<<endl << "------" << endl;

    cout << "Lore :\n" << "------" << endl
         << "Nous sommes en 2137. Le monde a irrémédiablement changé avec l'avancée rapide de la technologie et l'urbanisation incessante. Les mégapoles s'étendent jusqu'au ciel, citadelles de verre et d'acier. La société est contrôlée par un réseau de drones, de caméras et d'IA de surveillance qui rend presque impossible toute tentative d'évasion de la part d'un criminel.\n"
         << "À la suite de l'événement désastreux connu sous le nom de Grand Effondrement en 2098, les gouvernements nationaux se sont rassemblés sous la bannière de l'Union Terrienne Unifiée. Il s'agit d'une unité politique mondiale régissant toutes les mégapoles et dotée de forces de sécurité omniprésentes, la Division de la sécurité et de l'ordre public (Security and Public Order Division, SPOD).\n"
         << "Les agents de la SPOD forment un cadre professionnel composé des meilleurs agents, formés aux innovations les plus récentes en matière de combat et de surveillance. Leur mission consiste essentiellement à maintenir l'ordre dans les mégapoles et à traquer les criminels qui osent s'opposer à l'autorité de l'UTU, l'Union Terrestre Unifiée.\n"
         << "Mais dans ce monde hyperconnecté et surveillé, il existe une faction de renégats qui s'élèvent contre l'oppression : les Cyber-Outlaws. Il s'agit de bandits spécialisés dans la cybernétique, passés maîtres dans l'art de la furtivité et de l'infiltration. Opérant à l'ombre des gratte-ciel, ils utilisent des implants et des gadgets futuristes pour échapper aux griffes de la SPOD, tout cela pour la chute de l'UTU et le rétablissement de la liberté de l'humanité.\n"
         << "Les Cyber-Outlaws viennent de réussir un hold-up audacieux en volant des données critiques sur une conspiration au sein de l'UEU. Ces informations révèlent des plans clandestins visant à étrangler davantage les citoyens par des mesures répressives. Le SPOD, dirigé par l'infatigable inspecteur Voss, est à leurs trousses, déterminé à récupérer les informations volées et à mettre fin à leurs activités subversives.\n"
         << "Dans « Cyber-Escape : Futuristic Fugitives », les players doivent choisir leur camp, soit celui d'un agent du SPOD, soit celui d'un membre des Cyber-Outlaws.\n"
         << "Les agents de la SPOD doivent utiliser leurs compétences de détection avancées et leur puissance de feu pour traquer et appréhender les hors-la-loi. Les cyber-Outlaws doivent faire preuve de furtivité, de piratage et d'ingéniosité pour échapper au SPOD, mener à bien des missions secrètes et dévoiler les sombres secrets de l'UEU.\n"
         << "Chaque mouvement doit être planifié stratégiquement pour contourner les murs (représentant des obstacles ou des zones surveillées), et les téléportations (déplacements rapides entre les différents niveaux de la ville) doivent être utilisées judicieusement pour éviter les impasses et les pièges tendus par l'ennemi.\n";
}

void resetTerminal(int sig) {
    // Cette fonction permet de réinitialiser l'état du terminal lorsqu'on appuie sur ctrl+c
    // Elle permet surtout d'éviter de conserver la color bleu ou rouge lorsqu'on interrompt la partie
    cout << "\033[0m" << endl ;  // Réinitialisation de toutes les propriétés (y compris color)
    configureTerminal(true);
    exit(0);  // Quitter proprement le programme
}

int ppal(){
    srand(time(NULL));
    CMatrix Mat;
    unsigned nbLine;
    unsigned nbColumn;
    cout << "Rentrez la largeur de la carte (en ligne) : ";
    cin >> nbLine;
    cout << "Rentrez la hauteur de la carte (en colonne) : ";
    cin >> nbColumn;
    configureTerminal(false); // Désactiver le mode canonique
    signal(SIGINT, resetTerminal);
    CPosition posPlayer1;
    CPosition posPlayer2;
    posPlayer1 = CPosition(0, 0);
    posPlayer2 = CPosition(nbLine-1, nbColumn-1);
    const unsigned nbMax = nbLine * nbColumn;
    unsigned nbMove= 1;
    bool victory = false;
    bool replay = false;
    int player = 1;
    char game_mode;
    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2);
    char move;
    cout << "Appuyez sur 'm' pour commencer le jeu ou 'n' pour lire les règles.\n";
    while (true) {
        move = entree(move);

        if (tolower(move) == 'm') {
            break; // Commencer le jeu
        }
        if (tolower(move) == 'n') {
            showRules();
            cout << endl << "Appuyer sur 'm' pour lancer le jeu" <<endl;
            move=entree(move);
            if (tolower(move) == 'm'){
                showMatrix(Mat,replay);
                break ;
            }
            else{
                continue ;
            }
        }

    }

    cout<< endl <<"Choisissez votre mode de jeu:"<<endl;
    cout<<"Appuyez sur 'j' pour commencer le mode 1v1, sur 'k' pour commencer le mode 1vIA, sur 'l' pour le mode ChasseurVChassé ou sur 'g' pour le mode ChasseurVChassé version Graphique\n";
    while(true){
        move=entree(move);
        if(tolower(move)=='j' || tolower(move)=='k' || tolower(move)=='l'|| tolower(move)=='g'){
            if(tolower(move)=='j'){
                game_mode='j';
                break;
            }
            if(tolower(move)=='k'){
                game_mode='k';
                break;
            }
            if(tolower(move)=='l'){
                game_mode='l';
                break;
            }
            if(tolower(move)=='g'){
                game_mode='g';
                break;
            }
        }
        else{
            continue;
        }

    }
    if (game_mode=='j'){
        showMatrix(Mat,replay);
        color(KRed);
        cout << "Coup numero " << nbMove << endl;
        color(KReset);
        ++nbMove ;


        while (nbMove < nbMax && !victory) {

            if (player == 1) {
                move = entree(move) ;
                if(tolower(move) == 'a' || tolower(move) == 'z' || tolower(move) == 'e' || tolower(move) == 'd' || tolower(move) == 'c' || tolower(move) == 'x' || tolower(move) == 'w' || tolower(move) == 'q'){
                    moveToken(Mat, move, posPlayer1,replay);
                    while (replay) {
                        cout << "Rejoue, player " << player << " : ";
                        showMatrix(Mat,replay);
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer1, replay);
                    }

                    player = 2;
                }
                else{
                    continue ;
                }
            }
            else {
                move = entree(move) ;
                if(tolower(move) == 'a' || tolower(move) == 'z' || tolower(move) == 'e' || tolower(move) == 'd' || tolower(move) == 'c' || tolower(move) == 'x' || tolower(move) == 'w' || tolower(move) == 'q'){
                    moveToken(Mat, move, posPlayer2,replay);
                    while (replay) {
                        cout << "Rejoue, player " << player << " : ";
                        showMatrix(Mat,replay);
                        move = entree(move);
                        cout << endl;
                        moveToken(Mat, move, posPlayer2, replay);
                    }
                    player = 1;
                }
                else{
                    continue ;
                }
            }
            showMatrix(Mat,replay);
            if (nbMove % 2 == 0) {
                color(KBlue);
                cout << "Coup numero " << nbMove << endl;
                color(KReset);
            }
            else {
                color(KRed);
                cout << "Coup numero " << nbMove << endl;
                color(KReset);
            }
            cout << "Coup du joueur " << player << " : ";
            if (posPlayer1 == posPlayer2)
                victory = true;
            else
                ++nbMove;

        }
        if (victory){
            if(nbMove % 2 == 1 ){
                color(KBlue);
            }
            else{
                color(KRed);
            }
            cout<<"victory Royale du joueur "<< player << " !"<<endl;
            color(KReset);
            cout << "Bien joué !" << endl ;
        }
        else{
            cout<<"Match nul!"<<endl;
        }
        return 0;
    }
    if (game_mode=='g'){
        GraphicGame();
    }
    else if (game_mode=='l'){
        showMatrix(Mat,replay);
        color(KRed);
        cout << "Coup numero " << nbMove << endl;
        color(KReset);
        ++nbMove;
        int hunter = 1;
        while (nbMove < nbMax && !victory) {
            if (player == 1) {
                move = entree(move) ;
                if(move == 'a' || move == 'z' || move == 'e' || move == 'd' || move == 'c' || move == 'x' || move == 'w' || move == 'q'){
                    moveToken(Mat, move, posPlayer1,replay);
                    while (replay) {
                        showMatrix(Mat,replay);
                        cout << "Rejoue, joueur " << player << " : ";
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer1, replay);
                    }
                    player = 2;
                }
                else{
                    continue ;
                }
            }
            else {
                move = entree(move) ;
                if(move == 'a' || move == 'z' || move == 'e' || move == 'd' || move == 'c' || move == 'x' || move == 'w' || move == 'q'){
                    moveToken(Mat, move, posPlayer2,replay);
                    while (replay) {
                        showMatrix(Mat,replay);
                        cout << "Rejoue, joueur " << player << " : ";
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer2, replay);
                    }
                    player = 1;
                }
                else{
                    continue ;
                }
            }
            showMatrix(Mat,replay);
            if (nbMove % 2 == 0) {
                color(KBlue);
                cout << "Coup numero " << nbMove << endl;
                color(KReset);
            }
            else {
                color(KRed);
                cout << "Coup numero " << nbMove << endl;
                color(KReset);
            }
            cout << "Coup du joueur " << player << " : ";
            if (posPlayer1 == posPlayer2)
                victory = true;
            else
                ++nbMove;
            if(nbMove%10==0){
                hunter=(nbMove/10)%2;
                cout<<"Le chasseur est le joueur: "<<hunter+1<<endl;
            }
        }
        if (victory)
            cout<<"Le chasseur a gagné!"<<endl;
        else
            cout<<"Le chassé a gagné"<<endl;
    }
    else if (game_mode=='k'){
        showMatrix(Mat,replay);
        color(KRed);
        cout << "Coup numero " << nbMove << endl;
        color(KReset);
        ++nbMove ;
        while (nbMove < nbMax && !victory) {

            if (player == 1) {
                move = entree(move) ;
                if(tolower(move) == 'a' || tolower(move) == 'z' || tolower(move) == 'e' || tolower(move) == 'd' || tolower(move) == 'c' || tolower(move) == 'x' || tolower(move) == 'w' || tolower(move) == 'q'){
                    moveToken(Mat, move, posPlayer1,replay);
                    while (replay) {
                        cout << "Rejoue, joueur " << player << " : ";
                        showMatrix(Mat,replay);
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer1, replay);
                    }

                    player = 2;
                }
                else{
                    continue ;
                }
            }
            else {
                iaMove(Mat,posPlayer2, posPlayer1) ;
                player=1;
            }
            showMatrix(Mat,replay);
            if (nbMove % 2 == 0) {
                color(KBlue);
                cout << "Coup numero " << nbMove << endl;
                color(KReset);
            }
            else {
                color(KRed);
                cout << "Coup numero " << nbMove << endl;
                color(KReset);
            }
            cout << "Coup du joueur " << player << " : ";
            if (posPlayer1 == posPlayer2)
                victory = true;
            else
                ++nbMove;

        }
        if (victory){
            if(nbMove % 2 == 1 ){
                color(KBlue);
            }
            else{
                color(KRed);
            }
            if (player == 1){
               cout<<"victory Royale de l'IA !"<<endl;
            }
            else{
                cout<<"victory Royale du joueur 1 !"<<endl;}
            color(KReset);
            cout << "Bien joué !" << endl ;
        }
        else{
            cout<<"Match nul!"<<endl;
        }
        return 0;
    }
}

int main() {
    ppal();
    configureTerminal(true);
    return 0;
}
