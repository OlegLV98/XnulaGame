#include <iostream>
#include <ncurses.h>
using namespace std;

class DCoord {
private:
    int m_x;
    char** pole;
public:
    DCoord()=default;
    DCoord(int x){
        m_x = x;
        pole = new char*[x];
        for(int i = 0; i < x; ++i){
            pole[i] = new char[x];
        }
    }
    ~DCoord(){
        this->clean();
    }
    char**& getPole();
    void fill();
    void show();
    void clean();
    int getX() const;
};
int DCoord::getX() const{
    return m_x;
}
char**& DCoord::getPole(){
    return pole;
}
void DCoord::fill(){
    for(int i = 0; i < m_x; ++i){
        for(int j = 0; j < m_x; ++j){
            if(i % 3 == 0 || j % 3 == 0){
                pole[i][j] = '#';
            }else if(pole[i][j] == 'b' || pole[i][j] == 'w'){

            }else {
                pole[i][j] = ' ';
            }
        }
    }
}

void DCoord::show(){
    for(int i = 0; i < m_x; ++i){
        for(int j = 0; j < m_x; ++j){
            if(pole[i][j] == '+'){
                attron(COLOR_PAIR(1));
                printw("%c ", pole[i][j]);
                attroff(COLOR_PAIR(1));
            } else if(pole[i][j] == '#'){
                attron(COLOR_PAIR(2));
                printw("%c ", pole[i][j]);
                attroff(COLOR_PAIR(2));
            } else if(pole[i][j] == 'w'){
                attron(COLOR_PAIR(3));
                printw("%c ", pole[i][j]);
                attroff(COLOR_PAIR(3));
            } else if(pole[i][j] == 'b'){
                attron(COLOR_PAIR(4));
                printw("%c ", pole[i][j]);
                attroff(COLOR_PAIR(4));
            } else {
                printw("%c ", pole[i][j]);
            }
            //printw("%c ", pole[i][j]);
            //cout << pole[i][j] << ' ' << ' ';
        }
        printw("\n");
        //cout << endl;
    }
}
void DCoord::clean(){
//    for(int i = 0; i < m_x; ++i){
//        delete []pole[i];
//    }
//    delete []pole;
}
class Position {
private:
    int m_x;
    int m_y;
public:
    Position()=default;
    Position(int x, int y){
        m_x = x;
        m_y = y;
    }
    Position(Position& a){
        m_x = a.getX();
        m_y = a.getY();
    }
    int getX() const{
        return m_x;
    }
    int getY() const{
        return m_y;
    }
    void setPosition(int x, int y){
        m_x = x;
        m_y = y;
    }
};
class Area {
private:
    int ltX;
    int ltY;
    int rdX;
    int rdY;
public:
    Area(Position p){
        ltX = p.getX();
        ltY = p.getY();
        rdX = p.getX() + 3;
        rdY = p.getY() + 3;
    }
};
class Box {
private:
    DCoord* d;
    Position newP;
    int m_n;
    char m_symbol;
public:
    Box(DCoord*& a, int n, Position& p){
        d = a;
        newP.setPosition(p.getX(),p.getY());
        m_n = n;
        if(m_n % 2 == 0){
            m_symbol = 'b';
        } else {
            m_symbol = 'w';
        }

    }
    void show(){
        d->getPole()[newP.getX() + 1][newP.getY() + 1] = m_symbol;
        d->getPole()[newP.getX() + 1][newP.getY() + 2] = m_symbol;
        d->getPole()[newP.getX() + 2][newP.getY() + 1] = m_symbol;
        d->getPole()[newP.getX() + 2][newP.getY() + 2] = m_symbol;
    }
};
class Cell {
private:
    char m_symbol;
    DCoord* d;
    Position sP;
    Position actP;
public:
    Cell(DCoord& a, char symbol) {
        d = &a;
        m_symbol = symbol;
        sP.setPosition(0,0);
        actP = sP;
    }
    void show(){
        d->getPole()[actP.getX()][actP.getY() + 1] = m_symbol;
        d->getPole()[actP.getX()][actP.getY() + 2] = m_symbol;
        d->getPole()[actP.getX() + 1][actP.getY()] = m_symbol;
        d->getPole()[actP.getX() + 2][actP.getY()] = m_symbol;
        d->getPole()[actP.getX() + 3][actP.getY() + 1] = m_symbol;
        d->getPole()[actP.getX() + 3][actP.getY() + 2] = m_symbol;
        d->getPole()[actP.getX() + 1][actP.getY() + 3] = m_symbol;
        d->getPole()[actP.getX() + 2][actP.getY() + 3] = m_symbol;
    }
    Position& getActPos(){
        return actP;
    }
    char move(int& num){
        char in = getch();

        if(in == 'w'){
            if(actP.getX() == 0){
            } else {
                actP.setPosition(actP.getX() - 3, actP.getY());
                return 'w';
            }
        } else if(in == 's'){
            if(actP.getX() == d->getX() - 4){
            } else {
                actP.setPosition(actP.getX() + 3, actP.getY());
                return 's';
            }
        } else if(in == 'a'){
            if(actP.getY() == 0){
            } else {
                actP.setPosition(actP.getX(), actP.getY() - 3);
                return 'a';
            }
        } else if(in == 'd'){
            if(actP.getY() == d->getX() - 4){
            } else {
                actP.setPosition(actP.getX(), actP.getY() + 3);
                return 'd';
            }
        } else if(in == ' '){
            Box newB(d, num, actP);
            newB.show();
            ++num;
            return '_';
        } else if(in == 'q'){
            return 'q';
        }
        return 'o';
    }
};

class Game {
private:

public:
    Game(){

    }
    void start(){
        char size = 10;
        echo();
        while(true){
            move(0,0);
            printw("Please, enter number from 1 to 7: ");
            refresh();
            size = getch();
            if(static_cast<int>(size) >= 48 && static_cast<int>(size) <= 55){break;}
        }
        noecho();
        DCoord dCoord(((static_cast<int>(size) - 48) * 3) + 1);
        dCoord.fill();
        dCoord.show();
        int num = 1;
        Cell cell(dCoord, '+');
        while(cell.move(num) != 'q'){
            move(0,0);
            dCoord.fill();
            cell.show();
            dCoord.show();
        }
    }
};

int main() {
    //srand(time(NULL));
    initscr();
    curs_set(0);
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    Game g;
    g.start();
    endwin();
}