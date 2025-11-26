
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int MAX_STUDENTS = 100;

struct Student {
    unsigned int StudentNumber; 
    string LastName;            
    string FirstName;           
    string MiddleName;          
    unsigned int BirthYear;     
    int Grade1;                 
    int Grade2;                 
    int Grade3;                 
};


Student SdntArr[MAX_STUDENTS];     
bool DeletedMask[MAX_STUDENTS];    
int IndexByLastName[MAX_STUDENTS]; 
int IndexByAverage[MAX_STUDENTS];  
int SdntCount = 0;                 


double GetAverage(int index) {
    return (SdntArr[index].Grade1 + SdntArr[index].Grade2 + SdntArr[index].Grade3) / 3.0;
}

void InputStudent(int i) {
    cout << "  Nomer zachetki: "; cin >> SdntArr[i].StudentNumber;
    cout << "  Familiya:       "; cin >> SdntArr[i].LastName;
    cout << "  Imya:           "; cin >> SdntArr[i].FirstName;
    cout << "  Otchestvo:      "; cin >> SdntArr[i].MiddleName;
    cout << "  God rozhdeniya: "; cin >> SdntArr[i].BirthYear;
    cout << "  Ocenka 1:       "; cin >> SdntArr[i].Grade1;
    cout << "  Ocenka 2:       "; cin >> SdntArr[i].Grade2;
    cout << "  Ocenka 3:       "; cin >> SdntArr[i].Grade3;
}

void PrintStudent(int i) {
    cout << "N" << SdntArr[i].StudentNumber << " "
         << SdntArr[i].LastName << " " << SdntArr[i].FirstName << " " << SdntArr[i].MiddleName
         << " | God: " << SdntArr[i].BirthYear
         << " | Ocenki: " << SdntArr[i].Grade1 << "," << SdntArr[i].Grade2 << "," << SdntArr[i].Grade3
         << " | Sr.ball: " << GetAverage(i) << endl;
}

void SaveToFile() {
    string filename = "students.csv";
    int mode;
    cout << "Viberyte rezhim: 1 - Noviy fail, 2 - Dobavit v konec > ";
    cin >> mode;

    ofstream f;
    if (mode == 2) {
        f.open(filename, ios::app);
    } else {
        f.open(filename);
    }

    if (f.is_open()) {
        for (int i = 0; i < SdntCount; i++) {
            if (DeletedMask[i] == false) {
                f << SdntArr[i].StudentNumber << ";"
                  << SdntArr[i].LastName << ";"
                  << SdntArr[i].FirstName << ";"
                  << SdntArr[i].MiddleName << ";"
                  << SdntArr[i].BirthYear << ";"
                  << SdntArr[i].Grade1 << ";"
                  << SdntArr[i].Grade2 << ";"
                  << SdntArr[i].Grade3 << endl;
            }
        }
        f.close();
        cout << "Dannye sohraneny v fail " << filename << endl;
    } else {
        cout << "Oshibka otkritiya faila!" << endl;
    }
}

void LoadFromFile() {
    string filename = "students.csv";
    ifstream f(filename);

    if (f.is_open()) {
        cout << "Chtenie iz faila..." << endl;

        while (!f.eof() && SdntCount < MAX_STUDENTS) {

            string tempStr;

            if (f.peek() == EOF) break;

            string val;

            getline(f, val, ';'); 
            if (val == "") break;
            SdntArr[SdntCount].StudentNumber = stoi(val);

            getline(f, SdntArr[SdntCount].LastName, ';');

            getline(f, SdntArr[SdntCount].FirstName, ';');

            getline(f, SdntArr[SdntCount].MiddleName, ';');

            getline(f, val, ';');
            SdntArr[SdntCount].BirthYear = stoi(val);

            getline(f, val, ';');
            SdntArr[SdntCount].Grade1 = stoi(val);

            getline(f, val, ';');
            SdntArr[SdntCount].Grade2 = stoi(val);

            getline(f, val); 
            SdntArr[SdntCount].Grade3 = stoi(val);

            DeletedMask[SdntCount] = false;
            SdntCount++;
        }
        f.close();
        cout << "Zagruzheno studentov: " << SdntCount << endl;
    } else {
        cout << "Fail ne naiden!" << endl;
    }
}

void BuildIndexByLastName() {
    for (int i = 0; i < SdntCount; i++) IndexByLastName[i] = i;

    for (int i = 1; i < SdntCount; i++) {
        int x = IndexByLastName[i];
        int j = i - 1;
        while (j >= 0 && SdntArr[x].LastName < SdntArr[IndexByLastName[j]].LastName) {
            IndexByLastName[j + 1] = IndexByLastName[j];
            j--;
        }
        IndexByLastName[j + 1] = x;
    }
}

