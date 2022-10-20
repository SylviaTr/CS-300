//Project Two - Sylvia Trynkin//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

// menu function to display options to the user
void DisplayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "4. Exit" << endl << endl;
    cout << "What do you want to do? ";
}

/*structure definition(course structure consists of course number, course nameand
prerequisites (as vector as some courses have more than one PreReq)*/
struct Course {
    string courseNum;
    string courseName;
    vector<string> preRequisite;
};

//structure for tree node with left and right child
struct Node {
    Course course;
    Node* left;
    Node* right;

    //default constructor       
    Node() {
        left = nullptr;
        right = nullptr;
    }
    //initialize a course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }

};


//define a tree class
class BinarySearchTree {

private:

    void ClearNode(Node* node);

public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(BinarySearchTree* tree, Node* node);
    void Search(string courseNum);
    void PrintCourse(Node* node);
};

//default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
};

//destructor
BinarySearchTree::~BinarySearchTree() {
    ClearNode(root);
};

//clears left and right pointer nodes and deletes them
void BinarySearchTree::ClearNode(Node* node) {
    if (node != nullptr) {
        ClearNode(node->left);
        node->left = nullptr;
        ClearNode(node->right);
        node->right = nullptr;
        delete node;
    }
};


//inserts course into a course list - builds the tree
void BinarySearchTree::Insert(BinarySearchTree* tree, Node* node) {
    //if the root is empty
    if (tree->root == nullptr) {
        //the current course becomes the root
        tree->root = node;
    }
    else {
        //current node is tree's root
        Node* curr = tree->root;
        //while the current node is not empty
        while (curr != nullptr) {
            //if node is less than current courseNumber
            if (node->course.courseNum < curr->course.courseNum) {
                //if current's left child is a nullpointer
                if (curr->left == nullptr) {
                    //assign left child with courseNumber
                    curr->left = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->left;
                }
            }
            else {
                //if current's right child is a nullpointer
                if (curr->right == nullptr) {
                    //assign right child with courseNumber
                    curr->right = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->right;
                }
            }

        }

    }
}
//search for a course and print course information
void BinarySearchTree::Search(string courseNum) {
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseNum == courseNum) {
            //prints course id, name
            cout << currentNode->course.courseNum << ", ";
            cout << currentNode->course.courseName;
            cout << endl;
            cout << "Prerequisites: ";
            //prints any prerequisites, followed by , if there are more than 1
            for (string preRequisite : currentNode->course.preRequisite) {
                if (preRequisite == currentNode->course.preRequisite.back()) {

                    cout << preRequisite << endl;
                }
                else {
                    cout << preRequisite << ", ";
                }
            }

            return;
        }
        //searches left pointer if not found
        else if (courseNum < currentNode->course.courseNum) {

            if (currentNode->left != nullptr) {
                currentNode = currentNode->left;
            }
        }
        //searches right pointer if not found
        else {

            currentNode = currentNode->right;
        }
    }
    //course is not in the list
    cout << "Course " << courseNum << "not found. " << endl;
    return;
}


//prints courselist
void BinarySearchTree::PrintCourse(Node* node) {

    //once an empty node is reached, print tree
    if (node == nullptr) {
        return;
    }

    //PrintCourse definition - starts at left node, prints course number and name, then contiunes with the right
    PrintCourse(node->left);
    cout << node->course.courseNum << ", ";
    cout << node->course.courseName << endl;
    PrintCourse(node->right);
};

//opens and loads a file and adds course info into tree
void loadCourse(string filename, BinarySearchTree* tree) {
    ifstream file(filename);
    //checks if file is open and gives feedback
    if (file.is_open()) {
        cout << "File is open." << endl;
        //initialize variables count, line and token
        int count;
        string line;
        string token;
        //parses through the line to get course number, name and PreReqs and store those in the tree
        while (getline(file, line)) {
            //initialize count with 0
            count = 0;
            Node* node = new Node();
            stringstream str(line);

            while (count < 2) {
                //parses through the line
                getline(str, token, ',');
                //if count is 0
                if (count == 0) {
                    //assign courseNum to token
                    node->course.courseNum = token;
                }
                //otherwise
                else {
                    //assign courseName to token
                    node->course.courseName = token;
                }
                //increment count by 1
                count++;
            }
            while (getline(str, token, ',')) {
                //if courseNumber exists as first value in another line: assign PreReq to token;
                node->course.preRequisite.push_back(token);
            }

            //inserts node into tree
            tree->Insert(tree, node);
        }
    }
    //file error if file not found
    else {
        cout << "File not found. " << endl;
        return;
    }

}
//main function
void main() {

    BinarySearchTree* tree = new BinarySearchTree();
    //initialize variables for the user's chosen file and the user's chosen course for loading a file
    string chosenFile;
    string chosenCourse;
    //initialize userInput with 1
    int userInput = 0;

    cout << "Welcome to the course information system." << endl;

    //loop while the user does not exit with 4
    while (userInput != 4) {
        //calls the DisplayMenu function and get user input
        DisplayMenu();
        cin >> userInput;

        switch (userInput) {
            //if user choses number 1 in the menu: loads chosen file
        case 1:
            cout << endl;
            cout << "What file do you want to load?";
            cin >> chosenFile;

            //load the chosen file
            loadCourse(chosenFile, tree);
            cout << endl;
            break;

            //if user choses number 2 in the menu: print courses in alphanumeric order by starting at the root
        case 2:
            cout << endl;
            cout << "Course Number / Course Name " << endl;
             
            //prints the tree in alphanumerical order starting at the root by calling the PrintCourse function
            tree->PrintCourse(tree->root);
            cout << endl;
            break;

            //if user choses number 3 in the menu: prints course that user chooses and any prerequisites
        case 3:
            cout << endl;
            cout << "Which course do you want to know about? ";
            cin >> chosenCourse;
            cout << endl;

            std::transform(chosenCourse.begin(), chosenCourse.end(), chosenCourse.begin(), ::toupper);
            tree->Search(chosenCourse);

            cout << endl;
            break;

            //if user choses number 4 in the menu: program exit
        case 4:
            cout << "Thank you and good bye!" << endl;
            break;

            //default message if user chooses other number than 1, 2, 3 or 4
        default:
            cout << "Error! Please choose a number between 1 and 4!" << endl;
            break;
        }
    }
}
