#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath> // для fabs
#include <locale>

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
         << " | Grades: " << SdntArr[i].Grade1 << ", " << SdntArr[i].Grade2 << ", " << SdntArr[i].Grade3
         << " | Avg: " << GetAverage(i) << endl;
}

void SaveToFile() {
    string filename;
    cout << "Enter file path to save: ";
    cin >> filename;
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
                  << SdntArr[i].Grade3 << "\n";
            }
        }
        f.close();
        cout << "Saved to file" << endl;
    } else {
        cout << "File open error" << endl;
    }
}

void LoadFromFile() {
    string filename;
    cout << "Enter file path to load: ";
    cin >> filename;
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

void PrintByIndex(int IndexArray[], bool reverse = false) {
    if (!reverse) {
        for (int i = 0; i < SdntCount; i++) {
            int realIndex = IndexArray[i];
            if (!DeletedMask[realIndex]) PrintStudent(realIndex);
        }
    } else {
        for (int i = SdntCount - 1; i >= 0; i--) {
            int realIndex = IndexArray[i];
            if (!DeletedMask[realIndex]) PrintStudent(realIndex);
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
    if (val > key) {
        return RecBinarySearchAvg(M + 1, R, key);
    } else {
        return RecBinarySearchAvg(L, M - 1, key);
    }
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
            SdntArr[newCount].StudentNumber = SdntArr[i].StudentNumber;
            SdntArr[newCount].LastName = SdntArr[i].LastName;
            SdntArr[newCount].FirstName = SdntArr[i].FirstName;
            SdntArr[newCount].MiddleName = SdntArr[i].MiddleName;
            SdntArr[newCount].BirthYear = SdntArr[i].BirthYear;
            SdntArr[newCount].Grade1 = SdntArr[i].Grade1;
            SdntArr[newCount].Grade2 = SdntArr[i].Grade2;
            SdntArr[newCount].Grade3 = SdntArr[i].Grade3;

            DeletedMask[newCount] = false;
            newCount++;
        }
    }
    int removed = SdntCount - newCount;
    SdntCount = newCount;

    BuildIndexByLastName();
    BuildIndexByAverage();

    cout << "Physically removed: " << removed << endl;
}


struct BSTNode {
    string key;                 // фамилия
    vector<int> indices;        // индексы записей SdntArr, имеющих эту фамилию
    BSTNode* left;
    BSTNode* right;
    BSTNode(const string& k, int arrIndex) : key(k), left(nullptr), right(nullptr) {
        indices.push_back(arrIndex);
    }
};

BSTNode* root = nullptr;

BSTNode* InsertBST(BSTNode* node, const string& key, int arrIndex) {
    if (node == nullptr) {
        return new BSTNode(key, arrIndex);
    }
    if (key == node->key) {
        node->indices.push_back(arrIndex);
    } else if (key < node->key) {
        node->left = InsertBST(node->left, key, arrIndex);
    } else {
        node->right = InsertBST(node->right, key, arrIndex);
    }
    return node;
}

void BuildBSTByLastName() {
    function<void(BSTNode*)> freeNode = [&](BSTNode* n){
        if (!n) return;
        freeNode(n->left);
        freeNode(n->right);
        delete n;
    };
    freeNode(root);
    root = nullptr;

    for (int i = 0; i < SdntCount; i++) {
        if (!DeletedMask[i]) {
            root = InsertBST(root, SdntArr[i].LastName, i);
        }
    }
}

void InOrderPrint(BSTNode* node) {
    if (!node) return;
    InOrderPrint(node->left);
    for (int idx : node->indices)
        if (!DeletedMask[idx]) PrintStudent(idx);
    InOrderPrint(node->right);
}

void ReverseInOrderPrint(BSTNode* node) {
    if (!node) return;
    ReverseInOrderPrint(node->right);
    for (int idx : node->indices) {
        if (!DeletedMask[idx]) PrintStudent(idx);
    }
    ReverseInOrderPrint(node->left);
}


void RecSearchBST(BSTNode* node, const string& key, vector<int>& out) {
    if (!node) return;
    if (key == node->key) {
        for (int idx : node->indices) { 
            out.push_back(idx);
        }
        return;
    } else if (key < node->key) {
        RecSearchBST(node->left, key, out);
    } else {
        RecSearchBST(node->right, key, out);
    }
}

vector<int> IterSearchBST(BSTNode* node, const string& key) {
    BSTNode* cur = node;
    while (cur != nullptr) {
        if (key == cur->key) {
            return cur->indices;
        } else if (key < cur->key) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return {};
}

void FreeBST(BSTNode* node) {
    if (!node) return;
    FreeBST(node->left);
    FreeBST(node->right);
    delete node;
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

    cout << "\n=== File saving (optional) ===" << endl;
    char saveChoice;
    cout << "Save to file? (y/n): ";
    cin >> saveChoice;
    if (saveChoice == 'y' || saveChoice == 'Y') SaveToFile();

    cout << "\n=== Build and print array-based indexes ===" << endl;
    BuildIndexByLastName();
    BuildIndexByAverage();

    cout << "--- Sorted by last name (A to Z) using array index ---" << endl;
    PrintByIndex(IndexByLastName);

    cout << "--- Sorted by average score (High to Low) using array index ---" << endl;
    PrintByIndex(IndexByAverage);

    cout << "\n=== Build BST index by last name ===" << endl;
    BuildBSTByLastName();

    cout << "\n--- BST: Sorted by last name (A to Z) ---" << endl;
    InOrderPrint(root);

    cout << "\n--- BST: Sorted by last name (Z to A) ---" << endl;
    ReverseInOrderPrint(root);

    cout << "\n=== Search in BST (iterative) ===" << endl;
    string keyName;
    cout << "Surname for search (iterative): ";
    cin >> keyName;
    vector<int> foundIter = IterSearchBST(root, keyName);
    if (!foundIter.empty()) {
        cout << "Found (iterative):" << endl;
        for (int idx : foundIter) {
            if (!DeletedMask[idx]) PrintStudent(idx);
        }
    } else {
        cout << "Not found (iterative)" << endl;
    }

    cout << "\n=== Search in BST (recursive) ===" << endl;
    cout << "Surname for search (recursive): ";
    cin >> keyName;
    vector<int> foundRec;
    RecSearchBST(root, keyName, foundRec);
    if (!foundRec.empty()) {
        cout << "Found (recursive):" << endl;
        for (int idx : foundRec) {
            if (!DeletedMask[idx]) PrintStudent(idx);
        }
    } else {
        cout << "Not found (recursive)" << endl;
    }

    
    cout << "\nEdit first student if exists (rebuild BST)" << endl;
    if (SdntCount > 0) {
        EditStudent(0);
        BuildBSTByLastName();
        cout << "BST after edit (A to Z)" << endl;
        InOrderPrint(root);
    }

    cout << "\n=== Deletion (mark) and pack ===" << endl;
    if (SdntCount > 0) {
        cout << "Marking first student as deleted and packing array..." << endl;
        MarkDeleted(0);
        PackArray();
        BuildBSTByLastName();
        cout << "--- BST after pack ---" << endl;
        InOrderPrint(root);
    }

    FreeBST(root);
    root = nullptr;

    cout << "\nProgram completed" << endl;
    return 0;
}
