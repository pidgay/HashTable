#include <iostream>
#include <cmath>
using namespace std;

string randomKey() {
    string key;
    for (int i = 0; i < 6; ++i) {
        key += 'A' + (rand() % 26);
    }
    return key;
}

int randomInt(){
    return 1 + (rand() % 1000);
}

template <class T>
class HashTable{
private:
    struct LinkedList{
        struct Node {
            string key = {};
            T value = {};
            Node* nextNode = {};
            Node* prevNode = {};

            Node(const string& key, T value){
                this->key = key;
                this->value = value;
                nextNode = nullptr;
                prevNode = nullptr;
            }

            void update( T v){
                value = v;
            }
        };

        Node* First = {};
        Node* Last = {};
        size_t size = {};

        LinkedList(){
            First = nullptr;
            Last = nullptr;
            size = 0;
        }

        bool checkIfEmpty(){
            return !First && !Last ? true : false;
        }

        void push(string key, T value){
            Node* newNode = new Node(key, value);
            if(checkIfEmpty()){
                First = newNode;
                Last = newNode;
            }
            else{
                Last->nextNode = newNode;
                newNode->prevNode = Last;
                Last = newNode;
            }
            size++;
        }

        void removeLast(){
            if(Last == First){
                delete Last;
                First = nullptr;
                Last = nullptr;
                size = 0;
                return;
            }
            Node newLast;
            Last->prevNode->nextNode = nullptr;
            newLast = Last->prevNode;
            delete Last;
            Last = newLast;
            size--;
        }

        void removeFirst(){
            if(First == Last) {
                delete First;
                First = nullptr;
                Last = nullptr;
                size = 0;
                return;
            }
           Node newFirst;
            First->nextNode->prevNode = nullptr;
            newFirst = First->nextNode;
            delete First;
            First = newFirst;
            size--;
        }

        Node* findNode(string key){
            Node* currentNode = First;
            while(currentNode){
                if(currentNode->key == key){
                    return currentNode;
                }
                currentNode = currentNode->nextNode;
            }
            return nullptr;
        }

        bool findAndRemoveNode(string key){
            if (size == 0){
                return false;
            }

            Node currentNode = findNode(key);

            if (currentNode) {
                if(currentNode == First && currentNode == Last){
                    delete currentNode;
                    size--;
                    return true;
                }
                else if(currentNode == First){
                    removeFirst();
                    return true;
                }
                else if(currentNode == Last) {
                    removeLast();
                    return false;
                }
                else{
                    currentNode->prevNode->nextNode = currentNode->nextNode;
                    currentNode->nextNode->prevNode = currentNode->prevNode;
                    delete currentNode;
                    size--;
                    return true;
                }
            }
            return false;
        }

        int printList(){
            Node* currentNode = First;
            int count = 0;
            while (currentNode){
                cout << currentNode->key << " -> " << currentNode->value << "; ";
                currentNode = currentNode->nextNode;
                count++;
            }
            return count;
        }

        void clearList(){
            Node* nodeToRemove = First;
            Node* nextNode;

            do{
                nextNode = nodeToRemove->nextNode;
                delete nodeToRemove;
                nodeToRemove = nextNode;
            }while(nodeToRemove->nextNode);

            First = nullptr;
            Last = nullptr;
            size = 0;
        }

    };
public:
    LinkedList* listPointer = {};
    int currentSize = {};
    int tableSize = {};
    float resizeMultiplier = {};
    float fillIndicator = {};

    HashTable(){
        listPointer = new LinkedList[10];
        currentSize = 0;
        tableSize = 10;
        resizeMultiplier = 1.5;
        fillIndicator = 0;
    }

    ~HashTable(){
        delete [] listPointer;
    }

    unsigned int generateHash(string key, int size){
        unsigned int hash = 0;
        for (int i = 0; i < key.size(); ++i) {
            hash += static_cast<unsigned>(key[i]) * static_cast<unsigned>(pow(31,key.size() - i + 1));
        }
        hash = hash % size;
        return hash;
    }

    void addNode(string key, T value){
        int index = generateHash(key, tableSize);
        typename LinkedList::Node *tempNode = find(key);

        if(tempNode){
            tempNode->update(value);
        }

        if(fillIndicator > 0.75){
            rehash();
        }

        listPointer[index].push(key,value);
        currentSize++;
        fillIndicator = static_cast<float>(currentSize) / static_cast<float>(tableSize);
    }

    void rehash(){
        int tableNewSize = tableSize * resizeMultiplier;
        LinkedList* tempPointer = new LinkedList[tableNewSize];

        for (int i = 0; i < tableSize; ++i) {
            typename LinkedList::Node *currentNode = listPointer[i].First;
            do{
                if (listPointer[i].First) {
                    string key = currentNode->key;
                    T value = currentNode->value;
                    int index = generateHash(key, tableNewSize);
                    tempPointer[index].push(key, value);
                    currentNode = currentNode->nextNode;
                }
            }while (currentNode);
        }
        delete listPointer;
        tableSize = tableNewSize;
        listPointer = tempPointer;
    }

    typename LinkedList::Node* find(string key){
        int index = generateHash(key, tableSize);
        typename LinkedList::Node* currentNode = listPointer[index].First;

        while(currentNode){
            if (currentNode->key == key){
                return currentNode;
            }
            currentNode = currentNode->nextNode;
        }
        return nullptr;
    }

    void printTableInfo(){
        int maxList = 0;
        int minList = 1;
        int nonNullLists = 0;
        float avgList = 0;
        int lengthList = 0;

        cout
            << "======================================" << endl
            << "Current Size: " << currentSize << endl
            << "Max Size: " << tableSize << endl
            << "======================================" << endl;

        for (int i = 0; i < tableSize; ++i) {
            if (listPointer[i].First){
                nonNullLists++;
                cout << i << ": ";
                lengthList = listPointer[i].printList();
                cout << endl;
            }
            maxList = lengthList > maxList ? lengthList : maxList;
            minList = lengthList < minList ? lengthList : minList;
            avgList += static_cast<float>(lengthList);
        }
        avgList = avgList / static_cast<float>(currentSize) - 1.0;
        cout
            << "======================================" << endl
            << "Non-Null Lists: " << nonNullLists << endl
            << "List Min Size: " << minList << endl
            << "List Max Size: " << maxList << endl
            << "List Avg Size: " << avgList << endl;
    }

    void clearTable(){
        while(currentSize != 0){
            listPointer[currentSize - 1].clearList();
            currentSize--;
        }
    }
};


int main() {
    srand(time(nullptr));
    class HashTable<int>* hashTable = new HashTable<int>;

    for (int i = 0; i < 1000; ++i) {
        hashTable->addNode(randomKey(), randomInt());
    }

    hashTable->printTableInfo();
    return 0;
}
