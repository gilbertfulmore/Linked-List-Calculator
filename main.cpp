/**********************************************************************
 *  Project: add or mult two linked lists together from file input    *
 *  Syntax:  $LinkedList <add or mult> <input1> <input2> <output>     *
 *  Author:  Gilbert Fulmore                                          *
 *  Course:  Computer Science 222                                     *
 *  Institution:  Okanagan College                                    *
 *  Version:  2014-11-24                                               *
 *  Platform: Unix (Solaris 11)					      *
 **********************************************************************/

#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

class LinkedList {
  struct Node {
    char data;
    Node* link;
  };

private:
  Node* current;
  int length;

public:
  LinkedList() {
    current = NULL;
    length = 0;
  }
  void insertLast(char);  //insert node at end of list
  void reverseList();     //reshape list to start read from reversed side
  void clearList();       //clears linkedList
  LinkedList operator+(const LinkedList&); //add linked lists together
  LinkedList operator*(const LinkedList&); //mult linked lists together
  void print();//print to terminal
  void fileToList(char*); //import file.txt to linked list
  void listToFile(char*);
};

void LinkedList::insertLast(char data) {
  Node* temp = new Node();  //create node
  temp->data = data;        //inject data
  temp->link = current;     //point previous
  current = temp;           //proceed pointer up linked list

  temp = NULL;              //free memory
  delete temp;
  ++length;
}

void LinkedList::reverseList() {
  Node* next = current->link;
  Node* last = NULL;
  bool endloop = false;

  while (!endloop) {
    if (next != NULL) {
      current->link = last;
      last = current;
      current = next;
      next = next->link;
    }
    else {
      current->link = last;
      endloop = true;
    }
  }

  next = NULL;
  last = NULL;
  delete next;
  delete last;
}

void LinkedList::clearList() {
  Node* temp = NULL;
  while (current != NULL) {
    temp = current->link;
    current = NULL;
    delete(current);
  }

  temp = NULL;
  delete(NULL);
}

LinkedList LinkedList::operator+(const LinkedList& b) {
  Node* left = current;
  Node* right = b.current;
  LinkedList list;

  char solution = 0;
  char remainder = 0;

  while (left != NULL && right != NULL) {  //add lists together 
    solution = left->data + right->data + remainder;
    if (solution > 9) {
      remainder = 1;
      solution -= 10;
    }
    else
      remainder = 0;

    list.insertLast(solution);
    left = left->link;
    right = right->link;
  }

  while (left != NULL) {  //if list left is longer than list right
    solution = left->data + remainder;
    if (solution > 9) {
      remainder = 1;
      solution -= 10;
    }
    else
      remainder = 0;

    list.insertLast(solution);
    left = left->link;
  }

  while (right != NULL) {  //if list right is longer than list left
    solution = right->data + remainder;
    if (solution > 9) {
      remainder = 1;
      solution -= 10;
    }
    else
      remainder = 0;

    list.insertLast(solution);
    right = right->link;
    solution = 0;
  }
  
  if (remainder == 1) {
    list.insertLast(1);
  }

  return list;
}

LinkedList LinkedList::operator*(const LinkedList& b) { 
  Node* top = NULL;
  Node* bottom = NULL;
  LinkedList tempA;
  LinkedList tempB;
  
  if (length > b.length) {  //if smaller number is on top code will require
    top = current;//additional processes
    bottom = b.current;
  } else {
    top = b.current;
    bottom = current;
  }
  Node* topHead = top;

  char remainder = 0;
  char solution = 0;
  //bottom first element
  while (top != NULL) {
    solution = (bottom->data * top->data) + remainder;
    if (solution > 9) {
      remainder = solution / 10;
      solution = solution % 10;
    }
    else
      remainder = 0;
    tempA.insertLast(solution);
    top = top->link;
  }
  
  if (remainder != 0)
    tempA.insertLast(remainder);

  //bottom second+ element
  int zerosToAdd = 1;
  bottom = bottom->link;

  while (bottom != NULL) {
    top = topHead;

    for (int i = 0; i < zerosToAdd; i++) {
      tempB.insertLast(0);
    }

    remainder = 0;
    while (top != NULL) {
      solution = (bottom->data * top->data) + remainder;
      if (solution > 9) {
	remainder = solution / 10;
	solution = solution % 10;
      } else
	remainder = 0;
      tempB.insertLast(solution);
      top = top->link;
    }
    if (remainder != 0)
      tempB.insertLast(remainder);

    tempA.reverseList();
    tempB.reverseList();
    tempA = tempA + tempB;

    bottom = bottom->link;
    ++zerosToAdd;

    tempB.clearList();
  }

  return tempA;
}

void LinkedList::print() {
  Node* temp = current;
  while (temp != NULL) {
    cout << (int)temp->data;
    temp = temp->link;
  }
  cout << endl;
  temp = NULL;
  delete temp;
}

void LinkedList::fileToList(char* filename) {
  ifstream data;
  data.open(filename);  //load file
  char output[1000000]; //export location
  if (data.is_open())   //if file is open
    while (!data.eof()) 
      data >> output;   //pass textfile deta into output array
  data.close();         //close file

  long i = 0;
  while (output[i] != NULL) {
    insertLast(output[i] - 48);
    i++;
  }
}

void LinkedList::listToFile(char* filename) {
  ofstream fileOut;
  fileOut.open(filename);

  Node* temp = current;
  while (temp != NULL) {
    fileOut << (int)temp->data;
    temp = temp->link;
  }
  fileOut.close();
  temp = NULL;
  delete temp;
}

int main(int argc, char* argv[]) {
  if (argc == 5) {
    LinkedList list1;
    LinkedList list2;
    LinkedList solution;

    list1.fileToList(argv[2]);
    list2.fileToList(argv[3]);

    if (strcmp(argv[1], "add") == 0) {
      solution = list1 + list2;

      list1.reverseList();
      list2.reverseList();

      list1.print(); 
      cout << "+" << endl;
      list2.print(); 
      cout << "=" << endl;
      solution.print();
      solution.listToFile(argv[4]);
    }
    else if (strcmp(argv[1], "mult") == 0) {
      solution = list1 * list2;

      list1.reverseList();
      list2.reverseList();

      list1.print();
      cout << "*" << endl;
      list2.print();
      cout << "=" << endl;
      solution.print();
      solution.listToFile(argv[4]);
    } else 
      cout << "Invalid: [options: add, mult]" << endl;
  } else
    cout << "invalid: [format: $LinkedList <add> <file1.txt> <file2.txt> <output.txt>]" << endl;

  return 0;
}
