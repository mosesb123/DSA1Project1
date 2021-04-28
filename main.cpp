#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <type_traits>

using namespace std;

template <typename T>
//template classes provide for a SimpleList class to be created which carries the combined functionalities of Queues and Stacks.
class SimpleList
{
private:
    class Node
    {
    private:
        //this is a singly linked list.
        Node *pointerToNext;
        T nodeValue;

    public:
        //Node only has a constructor, getters and setters.
        Node(T val, Node *pointer);
        Node *getNodePointer();
        void setNodePointer(Node *nodePointer);
        T getNodeValue();
        void setNodeValue(T value);
    };
    //the list keeps track of start and end pointers to implement pushes and pops in constant time.
    Node *pStart;
    Node *pEnd;
    string listName;

protected:
    //these will be crucial for Stack and/or Queue
    void insertAtEnd(T value);
    void insertAtStart(T value);
    T removeFromStart();

public:
    SimpleList(string listName);
    //it is up to Stack and Queue to implement push and pop, since push functions differently. Pop is also left as a virtual.
    virtual void push(T value) = 0;
    virtual T pop() = 0;
    Node *getStartNode();
    void setStartNode(Node *nodePointer);
    Node *getEndNode();
    void setEndNode(Node *nodePointer);
    string getListName();
    bool isEmpty();
};

template <typename T>
class Stack : SimpleList<T>
//Stack extends SimpleList and takes advatage of insertAtStart.
{
public:
    Stack(string listName) : SimpleList<T>(listName) {}
    T pop() { return this->removeFromStart(); }
    void push(T value);
};
template <typename T>
void Stack<T>::push(T value)
{
    this->insertAtStart(value);
}

template <typename T>
class Queue : SimpleList<T>
//Queue extends SimpleList and takes advatage of insertAtEnd.
{
public:
    Queue(string listName) : SimpleList<T>(listName) {}
    T pop() { return this->removeFromStart(); }
    void push(T value) { this->insertAtEnd(value); }
};
//SimpleList functions: constructor, getters and setters are self explanatory.
template <typename T>
SimpleList<T>::SimpleList(string listName)
{
    this->listName = listName;
    pStart = nullptr;
    pEnd = nullptr;
}
template <typename T>
T SimpleList<T>::removeFromStart()
//removes a node from the start and returns the value (pop)
//the value is returned and the next node on the list is updated to be the start
{
    T val = pStart->getNodeValue();
    Node *pointer = pStart;
    if (pStart->getNodePointer() == nullptr)
        //if the list is now empty, pEnd is updated too
        pEnd = nullptr;
    pStart = pStart->getNodePointer();
    delete pointer;
    return val;
}
template <typename T>
typename SimpleList<T>::Node *SimpleList<T>::getStartNode()
{
    return pStart;
}
template <typename T>
void SimpleList<T>::setStartNode(Node *nodePointer)
{
    pStart = nodePointer;
}
template <typename T>
typename SimpleList<T>::Node *SimpleList<T>::getEndNode()
{
    return pEnd;
}
template <typename T>
void SimpleList<T>::setEndNode(Node *nodePointer)
{
    pEnd = nodePointer;
}
template <typename T>
string SimpleList<T>::getListName()
{
    return listName;
}
template <typename T>
bool SimpleList<T>::isEmpty()
{ // if both pStart and pEnd are empty, the list is empty.
    return (pStart == nullptr && pEnd == nullptr);
}
template <typename T>
void SimpleList<T>::insertAtEnd(T val)
//inserts a node at the end- functionally, pop for queues.
//creates a new node to be the End node and has the former End node point to it.
{
    Node *endNode = new Node(val, nullptr);
    if (!isEmpty())
        pEnd->setNodePointer(endNode);
    else
        pStart = endNode;
    //if it's empty this node is also pStart.
    pEnd = endNode;
}
template <typename T>
void SimpleList<T>::insertAtStart(T val)
//inserts a node at the start- functionally, pop for stacks.
//creates a new node to be the Start node and points to the former Start node.
{
    Node *startNode = new Node(val, pStart);
    if (isEmpty())
        pEnd = startNode;
    //if it's empty this node is also pEnd.
    pStart = startNode;
}

//Node Functions: basic constructor, getters, setters.
template <typename T>
SimpleList<T>::Node ::Node(T val, Node *pointer)
{
    nodeValue = val;
    pointerToNext = pointer;
}
template <typename T>
typename SimpleList<T>::Node *SimpleList<T>::Node::getNodePointer()
{
    return pointerToNext;
}
template <typename T>
T SimpleList<T>::Node::getNodeValue()
{
    return nodeValue;
}
template <typename T>
void SimpleList<T>::Node::setNodePointer(Node *pointer)
{
    pointerToNext = pointer;
}
template <typename T>
void SimpleList<T>::Node::setNodeValue(T val)
{
    nodeValue = val;
}

template <typename T>
bool nameExists(string name, list<SimpleList<T> *> *listOfLists)
{
    //only checks if a list exists by name. O(N).
    //this function and findlist could have been combined to improve speed.
    for (auto l : *listOfLists)
    {
        if (l->getListName() == name)
            return true;
    }
    return false;
}

