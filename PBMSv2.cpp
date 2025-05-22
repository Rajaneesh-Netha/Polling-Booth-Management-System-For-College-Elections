#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ---------- Structures ----------
struct Candidate {
    string name;
    string symbol;
    string slogan;
    int votes = 0;
};

struct Student {
    string name;
    string rollNo;
    int year;
    bool hasVoted = false;
};

struct Node {
    string studentID;
    string candidateName;
    Node* next;

    Node(string sID, string cName) {
        studentID = sID;
        candidateName = cName;
        next = nullptr;
    }
};

// ---------- Globals ----------
vector<Candidate> candidates;
vector<Student> students;

Node* voteHead = nullptr;
Node* voteTail = nullptr;

bool isVotingOpen = false;
bool isVotingEnded = false;

// ---------- Admin Credentials ----------
const string admin_username = "admin";
const string admin_password = "CBIT1979";

// ---------- Function Prototypes ----------
bool adminLogin();
void registerCandidate();
void registerStudent();
bool isStudentAlreadyRegistered(const string &rollNo);
Student* findStudentByRollNo(const string &rollNo);
void castVote();
void showCandidateList();
void showResults();
void declareWinner();
void showVoteHistory();
void startVoting();
void endVoting();

// ---------- Admin Login ----------
bool adminLogin() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;
    return username == admin_username && password == admin_password;
}

// ---------- Registration ----------
void registerCandidate() {
    Candidate c;
    cout << "Enter candidate name: ";
    cin.ignore();
    cin>>c.name;
    cout << "Enter candidate symbol: ";
    cin>>c.symbol;
    cout << "Enter candidate slogan: ";
    cin>>c.slogan;

    for (auto &cand : candidates) {
        if (cand.name == c.name) {
            cout << "Candidate already registered.\n";
            return;
        }
    }

    candidates.push_back(c);
    cout << "Candidate '" << c.name << "' registered successfully.\n";
}

bool isStudentAlreadyRegistered(const string &rollNo) {
    for (auto &s : students) {
        if (s.rollNo == rollNo)
            return true;
    }
    return false;
}

void registerStudent() {
    Student s;
    cout << "Enter student name: ";
    cin.ignore();
    cin>>s.name;
    cout << "Enter roll number: ";
    cin>>s.rollNo;
    cout << "Enter the year of study (1-4): ";
    cin >> s.year;

    if (s.year < 2 || s.year > 4) {
        cout << "Only 2nd year and above students are eligible.\n";
        return;
    }

    if (isStudentAlreadyRegistered(s.rollNo)) {
        cout << "Student already registered.\n";
        return;
    }

    s.hasVoted = false;
    students.push_back(s);
    cout << "Student '" << s.name << "' registered.\n";
}

// ---------- Find Student ----------
Student* findStudentByRollNo(const string &rollNo) {
    for (auto &s : students) {
        if (s.rollNo == rollNo)
            return &s;
    }
    return nullptr;
}

