// The worst case of searching for a sorted list is O(n)
// when we can only linearly traverse it, for a Balanced BST we skip
// almost half of the nodes after one comparison with root

// The idea of SkipList:
// Creating multiple layers with "reference values" so that we can skip some nodes.

// For all (insertion, searching, deletion)
// Average Time Complexity is O(logn)
// Worst Time Complexity is O(n)
using namespace std;

#include <memory.h>
#include <ctime>
#include "iostream"

class Node {
public:
    int key;
    Node **forward;
    Node(int, int);
};

Node::Node(int key, int level) {
    this->key = key;
    forward = new Node *[level + 1];
    memset(forward, 0, sizeof(Node *) * (level + 1)); // filling forward array with NULLs
}

class SkipList {
    int maxLVL; // max level for this skipList
    float p; // probability
    int level;
    Node *header;

public:
    SkipList(int, float);
    int getRandomLevel();
    Node *createNode(int, int);
    void insertElement(int);
    int findElement(int);
    int deleteElement(int);
    void displayList();
};

SkipList::SkipList(int maxLVL, float p) {
    this->maxLVL = maxLVL;
    this->p = p;
    level = 0;
    header = new Node(-1, maxLVL);
}

int SkipList::getRandomLevel() {
    // Randomization for saving probability of pasting on top layer
    float r = (float) rand() / RAND_MAX;
    int lvl = 0;
    while (r < p && lvl < maxLVL) {
        lvl++;
        r = (float) rand() / RAND_MAX;
    }
    return lvl;
}

Node *SkipList::createNode(int key, int level) {
    Node *n = new Node(key, level);
    return n;
}

void SkipList::insertElement(int key) {
// Algorithm, two cases:
// 1) Key of next node is less than inserted key: we keep on moving forward at the same level
// 2) Key of next node is greater than inserted key: we store the pointer to current node i
// at update[i] (function for moving level from i to i-1) and move one level down and continue our search
// So at the 0 level we will definitely find a position to inserted element;
    Node *current = header;
    Node *update[maxLVL + 1];
    memset(update, 0, sizeof(Node*) * (maxLVL + 1));
    /*  start from highest level of skip list
        move the current pointer forward while key
        is greater than key of node next to current
        Otherwise inserted current in update and
        move one level down and continue search
    */
    for (int i = level; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->key < key)
            current = current->forward[i];
        update[i]=current;
    }
    /* reached level 0 and forward pointer to
       right, which is desired position to
       insert key.
    */
    current = current->forward[0];

    /* if current is NULL that means we have reached
       to end of the level or current's key is not equal
       to key to insert that means we have to insert
       node between update[0] and current node */
    if (current == nullptr || current->key != key) {
        int rlevel = getRandomLevel();

        // If random level is greater than list's current
        // level (node with highest level inserted in
        // list so far), initialize update value with pointer
        // to header for further use
        if (rlevel > level) {
            for (int i = level + 1; i < rlevel + 1; i++)
                update[i] = header;
            // Update the list current level
            level = rlevel;
        }

        // Here update[i] holds the pointer to node
        // at level i from which we moved down to
        // level i-1 and pointer of node left to insertion position at level 0
        Node *n = createNode(key, rlevel);

        // insert node by rearranging pointers
        for (int i=0;i<=rlevel;i++)
        {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
        printf("\nKey %d inserted\n", key);
    }
}

void SkipList::displayList() {
    for (int i=0;i<=level;i++)
    {
        Node *node = header->forward[i];
        cout << "Level " << i << ": ";
        while (node != NULL)
        {
            cout << node->key<<" ";
            node = node->forward[i];
        }
        cout << "\n";
    }
}

int SkipList::findElement(int key) {
    Node *current = header;

    for(int i = level; i >= 0; i--)
    {
        while(current->forward[i] &&
              current->forward[i]->key < key)
            current = current->forward[i];
    }

    current = current->forward[0];
    // If current node have key equal to
    // search key, we have found our target node
    if(current and current->key == key)
        cout<<"Found key: "<<key<<"\n";
    return key;
}

int SkipList::deleteElement(int key) {
    Node *current=header;
    Node *update[maxLVL+1];
    memset(update, 0, sizeof(Node*)*(maxLVL+1));
    for(int i = level; i >= 0; i--)
    {
        while(current->forward[i] != NULL  &&
              current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];

    // If current node is target node
    if(current != NULL and current->key == key) {
        /* start from lowest level and rearrange
           pointers just like we do in singly linked list
           to remove target node */
        for (int i = 0; i <= level; i++) {
            /* If at level i, next node is not target
               node, break the loop, no need to move
              further level */
            if (update[i]->forward[i] != current)
                break;

            update[i]->forward[i] = current->forward[i];
        }
        // Remove levels having no elements
        while(level>0 && header->forward[level]==0)
            level--;
    }
    return 0;
}

int main() {
    srand((unsigned)time(0));
    SkipList skipList (3, .5);
    skipList.insertElement(3);
    skipList.insertElement(6);
    skipList.insertElement(7);
    skipList.insertElement(9);
    skipList.insertElement(12);
    skipList.insertElement(19);
    skipList.insertElement(17);
    skipList.insertElement(26);
    skipList.insertElement(21);
    skipList.insertElement(25);
    skipList.findElement(25);
    skipList.deleteElement(12);
    skipList.displayList();
}