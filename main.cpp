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
    cout << "Student number: "; cin >> SdntArr[i].StudentNumber;
    cout << "Last name: "; cin >> SdntArr[i].LastName;
    cout << "First name: "; cin >> SdntArr[i].FirstName;
    cout << "Middle name: "; cin >> SdntArr[i].MiddleName;
    cout << "Birth year: "; cin >> SdntArr[i].BirthYear;
    cout << "Grade 1: "; cin >> SdntArr[i].Grade1;
    cout << "Grade 2: "; cin >> SdntArr[i].Grade2;
    cout << "Grade 3: "; cin >> SdntArr[i].Grade3;
}

void PrintStudent(int i) {
    cout << "ID " << SdntArr[i].StudentNumber << " | "
         << SdntArr[i].LastName << " " << SdntArr[i].FirstName << " " << SdntArr[i].MiddleName
         << " |   Birth: " << SdntArr[i].BirthYear
         << " |Grades: " << SdntArr[i].Grade1 << ", " << SdntArr[i].Grade2 << ", " << SdntArr[i].Grade3
         << " | Avg: " << GetAverage(i) << endl;
}

void SaveToFile() {
    string filename = "ENTER PATH";
    int mode;
    cout << "Save mode (1 - overwrite, 2 - append): ";
    cin >> mode;

    ofstream f;
    if (mode == 2) f.open(filename, ios::app);
    else f.open(filename);

    if (f.is_open()) {
        for (int i = 0; i < SdntCount; i++) {
            if (!DeletedMask[i]) {
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
        cout << "Saved to file" << endl;
    } else {
        cout << "File open error" << endl;
    }
}

void LoadFromFile() {
    string filename = "ENTER PATH";
    ifstream f(filename);

    if (f.is_open()) {
        cout << "Reading from file..." << endl;

        while (!f.eof() && SdntCount < MAX_STUDENTS) {
            string val;

            if (f.peek() == EOF) break;
            getline(f, val, ';');
            if (val == "") break;
            SdntArr[SdntCount].StudentNumber = stoi(val);

            getline(f, SdntArr[SdntCount].LastName, ';');
            getline(f, SdntArr[SdntCount].FirstName, ';');
            getline(f, SdntArr[SdntCount].MiddleName, ';');

            getline(f, val, ';'); SdntArr[SdntCount].BirthYear = stoi(val);
            getline(f, val, ';'); SdntArr[SdntCount].Grade1 = stoi(val);
            getline(f, val, ';'); SdntArr[SdntCount].Grade2 = stoi(val);
            getline(f, val);      SdntArr[SdntCount].Grade3 = stoi(val);

            DeletedMask[SdntCount] = false;
            SdntCount++;
        }
        f.close();
        cout << "Loaded students: " << SdntCount << endl;
    } else {
        cout << "File not found" << endl;
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

void BuildIndexByAverage() {
    for (int i = 0; i < SdntCount; i++) IndexByAverage[i] = i;

    for (int i = 0; i < SdntCount - 1; i++) {
        for (int j = 0; j < SdntCount - i - 1; j++) {
            if (GetAverage(IndexByAverage[j]) < GetAverage(IndexByAverage[j + 1])) {
                int tmp = IndexByAverage[j];
                IndexByAverage[j] = IndexByAverage[j + 1];
                IndexByAverage[j + 1] = tmp;
            }
        }
    }
}

void PrintByIndex(int IndexArray[]) {
    for (int i = 0; i < SdntCount; i++) {
        int realIndex = IndexArray[i];
        if (!DeletedMask[realIndex]) {
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

    if (fabs(val - key) < 0.001) {
        if (DeletedMask[realIndex]) return -1;
        return realIndex;
    }
    if (val > key) return RecBinarySearchAvg(M + 1, R, key);
    else return RecBinarySearchAvg(L, M - 1, key);
}

void EditStudent(int index) {
    cout << "Editing record..." << endl;
    PrintStudent(index);
    cout << "New last name: "; cin >> SdntArr[index].LastName;
    cout << "New first name: "; cin >> SdntArr[index].FirstName;
    cout << "New grade 1: "; cin >> SdntArr[index].Grade1;
    cout << "New grade 2: "; cin >> SdntArr[index].Grade2;
    cout << "New grade 3: "; cin >> SdntArr[index].Grade3;
    cout << "Updated. Rebuilding indexes..." << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();
}

void MarkDeleted(int index) {
    DeletedMask[index] = true;
    cout << "Record marked as deleted" << endl;
}

void PackArray() {
    int newCount = 0;
    for (int i = 0; i < SdntCount; i++) {
        if (!DeletedMask[i]) {
            SdntArr[newCount] = SdntArr[i];
            DeletedMask[newCount] = false;
            newCount++;
        }
    }
    int removed = SdntCount - newCount;
    SdntCount = newCount;
    for (int i = SdntCount; i < MAX_STUDENTS; i++) DeletedMask[i] = false;

    cout << "Physically removed: " << removed << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();
}

int main() {
    setlocale(LC_ALL, "Russian");

    for (int i = 0; i < MAX_STUDENTS; i++) DeletedMask[i] = false;

    cout << "=== Input ===" << endl;
    int n;
    cout << "How many students to enter: "; cin >> n;
    for (int k = 0; k < n; k++) {
        cout << "Student " << SdntCount + 1 << ":" << endl;
        InputStudent(SdntCount);
        SdntCount++;
    }

    cout << "\n=== File saving ===" << endl;
    SaveToFile();

    cout << "\n=== Build and print indexes ===" << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();

    cout << "--- Sorted by last name (A to Z) ---" << endl;
    PrintByIndex(IndexByLastName);

    cout << "--- Sorted by average score (High to Low) ---" << endl;
    PrintByIndex(IndexByAverage);

    cout << "\n=== Search ===" << endl;
    string keyName;
    cout << "Surname for search: ";
    cin >> keyName;
    int resName = IterBinarySearchLastName(keyName);
    if (resName != -1) PrintStudent(resName);
    else cout << "Not found" << endl;

    double keyAvg;
    cout << "Average score for search: ";
    cin >> keyAvg;
    int resAvg = RecBinarySearchAvg(0, SdntCount - 1, keyAvg);
    if (resAvg != -1) PrintStudent(resAvg);
    else cout << "Not found" << endl;

    cout << "\n=== Edit student ===" << endl;
    if (SdntCount > 0) EditStudent(0);

    cout << "\n=== Deletion ===" << endl;
    if (SdntCount > 0) {
        cout << "Deleting first student..." << endl;
        MarkDeleted(0);
        PackArray();
    }

    cout << "\n=== Final list ===" << endl;
    PrintByIndex(IndexByLastName);

    cout << "\nProgram completed" << endl;
    return 0;
}