// ---------- Voting Process ----------
void castVote() {
    if (!isVotingOpen) {
        cout << "Voting is not open.\n";
        return;
    }

    string inputName, inputRoll;
    cout << "\n--- Voter Authentication ---\n";
    cin.ignore();
    cout << "Enter your name: ";
    cin>>inputName;
    cout << "Enter your roll number: ";
    cin>>inputRoll;

    Student* s = findStudentByRollNo(inputRoll);
    if (!s || s->name != inputName) {
        cout << "Error: Invalid credentials.\n";
        return;
    }

    if (s->hasVoted) {
        cout << "Error: You have already voted (Malpractice detected).\n";
        return;
    }

    cout << "Welcome " << s->name << "! Please cast your vote.\n";
    for (int i = 0; i < candidates.size(); ++i) {
        cout << i + 1 << ". " << candidates[i].name << " (" << candidates[i].symbol << ") - " << candidates[i].slogan << "\n";
    }

    int choice;
    cout << "Enter candidate number: ";
    cin >> choice;

    if (choice < 1 || choice > candidates.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    candidates[choice - 1].votes++;
    s->hasVoted = true;

    Node* newVote = new Node(inputRoll, candidates[choice - 1].name);
    if (voteHead == nullptr)
        voteHead = voteTail = newVote;
    else {
        voteTail->next = newVote;
        voteTail = newVote;
    }

    cout << "Your vote has been casted successfully.\n";

    // -------- Auto-declare winner if all students voted --------
    bool allVoted = true;
    for (const auto &stu : students) {
        if (!stu.hasVoted) {
            allVoted = false;
            break;
        }
    }

    if (allVoted && isVotingOpen) {
        cout << "All students have voted. Voting will now end automatically.\n";
        endVoting();
    }
}

// ---------- Results ----------
void showCandidateList() {
    if (candidates.empty()) {
        cout << "No candidates registered.\n";
        return;
    }

    cout << "\n--- Registered Candidates ---\n";
    for (const Candidate &c : candidates) {
        cout << "Name   : " << c.name << "\n";
        cout << "Symbol : " << c.symbol << "\n";
        cout << "Slogan : " << c.slogan << "\n";
        cout << "---------------------------\n";
    }
}

void showResults() {
    cout << "\n--- Voting Results ---\n";
    for (const Candidate &c : candidates)
        cout << c.name << ": " << c.votes << " votes\n";
}

void declareWinner() {
    int maxVotes = -1;
    string winner = "None";

    for (auto &c : candidates) {
        if (c.votes > maxVotes) {
            maxVotes = c.votes;
            winner = c.name;
        }
    }

    cout << "\n*** Winner: " << winner << " with " << maxVotes << " votes. ***\n";
}

void showVoteHistory() {
    if (voteHead == nullptr) {
        cout << "No votes have been cast yet.\n";
        return;
    }

    cout << "\n--- Vote History (Student -> Candidate) ---\n";
    Node* temp = voteHead;
    while (temp != nullptr) {
        cout << temp->studentID << " voted for " << temp->candidateName << "\n";
        temp = temp->next;
    }
}

// ---------- Voting Control ----------
void startVoting() {
    if (candidates.empty() || students.empty()) {
        cout << "Cannot start voting. Ensure both candidates and students are registered.\n";
        return;
    }
    isVotingOpen = true;
    cout << "Voting has started!\n";
}

void endVoting() {
    if (!isVotingOpen) {
        cout << "Voting is not active.\n";
        return;
    }
    isVotingOpen = false;
    isVotingEnded = true;
    declareWinner();
    cout << "Voting has ended.\n";
}

// ---------- Admin Menu ----------
void adminMenu() {
    int choice;
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Register Candidate\n";
        cout << "2. Register Student\n";
        cout << "3. Start Voting\n";
        cout << "4. Show Candidate List\n";
        cout << "5. Show Vote History\n";
        cout << "6. Declare Winner\n";
        cout << "7. End Voting\n";
        cout << "8. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!isVotingOpen) registerCandidate();
                else cout << "Cannot register candidates after voting has started.\n";
                break;
            case 2:
                if (!isVotingOpen) registerStudent();
                else cout << "Cannot register students after voting has started.\n";
                break;
            case 3:
                startVoting();
                break;
            case 4:
                showCandidateList();
                break;
            case 5:
                showVoteHistory();
                break;
            case 6:
                declareWinner();
                break;
            case 7:
                endVoting();
                break;
            case 8:
                return;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

// ---------- Main ----------
int main() {
    int choice;
    while (true) {
        cout << "\n--- Polling Booth Management System ---\n";
        cout << "1. Admin Login\n";
        cout << "2. Cast Vote (Student)\n";
        cout << "3. Show Results\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (adminLogin()) {
                    adminMenu();
                } else {
                    cout << "Invalid admin credentials.\n";
                }
                break;
            case 2:
                castVote();
                break;
            case 3:
                showResults();
                break;
            case 4:
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}
