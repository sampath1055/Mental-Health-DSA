#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ----------- Signup & Login ------------
bool signup(string username, string password) {
    ofstream file("users.txt", ios::app);
    if (!file) return false;
    file << username << " " << password << endl;
    file.close();
    return true;
}

bool login(string username, string password) {
    ifstream file("users.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == username && p == password) return true;
    }
    return false;
}

// ---------- Queue for Mood Entries ----------
struct Mood {
    int level;
    string note;
    Mood* next;
};

class MoodQueue {
    Mood* front;
    Mood* rear;
public:
    MoodQueue() { front = rear = NULL; }
    void enqueue(int level, string note) {
        Mood* temp = new Mood{level, note, NULL};
        if (rear == NULL) {
            front = rear = temp;
            return;
        }
        rear->next = temp;
        rear = temp;
    }
    void display() {
        Mood* temp = front;
        cout << "\nMood Entries:\n";
        while (temp != NULL) {
            cout << "Level: " << temp->level << " - Note: " << temp->note << endl;
            temp = temp->next;
        }
    }
    Mood* getFront() { return front; }
};

// ---------- Stack for Undo ----------
struct MoodStack {
    int level;
    string note;
    MoodStack* next;
};

class MoodUndo {
    MoodStack* top;
public:
    MoodUndo() { top = NULL; }
    void push(int level, string note) {
        MoodStack* temp = new MoodStack{level, note, top};
        top = temp;
    }
    void pop() {
        if (!top) {
            cout << "Nothing to undo!\n";
            return;
        }
        cout << "Undoing: " << top->level << " - " << top->note << endl;
        MoodStack* temp = top;
        top = top->next;
        delete temp;
    }
};

// ---------- Linked List for Session History ----------
struct Session {
    string entry;
    Session* next;
};

class HistoryList {
    Session* head;
public:
    HistoryList() { head = NULL; }
    void addSession(string text) {
        Session* temp = new Session{text, NULL};
        if (!head) head = temp;
        else {
            Session* ptr = head;
            while (ptr->next) ptr = ptr->next;
            ptr->next = temp;
        }
    }
    void displayHistory() {
        cout << "\nSession History:\n";
        Session* temp = head;
        while (temp) {
            cout << temp->entry << endl;
            temp = temp->next;
        }
    }
};

// ---------- BST for Mood Analysis ----------
struct Node {
    int score;
    Node* left;
    Node* right;
};

class MoodTree {
    Node* root;
public:
    MoodTree() { root = NULL; }
    Node* insert(Node* node, int score) {
        if (!node) return new Node{score, NULL, NULL};
        if (score < node->score) node->left = insert(node->left, score);
        else node->right = insert(node->right, score);
        return node;
    }
    void insertScore(int score) {
        root = insert(root, score);
    }
    void inOrder(Node* node) {
        if (node) {
            inOrder(node->left);
            cout << node->score << " ";
            inOrder(node->right);
        }
    }
    void display() {
        cout << "\nMood Score BST: ";
        inOrder(root);
        cout << endl;
    }
    Node* getRoot() { return root; }
};

// ---------- MAIN PROGRAM ----------
int main() {
    string username, password;
    int choice;

    cout << "----- Mental Health Simulation -----\n";
    cout << "1. Signup\n2. Login\nChoice: ";
    cin >> choice;

    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    bool loggedIn = false;
    if (choice == 1) {
        if (signup(username, password)) {
            cout << "Signup Successful. Please login.\n";
        } else {
            cout << "Signup Failed.\n";
            return 0;
        }
        cout << "Login Now...\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        loggedIn = login(username, password);
    } else if (choice == 2) {
        loggedIn = login(username, password);
    }

    if (!loggedIn) {
        cout << "Login Failed.\n";
        return 0;
    }

    MoodQueue moodQueue;
    MoodUndo undoStack;
    HistoryList sessionHistory;
    MoodTree moodBST;

    while (true) {
        cout << "\n----- Menu -----\n";
        cout << "1. Enter Mood\n";
        cout << "2. Undo Last Entry\n";
        cout << "3. View Mood History\n";
        cout << "4. Analyze Mood (BST)\n";
        cout << "5. View Session History\n";
        cout << "6. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            int level;
            string note;
            cout << "Enter Mood Level (1-10): ";
            cin >> level;
            cin.ignore();
            cout << "Enter Note: ";
            getline(cin, note);
            moodQueue.enqueue(level, note);
            undoStack.push(level, note);
            moodBST.insertScore(level);
            sessionHistory.addSession("Mood: " + to_string(level) + ", Note: " + note);
        } else if (choice == 2) {
            undoStack.pop();
        } else if (choice == 3) {
            moodQueue.display();
        } else if (choice == 4) {
            moodBST.display();
        } else if (choice == 5) {
            sessionHistory.displayHistory();
        } else if (choice == 6) {
            cout << "Goodbye, take care!\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
