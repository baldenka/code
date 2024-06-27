#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath> 
#include <cstdlib>
#include <algorithm>
#include <string>
#include <ctime>
#include <time.h>

using namespace std;

const int hashTableSize = 500;
int counterRooms = 1;
string num;

//структура гостя
struct Guest {
    string passport; //серия+номер паспорта
    string fio; //фио 
    int year; //год рождения
    string address; //адрес проживания
    string purpose; //цель прибытия
    Guest* next;

    Guest() {
        passport = ' ';
        fio = ' ';
        year = 0;
        address = ' ';
        purpose = ' ';
        next = nullptr;
    }

    Guest(string passport, string fio, int year, string address, string purpose) { // конструктор с заданными значениями
        this->passport = passport;
        this->fio = fio;
        this->year = year;
        this->address = address;
        this->purpose = purpose;
        next = nullptr;
    }
    
    string getFio() {
        return fio;
    }

    string getPass() {
        return passport;
    }
};

void GuestInfo(Guest* guest) {
    cout << "-----------------------------------------" << endl;
    cout << "постоялец: " << guest->fio << endl;
    cout << "паспорт: " << guest->passport << endl;
    cout << "год рождения: " << guest->year << endl;
    cout << "место проживания: " << guest->address << endl;
    cout << "цель прибытия: " << guest->purpose << endl;
}

//структура заселения/выселения
struct CheckInOut {
    string passport; //серия+номер паспорта
    string number; //номер номера
    string dateIn; //дата заселения
    string dateOut; //дата выселения

    CheckInOut* next = NULL; //следующий элемент двусвязного списка
    CheckInOut* prev = NULL; //предыдущий элемент двусвязного списка

    CheckInOut() {
        passport = "";
        number = "";
        dateIn = "";
        dateOut = "";
        prev = next = NULL;
    }

    CheckInOut(string passport, string number, string dateIn, string dateOut) {
        this->passport = passport;
        this->number = number;
        this->dateIn = dateIn;
        this->dateOut = dateOut;
        this->prev = this->next = NULL;
    }
};

// лист для заселения/выселения
class CIOList {
public:
    CheckInOut* head;
    CheckInOut* tail;

    CIOList() {
        head = tail = NULL;
    }

    ~CIOList() {
        while (head != NULL)
            pop_front();
    }

    CheckInOut* push_front(string passport, string number, string dateIn, string dateOut) {
        CheckInOut* ptr = new CheckInOut(passport, number, dateIn, dateOut);
        ptr->next = head;
        if (head != NULL)
            head->prev = ptr;
        if (tail == NULL)
            tail = ptr;
        head = ptr;

        return ptr;
    }

    CheckInOut* push_back(string passport, string number, string dateIn, string dateOut) {
        CheckInOut* ptr = new CheckInOut(passport, number, dateIn, dateOut);
        ptr->prev = tail;
        if (tail != NULL)
            tail->next = ptr;
        if (head == NULL)
            head = ptr;
        tail = ptr;

        return ptr;
    }

    void pop_front() {
        if (head == NULL) return;

        CheckInOut* ptr = head->next;
        if (ptr != NULL)
            ptr->prev = NULL;
        else
            tail = NULL;

        delete head;
        head = ptr;
    }

    void pop_back() {
        if (tail == NULL) return;

        CheckInOut* ptr = tail->prev;
        if (ptr != NULL)
            ptr->next = NULL;
        else
            head = NULL;

        delete tail;
        tail = ptr;
    }

    bool find(string pass) {
        if (head == NULL) return 0;
        CheckInOut* ptr = head;

        while (true) {
            if (ptr->next == NULL)
                break;
            if (ptr->passport == pass)
                return 1;
            ptr = ptr->next;
        }
       
        return 0;
    }
};

// лист для хэша
class HList { // класс связного списка сегмента 
private:
    Guest* head; // 1-ый элемент
    int size; // размер

public:
    HList() { // конструктор по умолчанию
        head = nullptr;
        size = 0;
    }