template <typename T>
SimpleList<T> *findList(string name, list<SimpleList<T> *> *listOfLists)
//finds list from a name. O(N)
{
    for (auto l : *listOfLists)
    {
        if (l->getListName() == name)
            return l;
    }
    return NULL;
}

template <typename T>
string create(string name, string listType, list<SimpleList<T> *> *lst)
//first the master list is checked to see if a list of this type already exists.
//if so, an error is thrown. if not the list is created of specific format and types.
{
    if (nameExists<T>(name, lst))
        return "ERROR: This name already exists!\n";
    else
    {
        if (listType == "queue")
            lst->push_back((SimpleList<T> *)(new Queue<T>(name)));
        else if (listType == "stack")
            lst->push_back((SimpleList<T> *)(new Stack<T>(name)));
    }
    return "";
}
template <typename T>
string pop(string name, list<SimpleList<T> *> *lst)
//the specific list is found and a value is popped from it.
{
    if (nameExists<T>(name, lst))
    {
        auto currentList = findList(name, lst);
        if (currentList->isEmpty())
            return "ERROR: This list is empty!\n";
        else
        {
            if constexpr (is_same<T, int>::value)
                //values needed to be converted to string- if it was already a string, a runtime error would be thrown.
                return ("Value popped: " + to_string(currentList->pop()) + "\n");
            else if constexpr (is_same<T, double>::value)
            {
                //there was an issue with doubles where they would often have trailing zeros. The extra logic solves this.
                string doubleChech = to_string(currentList->pop());
                doubleChech = doubleChech.erase(doubleChech.find_last_not_of('0') + 1, string::npos);
                doubleChech = doubleChech.erase(doubleChech.find_last_not_of('.') + 1, string::npos);
                return ("Value popped: " + doubleChech + "\n");
            }
            else if constexpr (is_same<T, string>::value)
                return ("Value popped: " + currentList->pop() + "\n");
        }
    }
    else
        return "ERROR: This name does not exist!\n";
}

template <typename T>
string push(string name, T value, list<SimpleList<T> *> *lst)
//the specific list is found and a value is pushed to it.
{

    if (nameExists<T>(name, lst))
    {
        auto currentList = findList(name, lst);
        currentList->push(value);
        return "";
    }
    else
        return "ERROR: This name does not exist!\n";
}

string inputLineHandler(string line, list<SimpleList<int> *> *listSLi, list<SimpleList<double> *> *listSLd, list<SimpleList<string> *> *listSLs)
//input can be one of three commands
//each command relates to a specific list Type
//and the commands will have one or two values afterwards
{
    string command = line.substr(0, line.find(" "));                                        //first word
    string partTwo = line.substr(1 + line.find(" "));                                       //remainder of action
    string name = partTwo.find(" ") == -1 ? partTwo : partTwo.substr(0, partTwo.find(" ")); //second word
    char listValues = name[0];                                                              //first character of the name which determines type

    string returns = "PROCESSING COMMAND: " + line + "\n";
    if (command == "create")
    {

        string listStructure = partTwo.substr(1 + partTwo.find(" ")); //third word - could have been streamlined for create and push
        //without an overly complicated Template structure, these if checks need to be repeated three times.
        //after the check the values and valueType are passed into template functions.
        if (listValues == 'i')
            returns += create<int>(name, listStructure, listSLi);
        else if (listValues == 'd')
            returns += create<double>(name, listStructure, listSLd);
        else if (listValues == 's')
            returns += create<string>(name, listStructure, listSLs);
    }
    else if (command == "pop")
    {
        if (listValues == 'i')
            returns += pop<int>(name, listSLi);
        else if (listValues == 'd')
            returns += pop<double>(name, listSLd);
        else if (listValues == 's')
            returns += pop<string>(name, listSLs);
    }
    else if (command == "push")
    {
        string value = partTwo.substr(1 + partTwo.find(" ")); //third word - could have been streamlined for create and push
        if (listValues == 'i')
            returns += push<int>(name, stoi(value.c_str()), listSLi); //only integers can be stored, so the value needs to be converted.
        else if (listValues == 'd')
            returns += push<double>(name, stod(value.c_str()), listSLd); //only doubles can be stored, so the value needs to be converted.
        else if (listValues == 's')
            returns += push<string>(name, value, listSLs);
    }
    return returns; // stream could have been accessed directly instead of returning, this was avoided.
}

int main()
{
    string inputFile = "commands1.txt"; //default input name;
    string outputFile = "output.txt";   //default output name;
    cout << "Enter name of input file: ";
    cin >> inputFile;
    cout << "Enter name of output file: ";
    cin >> outputFile;
    ifstream input(inputFile);
    ofstream output(outputFile);

    list<SimpleList<int> *> listSLi;    // all integer stacks and queues
    list<SimpleList<double> *> listSLd; // all double stacks and queues
    list<SimpleList<string> *> listSLs; // all string stacks and queues

    string line; //the file is read line by line
    while (getline(input, line))
    {
        //each line is fed into the inputLineHandler along with the stored lists of stacks and queues.
        //The list values could have been global, eliminating the need to pass them through, but that was avoided.
        output << inputLineHandler(line, &listSLi, &listSLd, &listSLs);
    }

    input.close();
    output.close();

    return 0;
}