// Построение индекса по Среднему баллу (Пункт 6 - Сортировка Пузырьком)
void BuildIndexByAverage() {
    for (int i = 0; i < SdntCount; i++) IndexByAverage[i] = i;

    for (int i = 0; i < SdntCount - 1; i++) {
        for (int j = 0; j < SdntCount - i - 1; j++) {
            if (GetAverage(IndexByAverage[j]) < GetAverage(IndexByAverage[j + 1])) {
                int temp = IndexByAverage[j];
                IndexByAverage[j] = IndexByAverage[j + 1];
                IndexByAverage[j + 1] = temp;
            }
        }
    }
}

void PrintByIndex(int IndexArray[]) {
    for (int i = 0; i < SdntCount; i++) {
        int realIndex = IndexArray[i];
        if (DeletedMask[realIndex] == false) {
            PrintStudent(realIndex);
        }
    }
}

int IterBinarySearchLastName(string key) {
    int L = 0, R = SdntCount - 1;
    while (L <= R) {
        int M = (L + R) / 2;
        int realIndex = IndexByLastName[M];

        if (SdntArr[realIndex].LastName == key) {
            if (DeletedMask[realIndex]) return -1;
            return realIndex;
        }
        if (SdntArr[realIndex].LastName < key) L = M + 1;
        else R = M - 1;
    }
    return -1;
}

int RecBinarySearchAvg(int L, int R, double key) {
    if (L > R) return -1;
    int M = (L + R) / 2;
    int realIndex = IndexByAverage[M];
    double val = GetAverage(realIndex);

    double diff = val - key;
    if (diff < 0) diff = -diff; 

    if (diff < 0.001) {
        if (DeletedMask[realIndex]) return -1;
        return realIndex;
    }
    if (val > key) return RecBinarySearchAvg(M + 1, R, key);
    else return RecBinarySearchAvg(L, M - 1, key);
}

void EditStudent(int index) {
    cout << "Redaktirovanie zapisi:" << endl;
    PrintStudent(index);
    cout << "Novaya Familiya: "; cin >> SdntArr[index].LastName;
    cout << "Novoe Imya: "; cin >> SdntArr[index].FirstName;
    cout << "Novaya Ocenka 1: "; cin >> SdntArr[index].Grade1;
    cout << "Novaya Ocenka 2: "; cin >> SdntArr[index].Grade2;
    cout << "Novaya Ocenka 3: "; cin >> SdntArr[index].Grade3;
    cout << "Obnovleno. Perestraivaem indexy." << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();
}

void MarkDeleted(int index) {
    DeletedMask[index] = true;
    cout << "Zapis pomechena kak udalennaya." << endl;
}

void PackArray() {
    int newCount = 0;
    for (int i = 0; i < SdntCount; i++) {
        if (DeletedMask[i] == false) {
            SdntArr[newCount] = SdntArr[i];
            DeletedMask[newCount] = false;
            newCount++;
        }
    }
    int removed = SdntCount - newCount;
    SdntCount = newCount;
    for (int i = SdntCount; i < MAX_STUDENTS; i++) DeletedMask[i] = false;

    cout << "Fizicheski udaleno " << removed << " zapisey." << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();
}

int main() {
    setlocale(LC_ALL, "Russian");

    for (int i = 0; i < MAX_STUDENTS; i++) DeletedMask[i] = false;

    cout << "=== 1. Vvod dannyh (Pukt 1) ===" << endl;
    int n;
    cout << "Skolko studentov vvesti? "; cin >> n;
    for (int k = 0; k < n; k++) {
        cout << "Student " << SdntCount + 1 << ":" << endl;
        InputStudent(SdntCount);
        SdntCount++;
    }

    cout << "\n=== 2. Rabota s failami (Pukty 3, 4) ===" << endl;
    SaveToFile();

    cout << "\n=== 3. Postroenie i vyvod indexov (Pukty 5, 6, 7) ===" << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();

    cout << "--- Po Familii (A-Z) ---" << endl;
    PrintByIndex(IndexByLastName);

    cout << "--- Po Sr. Ballu (High-Low) ---" << endl;
    PrintByIndex(IndexByAverage);

    cout << "\n=== 4. Poisk (Punkt 8) ===" << endl;
    string keyName;
    cout << "Vvedite Familiyu dlya poiska: "; cin >> keyName;
    int resName = IterBinarySearchLastName(keyName);
    if (resName != -1) PrintStudent(resName);
    else cout << "Ne naideno." << endl;

    double keyAvg;
    cout << "Vvedite Sr. Ball dlya poiska: "; cin >> keyAvg;
    int resAvg = RecBinarySearchAvg(0, SdntCount - 1, keyAvg);
    if (resAvg != -1) PrintStudent(resAvg);
    else cout << "Ne naideno." << endl;

    cout << "\n=== 5. Redaktirovanie (Punkt 9) ===" << endl;
    if (SdntCount > 0) EditStudent(0);

    cout << "\n=== 6. Udalenie (Punkt 10) ===" << endl;
    if (SdntCount > 0) {
        cout << "Udalyaem pervogo studenta..." << endl;
        MarkDeleted(0);
        PackArray();
    }

    cout << "\n--- Itogoviy spisok ---" << endl;
    PrintByIndex(IndexByLastName);

    cout << "\nRabota zavershena." << endl;
    return 0;
}