    ~HList() { // деструктор
        Guest* current = head;
        while (current) {
            Guest* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void push_back(string passport, string fio, int year, string address, string purpose) { // добавление нового элемента
        if (head == NULL) {
            Guest* newNode = new Guest(passport, fio, year, address, purpose);
            head = newNode;
            size++;
        }
        else if (head->passport == passport) {
            head->fio = fio;
            head->year = year;
            head->address = address;
            head->purpose = purpose;
        }
        else {
            Guest* current = head;
            bool flag = 1;
            while (current->next) {
                if (current->passport == passport) {
                    current->fio = fio;
                    current->year = year;
                    current->address = address;
                    current->purpose = purpose;
                    flag = 0;
                    break;
                }
                current = current->next;
            }
            if (current->passport == passport) {
                current->fio = fio;
                current->year = year;
                current->address = address;
                current->purpose = purpose;
                flag = 0;
            }
            if (flag) {
                Guest* newNode = new Guest(passport, fio, year, address, purpose);
                current->next = newNode;
                size++;
            }
        }

    }

    bool remove(string passport) { // удаление элемента
        if (!head) 
            return 0;

        if (head->passport == passport) {
            Guest* temp = head;
            head = head->next;
            delete temp;
            size--;
            return 1;
        }

        Guest* current = head;
        while (current->next) {
            if (current->next->passport == passport) {
                Guest* temp = current->next;
                current->next = current->next->next;
                delete temp;
                size--;
                return 1;
            }
            current = current->next;
        }
        return 0;
    }

    bool removeAll() {
        if (head == NULL) 
            return 0;

        Guest* current = head;
        while (current) {
            Guest* temp = current;
            current = current->next;
            delete temp;
            size--;
        }
        head = NULL; // устанавливаем указатель на голову в NULL для пустого списка
        return 1;
    }

    Guest* find(string pass) { // поиск по ключу
        Guest* current = head;
        while (current) {
            if (current->passport == pass) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void findP(string pass, int& count) { // поиск по ключу
        Guest* current = head;
        while (current) {
            if (current->passport == pass) {
                GuestInfo(current);
                count++;
            }
            current = current->next;
        }
    }

    void findF(string fio, int& count) { // поиск по ключу
        Guest* current = head;
        while (current) {
            if (current->fio == fio) {
                GuestInfo(current);
                count++;
            }
            current = current->next;
        }
    }

    Guest* findByName(string fio) {
        Guest* current = head;
        while (current) {
            if (current->fio == fio) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void display() { // вывод списка
        Guest* current = head;
        while (current) {
            cout << "-----------------------------------------" << endl;
            cout << "постоялец: " << current->fio << endl;
            cout << "паспорт: " << current->passport << endl;
            cout << "год рождения: " << current->year << endl;
            cout << "место проживания: " << current->address << endl;
            cout << "цель прибытия: " << current->purpose << endl;
            current = current->next;
        }
    }
};

int GuestToHash(string passport, int size) { // хеш-функция
    int hash = 0;
    for (int i = 0; i < 6; i++)
        hash += pow(double(passport[i]), 2);
    hash %= size;
    return hash;
}

class HashTable { // класс хеш-таблицы
private:
    HList* table; // массив сегментов
public:
    HashTable(int size) { // конструктор с заданным размером таблицы
        table = new HList[size];
    }

    ~HashTable() { // деструктор
        delete[] table;
    }

    void print() { // вывод всей таблицы
        for (int i = 0; i < hashTableSize; i++) {
            table[i].display();
        }
        cout << "-----------------------------------------";
    }

    bool add(string passport, string fio, int year, string address, string purpose) { // добавление элемента
        int hash = GuestToHash(passport, hashTableSize);
          table[hash].push_back(passport, fio, year, address, purpose);
          return 1;
    }

    bool remove(string passport) { // удаление элемента
        return table[GuestToHash(passport, hashTableSize)].remove(passport);

    }

    bool removeAll() {
        for (int i = 0; i < hashTableSize; i++) {
            table[i].removeAll();
        }
        return 1;
    }

    bool isFind(string pass) { // поиск по ключу
        int count = 0;
        int hash = GuestToHash(pass, hashTableSize);
        if (table[hash].find(pass) != NULL) 
            count++;
        if (count == 0) 
            return 0;
        return 1;
    }

    void find(string pass) { // поиск по ключу
        int count = 0;
        int hash = GuestToHash(pass, hashTableSize);
        if (table[hash].find(pass) != NULL) 
            table[hash].findP(pass, count);

        if (count == 0) {
            cout << "-----------------------------------------" << endl;
            cout << "постоялец не найден" << endl;
        }
    }

    void findByName(string fio) {
        int count = 0;
        for (int i = 0; i < hashTableSize; i++ ) {
            if (table[i].findByName(fio) != NULL) {
                table[i].findF(fio, count);
            }
        }
        if (count == 0) {
            cout << "-----------------------------------------" << endl;
            cout << "постоялец не найден" << endl;
        }
    }
};

//структура номера
struct Room {
    string number; //номер номера
    int num;
    int people; //кол-во мест
    int freePeople; //кол-во свободных мест 
    int rooms; //кол-во комнат
    bool toilet; //наличие санузла
    string equipment; //оборудование в номере
    Room* left;
    Room* right;
    int height;

    Room(string number, int num, int people, int freePeople, int rooms, bool toilet, string equipment) {
        this->number = number;
        this->num = num;
        this->people = people;
        this->freePeople = freePeople;
        this->rooms = rooms;
        this->toilet = toilet;
        this->equipment = equipment;
        this->left = nullptr;
        this->right = nullptr;
        height = 1;
    }

    Room() {
        number = "";
        num = 0;
        people = 0;
        freePeople = 0;
        rooms = 0;
        toilet = true;
        equipment = "";
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

void RoomInfo(Room* room) {
    cout << "-----------------------------------------" << endl;
    cout << "номер №: " << room->number << endl;
    cout << "количество мест: " << room->people << endl;
    cout << "количество свободных мест: " << room->freePeople << endl;
    cout << "количество комнат: " << room->rooms << endl;

    if (room->toilet)
        cout << "наличие санузла: +" << endl;
    else
        cout << "наличие санузла: -" << endl;

    cout << "оборудование в номере: " << room->equipment << endl;
}

//структура авл (хранит гостиничные номера)
struct Tree {
    Room* root; //данные о гостиничном номере
   
    int getHeight(Room* root) {
        return root ? root->height : 0;
    }

    int getBalance(Room* root) {
        return getHeight(root->left) - getHeight(root->right);
    }

    void fixHeight(Room* root) {
        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    }

    Room* rightRotate(Room* root) {
        Room* left = root->left;
        Room* leftRight = left->right;

        left->right = root;
        root->left = leftRight;

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
        left->height = max(getHeight(left->left), getHeight(left->right)) + 1;

        return left;
    }

    Room* leftRotate(Room* root) {
        Room* right = root->right;
        Room* rightLeft = right->left;

        right->left = root;
        root->right = rightLeft;

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
        right->height = max(getHeight(right->left), getHeight(right->right)) + 1;

        return right;
    }

    /*Room* insert(Room* root, string number, int people, int freePeople, int rooms, bool toilet, string equipment, int n) {
        if (!root) {
            return new Room(number, n, people, freePeople, rooms, toilet, equipment);
        }

        if (n < root->num) {
            root->left = insert(root->left, number, people, freePeople, rooms, toilet, equipment, n);
        }
        else if (n > root->num) {
            root->right = insert(root->right, number, people, freePeople, rooms, toilet, equipment, n);
        }
        else {
            return root;
        }

        fixHeight(root);

        int balance = getBalance(root);

        if (balance > 1 && n < root->left->num)
            return rightRotate(root);

        if (balance < -1 && n > root->right->num)
            return leftRotate(root);

        if (balance > 1 && n > root->left->num) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && n < root->right->num) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }*/

    Room* insert(Room* root, string number, int people, int freePeople, int rooms, bool toilet, string equipment, int n) {
        if (!root) {
            return new Room(number, n, people, freePeople, rooms, toilet, equipment);
        }

        if (n < root->num) {
            root->left = insert(root->left, number, people, freePeople, rooms, toilet, equipment, n);
        }
        else if (n > root->num) {
            root->right = insert(root->right, number, people, freePeople, rooms, toilet, equipment, n);
        }
        else {
            return root;
        }

        fixHeight(root);

        int balance = getBalance(root);

        if (balance > 1) {
            if (n < root->left->num) {
                return rightRotate(root);
            }
            else {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
        }

        if (balance < -1) {
            if (n > root->right->num) {
                return leftRotate(root);
            }
            else {
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }
        }

        return root;
    }

    Room* getMin(Room* root) {
        Room* current = root;
        while (current->left) {
            current = current->left;
        }
        return current;
    }

    Room* deleteRecursive(Room* root, string number) {
        if (!root) {
            return root;
        }

        if (number < root->number) {
            root->left = deleteRecursive(root->left, number);
        }
        else if (number > root->number) {
            root->right = deleteRecursive(root->right, number);
        }
        else {
            if (!root->left || !root->right) {
                Room* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp;
                }
                delete temp;
            }
            else {
                Room* temp = getMin(root->right);
                root->number = temp->number;
                root->right = deleteRecursive(root->right, temp->number);
            }
        }

        if (!root) {
            return root;
        }

        fixHeight(root);

        int balance = getBalance(root);

        if (balance > 1) {
            if (getBalance(root->left) >= 0) {
                return rightRotate(root);
            }
            else {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
        }

        if (balance < -1) {
            if (getBalance(root->right) <= 0) {
                return leftRotate(root);
            }
            else {
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }
        }

        return root;
    }


public:
    Tree() {
        root = nullptr;
    }

    void insert(string number, int people, int freePeople, int rooms, bool toilet, string equipment, int n) {
        root = insert(root, number, people, freePeople, rooms, toilet, equipment, n);
    }

    void deleteNode(string number) {
        root = deleteRecursive(root, number);
    }

    void search(string number) { // Метод для поиска элемента в дереве по ключу
        int steps = 0;
        Room* current = root;

        while (current != nullptr) {
            steps++;
            if (number == current->number) {
                cout << "элемент " << number << " найден за " << steps << " шагов" << endl;
                return;
            }
            else if (number < current->number) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        cout << "элемента " << number << " нет в дереве" << endl;
    }

    void printPreOrderRecursive(Room* root) { // Рекурсивный метод для обратного обхода дерева
        if (root == nullptr) {
            return;
        }
        printPreOrderRecursive(root->left);
        printPreOrderRecursive(root->right);
        RoomInfo(root);
    }
};

bool BMSearch(string str, string substr) {
    int lenSub = substr.length();
    int lenStr = str.length();

    if (lenStr == 0 || lenSub == 0)
        return false;
    else if (lenStr == 1 || lenSub == 1) {
        if (lenStr == 1) {
            lenStr += 1;
            str += " ";
        }
        if (lenSub == 1) {
            lenSub += 1;
            substr += " ";
        }
        int* arr = new int[256];
        for (int i = 0; i < 256; i++) {
            arr[i] = lenSub;
        }
        for (int i = lenSub - 1; i >= 0; i--) {
            if (arr[(int((unsigned char)(substr[i])))] == lenSub)
                arr[(int((unsigned char)(substr[i])))] = lenSub - i - 1;
        }
        int pos = lenSub - 1;
        while (pos < lenStr) {
            if (substr[lenSub - 1] != str[pos])
                pos += arr[(int((unsigned char)(str[pos])))];
            else {
                for (int i = lenSub - 2; i >= 0; i--) {
                    if (substr[i] != str[pos - lenSub + i + 1]) {
                        pos += arr[(int((unsigned char)(str[pos - lenSub + i + 1])))] - 1;
                        break;
                    }
                    else {
                        if (i == 0)
                            return true;
                    }
                }
            }
        }
    }
    else {
        int* arr = new int[256];
        for (int i = 0; i < 256; i++) {
            arr[i] = lenSub;
        }
        for (int i = lenSub - 1; i >= 0; i--) {
            if (arr[(int((unsigned char)(substr[i])))] == lenSub)
                arr[(int((unsigned char)(substr[i])))] = lenSub - i - 1;
        }
        int pos = lenSub - 1;
        while (pos < lenStr) {
            if (substr[lenSub - 1] != str[pos])
                pos += arr[(int((unsigned char)(str[pos])))];
            else {
                for (int i = lenSub - 2; i >= 0; i--) {
                    if (substr[i] != str[pos - lenSub + i + 1]) {
                        pos += arr[(int((unsigned char)(str[pos - lenSub + i + 1])))] - 1;
                        break;
                    }
                    else {
                        if (i == 0)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

void shakeSort(CIOList* list) {
    bool swapped = true;
    CheckInOut* head = list->head;
    CheckInOut* tail = nullptr;

    while (swapped) {
        swapped = false;

        // Проход слева направо
        CheckInOut* current = head;
        while (current->next != tail) {
            if (current->number > current->next->number) {
                swap(current, current->next);
                swapped = true;
            }
            current = current->next;
        }

        tail = current; // Устанавливаем новый конец для следующего прохода

        if (!swapped) {
            break; // Выход из цикла, если не было перестановок
        }

        swapped = false;

        // Проход справа налево
        current = tail;
        while (current->prev != head && current->prev != nullptr) {
            if (current->number < current->prev->number) {
                swap(current, current->prev);
                swapped = true;
            }
            current = current->prev;
        }

        head = current->next; // Устанавливаем новое начало для следующего прохода
    }
}

void checkFIO(string& fio) {
    cout << "введите ФИО постояльца: ";
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    getline(cin, fio);

    bool flag = 0;
    for (int i = 0; i < fio.length(); i++) {
        int s = int(fio[i]);
        if ((s >= -64 && s <= -1) || (s == -72) || (s == -88) || (s == 32) || (s == 45))
            flag = 0;
        else if (!isalpha(fio[i])) {
            flag = 1;
            break;
        }
    }

    while (flag == 1) {
        int s = 0;
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << "недопустимые символы.";
        cout << endl;
        cout << "введите ФИО постояльца повторно: ";
        getline(cin, fio);
        for (int i = 0; i < fio.length(); i++) {
            int f = int(fio[i]);
            if ((f >= -64 && f <= -1) || (f == -72) || (f == -88) || (f == 32) || (f == 45))
                s += 1;
            else if (isalpha(fio[i]))
                s += 1;
        }
        if (s == fio.length())
            flag = 0;
    }
}

void checkYearG(int& year) {
    cout << "введите год рождения (1900-2024): ";
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    cin >> year;
    while (true) {
        if (year > 1899 && year < 2025)
            break;
        else {
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            cout << "введите год рождения повторно (1900-2024): ";
            cin >> year;
        }
    }
}

void checkPassport(string& passport) {
    cout << "введите номер паспорта NNNN-NNNNNN (где N - цифра): ";
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    getline(cin, passport);
    while (true) {
        if ((int)passport[0] >= 48 && (int)passport[0] <= 57 && (int)passport[1] >= 48 && (int)passport[1] <= 57 && (int)passport[2] >= 48 && (int)passport[2] <= 57 && (int)passport[3] >= 48 && (int)passport[3] <= 57 && (int)passport[4] == 45 && (int)passport[5] >= 48 && (int)passport[5] <= 57 && (int)passport[6] >= 48 && (int)passport[6] <= 57 && (int)passport[7] >= 48 && (int)passport[7] <= 57 && (int)passport[8] >= 48 && (int)passport[8] <= 57 && (int)passport[9] >= 48 && (int)passport[9] <= 57 && (int)passport[10] >= 48 && (int)passport[10] <= 57 && passport.size() == 11)
            break;
        else {
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            cout << "введите номер паспорта повторно NNNN-NNNNNN (где N - цифра): ";
            getline(cin, passport);
        }
    }
}

void checkRoom(string& num) {
    cout << "введите номер комнаты «ANNN» где A – тип номера, N - цифры: ";
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    getline(cin, num);
    while (true) {
        if (num[0] == 'Л' || num[0] == 'П' || num[0] == 'О' || num[0] == 'М') {
            if (num.size() == 4) {
                if ((int)num[1] >= 48 && (int)num[1] <= 57 && (int)num[2] >= 48 && (int)num[2] <= 57 && (int)num[3] >= 48 && (int)num[3] <= 57)
                    break;
            }
        }
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << "введите номер комнаты повторно «ANNN» где A – тип номера, N - цифры: ";
        getline(cin, num);
    }
}

bool checkDay(char& d1, char& d2, char& m1, char& m2) {
    if (d1 == '0') {
        if (d2 >= '1' && d2 <= '9')
            return true;
    }
    else if (d1 == '1' || d1 == '2') {
        if (d2 >= '0' && d2 <= '9')
            return true;
    }
    else if (d1 == '3') {
        if ((m1 == '0' && (m2 == '1' || m2 == '3' || m2 == '5' || m2 == '7' || m2 == '8')) || (m1 == '1' && (m2 == '0' || m2 == '2'))) {
            if (d2 == '1' || d2 == '0')
                return true;
        }
        else if ((m1 == '0' && (m2 == '4' || m2 == '6' || m2 == '9')) || (m1 == '1' && m2 == '1')) {
            if (d2 == '0')
                return true;
        }
    }
    return false;
}

bool checkMonth(char& m1, char& m2) {
    if (m1 == '0') {
        if (m2 >= '1' && m2 <= '9')
            return true;
    }
    else if (m1 == '1') {
        if (m2 >= '0' && m2 <= '2')
            return true;
    }
    return false;
}

bool checkYear(char& y1, char& y2, char& y3, char& y4) {
    if (y1 == '2') {
        if (y2 == '0') {
            if (y3 == '0' || y3 == '1') {
                if (y4 >= 0 && y4 <= '9')
                    return true;
            }
            if (y3 == '2') {
                if (y4 >= '0' && y4 <= '4')
                    return true;
            }
        }
    }
    return false;
}

bool checkLeapYear(string& date) {
    int y1 = date[6] - '0'; // преобразование символа в число
    int y2 = date[7] - '0';
    int y3 = date[8] - '0';
    int y4 = date[9] - '0';
    if ((y1 + y2 + y3 + y4) % 4 == 0) { // если год - високосный
        if (date[3] == '0' && date[4] == '2') { // если месяц - февраль
            if (date[0] == '3') {
                return false;
            }
        }
    }
    else {
        if (date[3] == '0' && date[4] == '2') { // если месяц - февраль 
            if (date[0] == '2') {
                if (date[1] >= '0' && date[1] <= '8')
                    return true;
                else
                    return false;
            }
            else if (date[0] == '3')
                return false;
            else
                return true;
        }
        else
            return true;
    }
    return true;
}

// проверка на то, что дата заезда раньше даты выезда
bool checkOk(string& in, string& out) {
    if ((out[8] == '0' || out[8] == '1') && (in[8] == '9' || in[8] == '8')) // если десятилетие выселения больше десятилетия выселения
                return true;
    
    else if (out[8] == in[8]) { // если десятилетие одно
        if (out[9] == in[9]) { // если год один
            if (out[3] >= in[3] && out[4] >= in[4] && !(in[3] == out[3] && in[4] == out[4])) // если месяц выселения > месяца заселения
                return true;

            else if (in[3] == out[3] && in[4] == out[4]) { // если мeсяц один
                if (out[0] > in[0])
                    return true;
                else if (out[0] == in[0] && out[1] > in[1])
                    return true;
            }
        }
        else if (out[9] < in[9]) // если год выселения больше года заселения
            return true;
    }
 
    return false;
}

void checkDataIn(string& in) {
    cout << "введите дату заселения (ДД.ММ.ГГГГ): ";
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    getline(cin, in);
    while (true) {
        if (size(in) == 10) {
            if (in[2] == '.' && in[5] == '.') {
                if (checkDay(in[0], in[1], in[3], in[4]) && checkMonth(in[3], in[4]) && checkYear(in[6], in[7], in[8], in[9])) {
                    if (checkLeapYear(in))
                        break;
                }
            }
        }
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << "введите дату заселения (ДД.ММ.ГГГГ) повторно: ";
        getline(cin, in);
    }
}

void checkDataOut(string& in, string& out) {
    cout << "введите дату выселения (ДД.ММ.ГГГГ): ";
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    getline(cin, out);
    while (true) {
        if (size(out) == 10) {
            if (out[2] == '.' && out[5] == '.') {
                if (checkDay(out[0], out[1], out[3], out[4]) && checkMonth(out[3], out[4]) && checkYear(out[6], out[7], out[8], out[9])) {
                    if (checkLeapYear(out)) {
                        if (checkOk(in, out))
                            break;
                    }
                }
            }
        }
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << "введите дату выселения (ДД.ММ.ГГГГ) повторно: ";
        getline(cin, out);
    }
}

// проврека, проживает ли кто-то в номере по паспорту
bool checkGuestInRoomByPassport(CIOList* list, string p) {
    CheckInOut* h = list->head;
    while (h != NULL) {
        if (h->passport == p) {
            return 1;
        }
        h = h->next;
    }
    return 0;
}

void addGuest(HashTable& HTable) {
    Guest* g = new Guest;

    checkFIO(g->fio);
    checkYearG(g->year);
    checkPassport(g->passport);
    if (HTable.isFind(g->passport)) {
        cout << "гость с таким паспортом уже зарегистрирован" << endl;
    }
    else {
        cout << "введите адрес проживания: ";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        getline(cin, g->address);

        cout << "введите цель прибытия: ";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        getline(cin, g->purpose);

        HTable.add(g->passport, g->fio, g->year, g->address, g->purpose);
    }
}

void dltGuest(HashTable& HTable, CIOList* list) {

    string p;
    checkPassport(p);
    if (checkGuestInRoomByPassport(list, p)) {
        cout << "-----------------------------------------";
        cout << endl << "нельзя удалить постояльца, так как он еще не выселился";
        return;
    }

    if (HTable.isFind(p)) {
        HTable.remove(p);
        cout << "-----------------------------------------";
        cout << endl << "постоялец удален";
    }
    else {
        cout << "-----------------------------------------";
        cout << endl << "постояльца с таким паспортом нет";
    }
}

void clrGuests(HashTable& HTable, CIOList* list) {
    if (list->head != NULL) {
        cout << "-----------------------------------------";
        cout << endl << "нельзя удалить всех постояльцев, так как кто-то из них проживает в номерах";
        return;
    }
    else {
        HTable.removeAll();
        cout << "-----------------------------------------";
        cout << endl << "постояльцы удвлены";
    }
}

void findGuestByPassport(HashTable& HTable) {
    string p;
    checkPassport(p);
    HTable.find(p);
}

void findGuestByName(HashTable& HTable) {
    string name;
    checkFIO(name);
    HTable.findByName(name);
}

string newNumberofNumber() {
    string key = "";
    int n;
    cout << "доступные категории номеров:" << endl;
    cout << "1. Л - люкс" << endl;
    cout << "2. П - полулюкс" << endl;
    cout << "3. О - одноместный" << endl;
    cout << "4. М - многоместный" << endl;

    cout << "выберите необхродимую категорию (введите её порядковый номер): ";
    cin >> n;
    while (true) {
        if (cin.fail())
            cout << "недопустимые символы. попробуйте снова." << endl << "введите категорию повторно: ";
        else if (n != 1 && n != 2 && n != 3 && n != 4)
            cout << "неизвестная команда. попробуйте снова." << endl << "введите категорию повторно: ";
        else
            break;

        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cin >> n;
    }
    switch (n) {
    case 1:
        key.append("Л");
        break;
    case 2:
        key.append("П");
        break;
    case 3:
        key.append("О");
        break;
    case 4:
        key.append("М");
        break;
    }
    if (counterRooms <= 9) {
        key += "00";
        key += to_string(counterRooms);
    }
    else if (counterRooms >= 10 && counterRooms <= 99) {
        key += "0";
        key += to_string(counterRooms);

    }
    else {
        key += to_string(counterRooms);
    }
    return key;

}

bool isBathroom() {
    cout << endl;
    cout << "наличие санузла" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "1. есть" << endl;
    cout << "2. нет" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Ваш выбор: ";
    int k;
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    cin >> k;
    while (true) {
        if (cin.fail())
            cout << "недопустимые символы. попробуйте снова." << endl << "введите Ваш выбор: ";
        else if (k != 2 && k != 1)
            cout << "неизвестная команда. попробуйте снова." << endl << "введите Ваш выбор: ";
        else
            break;

        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cin >> k;
    }
    if (k == 1)
        return true;
    else
        return false;
}

void addRoom(Tree* tree) {
    string number = newNumberofNumber();
    Room* r = new Room;
    r->number = number;
    if (r->number[0] == 'О') { // одноместный
        r->people = 1;
        r->rooms = 1;
    }
    else if (r->number[0] == 'П') { // полулюкс
        r->people = 3;
        r->rooms = 2;
    }
    else if (r->number[0] == 'Л') { // люкс 
        r->people = 4;
        r->rooms = 3;
    }
    else if (r->number[0] == 'М') { // многоместный 
        r->people = 5;
        r->rooms = 4;
    }
    r->freePeople = r->people;
    r->toilet = isBathroom();
    r->num = counterRooms;

    cout << endl << "введите оборудование, доступное в номере: ";

    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    getline(cin, r->equipment);

    tree->insert(r->number, r->people, r->freePeople, r->rooms, r->toilet, r->equipment, r->num);
    counterRooms++;

    cout << endl << "номер " << number << " успешно добавлен!" << endl;
}

// Проверка, живет ли кто-то в номере
bool checkRoomWithGuest(CIOList* list, string room) {
    CheckInOut* h = list->head;
    while (h != NULL) {
        if (h->number == room)
            return 1;
        h = h->next;
    }
    return 0;
}

void dltRoom(Tree* tree, string num, CIOList* list) {
    checkRoom(num);
    if (checkRoomWithGuest(list, num)) {
        cout << "-----------------------------------------";
        cout << endl << "невозможно удалить номер, т. к. в нем проживают" << endl;
        return;
    }

    if (tree == nullptr) {
        cout << "-----------------------------------------";
        cout << endl << "номер с таким номером отсутствует" << endl;
    }
    else {
        tree->deleteNode(num);
        cout << "-----------------------------------------" << endl;
        cout << "номер удален";
    }
}

void clrRooms(Tree* tree, Room* room, CIOList* list) {
    if (list->head != NULL) {
        cout << "-----------------------------------------" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "нельзя удалить все номера, так как не все номера свободны";
        return;
    }
    if (tree == NULL)
        return;

    if (tree->root == nullptr)
        return;

    tree->deleteNode(tree->root->number);
    counterRooms--;


    if (tree->root != nullptr)
        clrRooms(tree, tree->root->right, list);
    if (tree->root != nullptr)
        clrRooms(tree, tree->root->left, list);


}

// Поиск сожителей по номеру комнаты
void searchRoommates(HashTable& HTable, string passport) {
    int count = 0;
    HTable.find(passport);
}

// Поиск паспорта по номеру
void searchPassportByRoom(CIOList* list, HashTable& HTable, string num) {
    int check = 0;
    CheckInOut* h = list->head;
    while (h != NULL) {
        if (h->number == num) {
            check++;
            searchRoommates(HTable, h->passport);
        }
        h = h->next;
    }
    if (check == 0) {
        cout << "-----------------------------------------" << endl;
        cout << "в номере №" << num << " постояльцы отсутствуют";
    }
}

void findRoomByNumber(Room* root, string num, CIOList* list, HashTable& HTable) {
    if (root == nullptr)
        return;
    findRoomByNumber(root->left, num, list, HTable);
    findRoomByNumber(root->right, num, list, HTable);
    if (root->number == num) {
        RoomInfo(root);
        searchPassportByRoom(list, HTable, num);
    }
}

void findRoomByEquipment(Room* root, string eq, HashTable& HTable) {
    if (root == NULL)
        return;
    findRoomByEquipment(root->left, eq, HTable);
    findRoomByEquipment(root->right, eq, HTable);
    if (BMSearch(root->equipment, eq)) {
        RoomInfo(root);
    }
}

// поиск комнаты по номеру и вычитание места
void searchRoomAndMinus(Tree* tree, Room* room, string num, int& count, CheckInOut* append) {
    if (tree == NULL)
        return;



    if (tree->root->number == num) {
        count++;
        if (tree->root->freePeople > 0) {
            tree->root->freePeople--;
            cout << "-----------------------------------------";
            cout << endl << "заселение зарегистрировано" << endl;
            return;
        }
        else {
            delete append;
            append = nullptr;
            cout << "-----------------------------------------";
            cout << endl << "в комнате нет мест" << endl;
            return;
        }
    }
    if (tree->root != nullptr) {
        searchRoomAndMinus(tree, tree->root->right, num, count, append);
        searchRoomAndMinus(tree, tree->root->left, num, count, append);
    }
}

void checkIn(CIOList* list, Tree* tree, HashTable& HTable) {
    if (list->head == NULL) {
        CheckInOut* append = new CheckInOut;
        checkPassport(append->passport);
        checkRoom(append->number);
        if (HTable.isFind(append->passport)) {
            // вводим даты заселения/выселения
            string in, out;
            checkDataIn(in);
            checkDataOut(in, out);

            int count = 0;
            searchRoomAndMinus(tree, tree->root, append->number, count, append);
            
            if (count == 0) {
                cout << "-----------------------------------------";
                cout << endl << "такого номера не существует" << endl;
                delete append;
                append = nullptr;
                return;
            }
            else {
                append;
            }
        }
        else {
            cout << "-----------------------------------------";
            cout << endl << "такого паспорта нет в базе" << endl;
            return;
        }
        if (append != NULL) {
            list->push_back(append->passport, append->number, append->dateIn, append->dateOut);;
        }
    }
    else {
        CheckInOut* append = new CheckInOut;
        checkPassport(append->passport);
        checkRoom(append->number);

        if (HTable.isFind(append->passport)) {
            // вводим даты заселения/выселения
            string in, out;
            checkDataIn(in);
            checkDataOut(in, out);

            int count = 0;
            searchRoomAndMinus(tree, tree->root, append->number, count, append);

            if (count == 0) {
                cout << "-----------------------------------------";
                cout << endl << "такого номера не существует" << endl;
                delete append;
                append = nullptr;
                return;
            }
        }
        else {
            cout << "-----------------------------------------";
            cout << endl << "такого паспорта нет в базе" << endl;
            return;
        }
        if (append != NULL) {
            CheckInOut* h = list->head;
            while (true) {
                if (h->next == NULL) {
                    h->next = append;
                    list->tail = append;
                    break;
                }
                h = h->next;
            }
        }
    }
}

// проврека, есть ли такой номер комнаты
void searchRoomNumber(Tree* tree, Room* r, string room, int& check) {
    if (tree == NULL)
        return;

    if (tree->root->number == room) {
        check++;
        return;
    }

    if (tree->root != nullptr) {
        searchRoomNumber(tree, tree->root->right, room, check);
        searchRoomNumber(tree, tree->root->left, room, check);

    }
}

//поиск номера для добавления места
void searchRoomAndPlus(Tree* tree, Room* r, string room) {
    if (tree == NULL)
        return;

    if (tree->root != nullptr)
       

    if (tree->root->number == room) {
        tree->root->freePeople++;
        return;
    }

    if (tree->root != nullptr) {
        searchRoomAndPlus(tree, tree->root->right, room);
        searchRoomAndPlus(tree, tree->root->left, room);
    }
}

void checkOut(CIOList* list, Tree* tree, HashTable& HTable) {
    string p, n;
    checkPassport(p);
    checkRoom(n);
    int check = 0;
    searchRoomNumber(tree, tree->root, n, check);
    if (!HTable.isFind(p) && check) {
        cout << "-----------------------------------------";
        cout << endl << "паспорта нет в базе" << endl;
        return;
    }
    CheckInOut* h = list->head;
    while (h != NULL) {
        if (h->number == n && h->passport == p) {
            if (list->head == list->tail) { // если всего 1 элемент
                delete list->head;
                list->head = nullptr;
                list->tail = nullptr;
                searchRoomAndPlus(tree, tree->root, n);
                cout << "-----------------------------------------";
                cout << endl << "место в комнате добавлено, постоялец выселен" << endl;
                return;
            }
            else if (h == list->head && h->next != NULL) {// если удаляем первый элемент
                list->head = h->next;
                delete h;
                h = nullptr;
                h = list->head;
                searchRoomAndPlus(tree, tree->root, n);
                cout << "-----------------------------------------";
                cout << endl << "место в комнате добавлено, постоялец выселен" << endl;
                return;
            }
            else if (h == list->tail) { // если удаляем последний элемент
                CheckInOut* last = list->head;
                h = list->head;
                while (h != NULL) {
                    if (h->next == list->tail) {
                        last = h->next;
                        h->next = NULL;
                        break;
                    }
                    h = h->next;
                }
                delete last;
                last = nullptr;
                list->tail = nullptr;
                h = list->head;
                while (h != NULL) {
                    if (h != NULL) {
                        list->tail = h;
                        break;
                    }
                    h = h->next;
                }
                searchRoomAndPlus(tree, tree->root, n);
                cout << "-----------------------------------------";
                cout << endl << "место в комнате добавлено, постоялец выселен" << endl;
                return;
            }
            else { // если удаляем элемент откуда-то из центра
                CheckInOut* delElem = h; // запоминаем удаляемый элемент
                CheckInOut* nextElem = h->next; // запоминаем следующий элемент за удаляемым
                h = list->head;
                // бежим, пока не встретим следующим элементом удаляемый
                while (h != NULL) {
                    if (h->next == delElem) {
                        h->next = nextElem; // помещаем следующий элемент разрывая цепочку
                        delete delElem;
                        delElem = nullptr;
                        break;
                    }
                    h = h->next;
                }
                searchRoomAndPlus(tree, tree->root, n);
                cout << "-----------------------------------------";
                cout << endl << "место в комнате добавлено, постоялец выселен" << endl;
                return;
            }
        }
        h = h->next;
    }
    cout << "-----------------------------------------";
    cout << endl << "данный номер не содержится в списке выданных" << endl;
}

void menu(HashTable& HTable, CIOList* list, Tree* tree) {
    // везде добавить возвращение в главное меню вместо выбранного действия 

    int choice = 0;
    string num, eq, room;
    while (choice != 15) {
        switch (choice) {
        case 1:
            cout << endl << "регистрация нового постояльца" << endl;
            addGuest(HTable);
            break;
        case 2:
            cout << endl << "удаление постояльца" << endl;
            dltGuest(HTable, list);
            cout << endl;
            break;
        case 3:
            cout << endl << "просмотр всех зарегистрированных постояльцев" << endl;
            HTable.print();
            break;
        case 4:
            cout << endl << "очистка данных о постояльцах" << endl;
            clrGuests(HTable, list);
            cout << endl;
            break;
        case 5:
            cout << endl << "поиск постояльца по номеру паспорта" << endl;
            findGuestByPassport(HTable);
            break;
        case 6:
            cout << endl << "поиск постояльца по ФИО" << endl;
            findGuestByName(HTable);
            break;
        case 7:
            cout << endl << "добавление нового гостиничного номера" << endl;
            addRoom(tree);
            break;
        case 8:
            cout << endl << "удаление сведений о гостиничном номере" << endl;
            dltRoom(tree, num, list);
            cout << endl;
            break;
        case 9:
            cout << endl << "просмотр всех имеющихся гостиничных номеров" << endl;
            tree->printPreOrderRecursive(tree->root);
            cout << endl;
            break;
        case 10:
            cout << endl << "очистка данных о гостиничных номерах" << endl;
            if (tree->root == nullptr) {
                cout << "-----------------------------------------" << endl;
                cout << "номера в базе отсутствуют" << endl;
            }
            else {
                clrRooms(tree, tree->root, list);
                cout << "-----------------------------------------" << endl;
                cout << "номера успешно удалены" << endl;
            }
            break;
        case 11:
            cout << endl << "поиск гостиничного номера по «номеру гостиничного номера»" << endl;
            checkRoom(room);
            findRoomByNumber(tree->root, room, list, HTable);
            cout << endl;
            break;
        case 12:
            cout << endl << "поиск гостиничного номера по разделу «оборудование»" << endl;
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            cout << "введите оборудование, по которому необходимо осуществить поиск: ";
            cin >> eq;
            findRoomByEquipment(tree->root, eq, HTable);
            cout << endl;
            break;
        case 13: 
            cout << endl << "регистрация вселения постояльца" << endl;
            checkIn(list, tree, HTable);
            cout << endl;
            break;
        case 14:
            cout << endl << "регистрация выселения постояльца" << endl;
            checkOut(list, tree, HTable);
            cout << endl;
            break;
        case 0:
            cout << "главное меню:" << endl;
            cout << "1. зарегистрировать нового постояльца" << endl;
            cout << "2. удалить постояльца" << endl;
            cout << "3. просмотр всех зарегистрированных постояльцев" << endl;
            cout << "4. очистить данные о постояльцах" << endl;
            cout << "5. поиск постояльца по номеру паспорта" << endl;
            cout << "6. поиск постояльца по ФИО" << endl;
            cout << "7. добавить новый гостиничный номер" << endl;
            cout << "8. удалить сведения о гостиничном номере" << endl;
            cout << "9. просмотр всех имеющихся гостиничных номеров" << endl;
            cout << "10. очистить данные о гостиничных номерах" << endl;
            cout << "11. поиск гостиничного номера по «Номеру гостиничного номера»" << endl;
            cout << "12. поиск гостиничного номера по фрагментам «Оборудования»" << endl;
            cout << "13. регистрация вселения постояльца" << endl;
            cout << "14. регистрация выселения постояльца" << endl;
            cout << "15. завершить работу программы" << endl;
            cout << "0. показать главное меню" << endl;
        }

        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << endl << "введите пункт меню: ";
        cin >> choice;
        while (true) {
            if (cin.fail())
                cout << "недопустимые символы. попробуйте снова." << endl << "введите пункт меню : ";
            else if (choice < 0 || choice > 15)
                cout << "неизвестная команда. попробуйте снова." << endl << "введите пункт меню: ";
            else
                break;

            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
            cin >> choice;
        }
    }
    cout << endl << "завершение работы." << endl;
}

int main() {
    SetConsoleCP(1251); // установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

    setlocale(LC_ALL, "Rus");

    int count = 0;
    HashTable HTable(hashTableSize);
    Tree* tree = new Tree;
    CIOList* list = new CIOList;
    menu(HTable, list, tree);
}
