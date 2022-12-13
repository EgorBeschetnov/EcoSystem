#include "iostream"
#include <random>
#include <iomanip>
#include <ctime>
using namespace std;

#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif

const int SZ=10; // SZ*2 - кол-во объектов в экосистеме == кол-во ячеек в матрице
//начальное кол-во растений/травоядных/хищников в системе
const int _pl=7; 
const int _he=4;
const int _pr=3;
const long N=10; //кол-во итераций
int Time; //кол-во итераций на данный момент
 
 
 
class Base{ //Базовый класс для объектов экосистемы
public:
    char Name=' '; // имя объекта, отображаемое в таблце
    int Ind=0; //индефикатор для определения типа объекта
    int Energy=0; // энергия объекта,(голод) для анализа готовности к размножению, смерти.
    int DiplEnergy=0; // кол-во энергии, необходимое для размножения
 
};
 
Base MAP[SZ][SZ]; //таблица для визуального представления экосистемы
 
class Plant: public Base{ //Класс растения наследован от базового класса
public:
    int Growth; //промежуток времени через который вырастает новое растение в экосистеме.
    Plant(){
        Growth = 3;
        Name = '*'; //Plant
        }
    
};
 
class Herbivore : public Base{ //Класс травоядного животного наследован от базового класса
public:
    
    Herbivore(){
        DiplEnergy = 30; // кол-во энергии, необходимое для размножения
        Name = 'o';
    }
};
 
class Predator : public Base{ //Класс хищника наследован от класса травоядного животного
public:
 
    Predator(){
        DiplEnergy = 40; // кол-во энергии, необходимое для размножения
        Name = '#';
    }
 
};
 
void Step(int s, int i, int j){
    cout << "step ";
    switch (s)
    {
    case 1:
        MAP[i][j - 1].Ind = MAP[i][j].Ind;  //Клонирование объекта в выбранную ячейку
        MAP[i][j - 1].Energy = MAP[i][j].Energy;
        MAP[i][j - 1].Name = MAP[i][j].Name;
 
        break;
    case 2:
        MAP[i + 1][j].Ind = MAP[i][j].Ind;  //Клонирование объекта в выбранную ячейку
        MAP[i + 1][j].Energy = MAP[i][j].Energy;
        MAP[i + 1][j].Name = MAP[i][j].Name;
        break;
    case 3:
        MAP[i][j + 1].Ind = MAP[i][j].Ind;  //Клонирование объекта в выбранную ячейку
        MAP[i][j + 1].Energy = MAP[i][j].Energy;
        MAP[i][j + 1].Name = MAP[i][j].Name;
        break;
    case 4:
        MAP[i - 1][j].Ind = MAP[i][j].Ind; //Клонирование объекта в выбранную ячейку
        MAP[i - 1][j].Energy = MAP[i][j].Energy;
        MAP[i - 1][j].Name = MAP[i][j].Name;
        break;
    default:
        cout << "ERROR";
        break;
    }
 
}
 
void Random(int m, int n){
    cout << "Random ";
    int side;
    bool k = 0;
    do {
        k = 0;
        side = rand() % (4 - 1 + 1) + 1; //рандомно выбираем сторону
        switch (side)
        {
        case 1: if (MAP[m][n - 1].Ind == 0){ //если выпало вверх и ячейка пуста
            Step(side, m, n); //вызов функции перемещения
            k = 1;
        }
                break;
        case 2: if (MAP[m + 1][n].Ind == 0){ //если выпало направо и ячейка пуста
            Step(side, m, n); //вызов функции перемещения
            k = 1;
        }
                break;
        case 3: if (MAP[m][n + 1].Ind == 0){ //если выпало вниз и ячейка пуста
            Step(side, m, n); //вызов функции перемещения
            k = 1;
        }
                break;
        case 4: if (MAP[m - 1][n].Ind == 0){ //если выпало влево и ячейка пуста
            Step(side, m, n); //вызов функции перемещения
            k = 1;
        }
                break;
        default:
            cout << "ERROR";
            break;
        }
    } while (k == 1);
}
 
 
void clear(int i, int j){
    MAP[i][j].Ind = 0;
    MAP[i][j].Energy = 0;
    MAP[i][j].Name = ' ';
}
 
void LivePl(int m, int n){ //функция жизненного цикла растения 
    cout << "Plant ";
    int DiplTime = 3; //промежуток времени через который происходит размножение в экосистеме.
    MAP[m][n].Energy = MAP[m][n].Energy - 1; //отнимает 1 энергию, 
    if (MAP[m][n].Energy == 0){ //если энергии не осталось, убирает растение с поля
        MAP[m][n].Ind = 0;
        MAP[m][n].Name = ' ';
    }
    else {
 
        Plant Simple; //опытный образец растения для сравнения 
        if (Time >= DiplTime){ //условие размножения
            Random(m, n);
            MAP[m][n].Energy = 10;
 
        }
    }
}
 
