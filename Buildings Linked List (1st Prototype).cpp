#include <iostream>
#include <fstream>
#include <string>

// Node structure for the linked list
struct Node {
    std::string buildingName;
    int number;
    Node* next;

    Node(std::string name, int num) : buildingName(name), number(num), next(NULL) {}
};

// Linked list class
class LinkedList {
public:
    Node* head;

    LinkedList() : head(NULL) {}

    void append(std::string name, int num) {
        Node* newNode = new Node(name, num);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    Node* search(std::string name) {
        Node* temp = head;
        while (temp) {
            if (temp->buildingName == name) {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;  // Not found
    }

    ~LinkedList() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() 
{
    LinkedList list;
    std::ifstream file("Buildings.txt");
    std::string buildingName;
    int count = 1;

    if (!file) {
        std::cerr << "Error: Could not open file Buildings.txt" << std::endl;
        return 1;
    }

    // Read building names from the file and store in the linked list
    while (std::getline(file, buildingName)) {
        list.append(buildingName, count++);
    }
    file.close();

    // Prompt the user to enter a building name to search
    std::string searchName;
    std::cout << "Enter the building name to search: ";
    std::getline(std::cin, searchName);

    // Search for the building in the list
    Node* result = list.search(searchName);
    if (result) 
	{
        std::cout << "Found: Building(" << result->number << "): " << result->buildingName << std::endl;
    }
}