void LiveHe(int m, int n){ //функция жизненного цикла травоядного
    cout << "Herbivore ";
    int DiplTime = 5; //промежуток времени через который происходит размножение в экосистеме.
    MAP[m][n].Energy = MAP[m][n].Energy - 1; //отнимает 1 энергию, 
    if (MAP[m][n].Energy == 0){ //если энергии не осталось, убирает животное с поля
        clear(m, n);
    }
    else{
        int side = 0;
        if ((n != 1) && (MAP[m][n - 1].Ind == 2)){ //если верхняя клетка над объектом растение, то съесть
            MAP[m][n].Energy = MAP[m][n - 1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
            clear(m, n - 1); //обнуление растения
            side = 1; //передвижение вверх
            Step(side, m, n); //вызов функции перемещения
            clear(m, n); //удаление его бывшего места обитания
        }
        else{
            if ((m != SZ) && (MAP[m + 1][n].Ind == 2)){ //если правая клетка около объекта - растение, то съесть
                MAP[m + 1][n].Energy = MAP[m + 1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                clear(m + 1, n); //обнуление растения //обнуление растения
                side = 2; //передвижение вправо
                Step(side, m, n);
                clear(m, n); //удаление его бывшего места обитания
            }
            else{
                if ((n != SZ) && (MAP[m][n + 1].Ind == 2)){ //если нижняя клетка под объектом - растение, то съесть
                    MAP[m][n].Energy = MAP[m][n + 1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                    clear(m, n + 1); //обнуление растения //обнуление растения
                    side = 3; //передвижение вниз
                    Step(side, m, n); //вызов функции перемещения
                    clear(m, n); //обнуление растения //удаление его бывшего места обитания
                }
                else{
                    if ((m != 1) && (MAP[m - 1][n].Ind == 2)){ //если левая клетка около объекта - растение, то съесть
                        MAP[m][n].Energy = MAP[m - 1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                        clear(m - 1, n); //обнуление растения
                        side = 4; //передвижение влево
                        Step(side, m, n); //вызов функции перемещения
                        clear(m, n); //удаление его бывшего места обитания
                    }
                    else{ //если растений вокруг травоядного нет
                        Random(m, n);
                    }
 
                }
            }
 
        }
 
        Herbivore Simple; //опытный образец травоядного для сравнения 
        cout << Simple.DiplEnergy << endl;
        cout << MAP[m][n].Energy << endl;
        cout << DiplTime << endl;
        if ((MAP[m][n].Energy >= Simple.DiplEnergy) && (Time >= DiplTime)){ //условие размножения
            Random(m, n);
            MAP[m][n].Energy = 20;
 
        }
    }
 
}
 
void LivePr(int m, int n){ //функция жизненного цикла хищника
    cout << "Predator ";
    int DiplTime = 7; //промежуток времени через который происходит размножение в экосистеме.
    MAP[m][n].Energy = MAP[m][n].Energy - 1; //отнимает 1 энергию, 
    if (MAP[m][n].Energy == 0){ //если энергии не осталось, убирает животное с поля
        clear(m, n);
    }
    else{
        int side = 0;
        if ((n != 1) && (MAP[m][n - 1].Ind == 3)){ //если верхняя клетка над объектом добыча, то съесть
            MAP[m][n].Energy = MAP[m][n - 1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
            clear(m, n - 1); //обнуление добычи
            side = 1; //передвижение вверх
            Step(side, m, n); //вызов функции перемещения
            clear(m, n); //удаление его бывшего места обитания
        }
        else{
            if ((m != SZ) && (MAP[m + 1][n].Ind == 3)){ //если правая клетка около объекта - добыча, то съесть
                MAP[m][n].Energy = MAP[m + 1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                clear(m + 1, n);//обнуление добычи
                side = 2; //передвижение вправо
                Step(side, m, n);
                clear(m, n); //удаление его бывшего места обитания
            }
            else{
                if ((n != SZ) && (MAP[m][n + 1].Ind == 3)){ //если нижняя клетка под объектом - добыча, то съесть
                    MAP[m][n].Energy = MAP[m][n + 1].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                    clear(m, n + 1); //обнуление добычи
                    side = 3; //передвижение вниз
                    Step(side, m, n); //вызов функции перемещения
                    clear(m, n); //удаление его бывшего места обитания
                }
                else{
                    if ((m != 1) && (MAP[m - 1][n].Ind == 3)){ //если левая клетка около объекта - добыча, то съесть
                        MAP[m][n].Energy = MAP[m - 1][n].Energy + MAP[m][n].Energy; //добавляем энергию после съедения
                        clear(m - 1, n); //обнуление добычи
                        side = 4; //передвижение влево
                        Step(side, m, n); //вызов функции перемещения
                        clear(m, n); //удаление его бывшего места обитания
                    }
                    else{ //если растений вокруг травоядного нет
                        Random(m, n);
                    }
 
                }
            }
 
        }
 
        Predator Simple; //опытный образец хищника для сравнения 
        cout << Simple.DiplEnergy << endl;
        cout << MAP[m][n].Energy << endl;
        cout << DiplTime << endl;
        if ((MAP[m][n].Energy >= Simple.DiplEnergy) && (Time >= DiplTime)){ //условие размножения
            Random(m, n);
            MAP[m][n].Energy = 25;
 
        }
    }
 
}
 
void show(){
    cout << "show";
    int c;
    cout << "FOREST" << endl; //рисуем таблицу и выводим данные
    cout << char(201);
    for (int i = 0; i < SZ * 2 + 1; i++){
        cout << char(205);
    }
    cout << char(187) << endl;
    for (int i = 0; i < SZ; i++){ //заполнение таблицы 
        cout << char(186);
        for (int j = 0; j < SZ; j++){
            c = MAP[i][j].Ind;
            cout << setw(2);
            switch (c)
            {
            case 0:
                cout << MAP[i][j].Name;
                break;
            case 1:
                cout << MAP[i][j].Name;
                break;
            case 2:
                cout << MAP[i][j].Name;
                break;
            case 3:
                cout << MAP[i][j].Name;
                break;
            default:
                cout << "ERROR";
                break;
            }
            cout << setw(2);
        }
        cout << char(186) << endl;
    }
    cout << char(200);
    for (int i = 0; i < SZ * 2 + 1; i++){
        cout << char(205);
    }
    cout << char(188) << endl;
}
 
void NachUsl(){ //функция задающая начальные условия экосистеме, помещает объекты на поле
    cout << "Start";
    for (int i = 0; i < SZ; i++){
        for (int j = 0; j < SZ; j++){
            clear(i, j);
        }
    }
    int m, n; //для задания начального кол-ва растений в экосистеме;
    int i = 0;
    do {
        m = rand() % ((SZ - 1) - 0 + 1) + 0; //находим рандомные координаты для растения на карте
        n = rand() % ((SZ - 1) - 0 + 1) + 0;
        i = i + 1;
        if (MAP[m][n].Ind == 0){  
            MAP[m][n].Ind = 1; // индефикатор
            MAP[m][n].Energy = 10;  //даем энергию объекту
            MAP[m][n].Name = '*'; //отоброжение на экране
        }
        else{
            if (i != 0){ i = i - 1; }  //если вдруг клетка занята, повторяем поиск
            else {
                i = 0;
            }
        }  
    } while (i < _pl);
 
    //для задания начального кол-ва травоядных в экосистеме;
 
    i = 0;
    do {
        m = rand() % ((SZ-1) - 0 + 1) + 0; //находим рандомные координаты для травоядного на карте
        n = rand() % ((SZ-1) - 0 + 1) + 0;
        i = i + 1;
        if (MAP[m][n].Ind == 0){
            MAP[m][n].Ind = 2; //индефикатор
            MAP[m][n].Energy = 20;  //даем энергию объекту
            MAP[m][n].Name = 'o'; //отоброжение на экране
        }
        else{
            if (i != 0){ i = i - 1; }  //если вдруг клетка занята, повторяем поиск
            else {
                i = 0;
            }
        }
    } while (i < _he);
 
        //для задания начального кол-ва хищников в экосистеме;
    i = 0;
    do {
            m = rand() % ((SZ - 1) - 0 + 1) + 0; //находим рандомные координаты для хищника на карте
            n = rand() % ((SZ - 1) - 0 + 1) + 0;
            i = i + 1;
            if (MAP[m][n].Ind == 0){
                MAP[m][n].Ind = 3; //индефикатор
                MAP[m][n].Energy = 25;  //даем энергию объекту
                MAP[m][n].Name = '#'; //отоброжение на экране
            }
            else{
                if (i != 0){ i = i - 1; }  //если вдруг клетка занята, повторяем поиск
                else {
                    i = 0;
                }
            }
 
    } while (i < _pr);
 
    }
 
void Life(){ // Функция прохождения 1 жизненного цикла для карты
    cout << "life" << endl;
    Time = Time + 1;
 
    for (int i = 0; i < SZ; i++){  //рассматриваем все ячейки, ищем заполненные
        for (int j = 0; j < SZ; j++){
            if (MAP[i][j].Ind != 0){
                int type;
                type = MAP[i][j].Ind; //считываем индефикатор объекта
                switch (type)
                {
                case 1: //если растение - запуск функции жизненного цикла для растения
                    LivePl(i,j);  
                    break;
                case 2: //если травоядное - запуск функции жизненного цикла для травоядного
                    LiveHe(i,j);
                    break;
                case 3: //если хищник - запуск функции жизненного цикла хищника
                    LivePr(i,j);
                    break;
                default: //индефикатор не опознан - ошибка
                    cout << "EROR";
                    break;
                }
            }
        }
    }
        
    
}
 
int main(int argc, TCHAR* argv[])
{   
    srand((time(NULL)));
    NachUsl();
    show();
    Life();
    show();
    return 0;
}