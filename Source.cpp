#include <iostream>
#include <random>
#include <cmath>
#include <queue>

using namespace std;

int tab = 0;
int num_of_tab = 0;

struct Node_BST
{
    int data;
    Node_BST* left;
    Node_BST* right;
    Node_BST* parent;
};

Node_BST* parent_bst = NULL;
int i = 0;

void add(int elem, Node_BST*& branch)
{
    //i = 0;
    if (i == 0) {
        parent_bst = NULL;
    }
    if (!branch)
    {
        branch = new Node_BST;
        branch->data = elem;
        branch->left = 0;
        branch->right = 0;
        branch->parent = parent_bst;
        return;
    }
    else
    {
        i++;
        parent_bst = branch;
        if (branch->data > elem)
            add(elem, branch->left);
        else
            add(elem, branch->right);
    }
}



void print(Node_BST* branch)
{
    if (!branch)
        return;
    tab += 5;
    print(branch->right);

    for (int i = 0; i < tab; i++)
        cout << " ";
    cout << "->";
    cout << branch->data << endl;
    print(branch->left);
    tab -= 5;
    return;
}

void printParent(Node_BST* branch) {
    if (branch == NULL)
        return;
    if (branch->parent != NULL) {
        cout << branch->data << ':' << branch->parent->data << '\n';
    }
    else
        cout << branch->data << ':' << "NULL" << '\n';
    printParent(branch->left);
    printParent(branch->right);

}

int height(Node_BST* root) {
    if (!root)
        return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return max(hl, hr) + 1;
}

struct Node_RBT
{
    int data;
    Node_RBT* left;
    Node_RBT* right;
    Node_RBT* parent;
    char color;
    bool nill;
};

int height(Node_RBT* root) {
    if (!root)
        return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return max(hl, hr) + 1;
}

void print(Node_RBT* branch)
{
    if (!branch)
        return;
    tab += 5;
    if (branch->nill) {
        for (int i = 0; i < tab; i++)
            cout << " ";
        cout << "->";
        cout << "nill" << branch->color << endl;
        tab -= 5;
        return;
    }
    print(branch->right);

    for (int i = 0; i < tab; i++)
        cout << " ";
    cout << "->";
    cout << branch->data << branch->color << endl;
    print(branch->left);
    tab -= 5;
    return;
}

Node_RBT* search(Node_RBT* root, int key) {
    Node_RBT* tmp;
    if (root == NULL || root->data == key) {
        return root;
    }
    else
        if (key < root->data) {
            return search(root->left, key);
        }
        else {
            return search(root->right, key);
        }
}
void left_rotate(Node_RBT* node, Node_RBT*& root) {
    Node_RBT* x, *y;
    x = node;
    y = node->right;
    if (y->left != NULL) {
        x->right = y->left;
        y->left->parent = x;
    }
    if (x->parent == NULL) {
        root = y;
        y->parent = NULL;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
        y->parent = x->parent;
    }
    else {
        x->parent->right = y;
        y->parent = x->parent;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(Node_RBT* node, Node_RBT*& root) {
    Node_RBT* x, * y;
    y = node;
    x = node->left;
    if (x->right != NULL) {
        y->left = x->right;
        x->right->parent = y;
    }
    if (x->parent == NULL) {
        root = x;
        x->parent = NULL;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
        x->parent = y->parent;
    }
    else {
        y->parent->left = x;
        x->parent = y->parent;
    }
    x->right = y;
    y->parent = x;
}

Node_RBT* parent_rbt = NULL;

void add(int elem, Node_RBT*& branch)
{
    if (branch == NULL) {
        branch = new Node_RBT;
        branch->data = elem;
        branch->nill = false;
        branch->color = 'b';
        branch->left = new Node_RBT;
        branch->left->color = 'b';
        branch->left->nill = true;
        branch->left->parent = branch;
        branch->right = new Node_RBT;
        branch->right->color = 'b';
        branch->right->nill = true;
        branch->right->parent = branch;
        branch->parent = NULL;
        return;
    }
    if (branch->nill == true)
    {
        branch->data = elem;
        branch->nill = false;
        branch->color = 'r';
        branch->left = new Node_RBT;
        branch->left->color = 'b';
        branch->left->nill = true;
        branch->left->parent = branch;
        branch->right = new Node_RBT;
        branch->right->color = 'b';
        branch->right->nill = true;
        branch->right->parent = branch;
        return;
    }
    else
    {
        if (branch->data > elem)
            add(elem, branch->left);
        else
            add(elem, branch->right);
    }
}
void add_balance_RBT(Node_RBT*& root, Node_RBT* elem) {
    Node_RBT* NewNode = elem, *p, *Gp;
    p = NewNode->parent;
    if (p == NULL) {
        return;
    }
    Gp = p->parent;
    if (Gp == NULL)
        return;
    while (p->color == 'r') {
        if (p == Gp->left) {
            if (Gp->right->color == 'r') {
                p->color = Gp->right->color = 'b';
                if (Gp->parent != NULL) {
                    Gp->color = 'r';
                }
                NewNode = Gp;
                p = NewNode->parent;
                if (p == NULL) {
                    break;
                }
                Gp = p->parent;
            }
            else if (NewNode == p->right) {
                NewNode = p;
                left_rotate(p, root);
                p = NewNode->parent;
                if (p == NULL) {
                    break;
                }
                Gp = p->parent;
            }
            else {
                p->color = 'b';
                if (Gp->parent != NULL) {
                    Gp->color = 'r';
                }
                right_rotate(Gp, root);
                p = NewNode->parent;
                if (p == NULL) {
                    break;
                }
                Gp = p->parent;
            }
        }
        else {
            if (Gp->right->color == 'r') {
                p->color = Gp->right->color = 'b';
                if (Gp->parent != NULL) {
                    Gp->color = 'r';
                }
                NewNode == Gp;
                p = NewNode->parent;
                if (p == NULL) {
                    break;
                }
                Gp = p->parent;
            }
            else if (NewNode == p->left) {
                NewNode = p;
                right_rotate(p, root);
                p = NewNode->parent;
                if (p == NULL) {
                    break;
                }
                Gp = p->parent;
            }
            else {
                p->color = 'b';
                if (Gp->parent != NULL) {
                    Gp->color = 'r';
                }
                left_rotate(Gp, root);
                p = NewNode->parent;
                if (p == NULL) {
                    break;
                }
                Gp = p->parent;
            }
        }
    }
}
struct Node_AVLT
{
    int data;
    Node_AVLT* left;
    Node_AVLT* right;
    Node_AVLT* parent;
    char color;
};

struct Generator {
    int random;
};

Generator random_uniform() {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> point(1, 100);
    int random = point(mt);
    struct Generator Gen_elem;
    Gen_elem.random = random;
    return Gen_elem;
}

Generator random_exponential() {
    random_device rd;
    mt19937 mt(rd());
    exponential_distribution<> point (1);
    int random = point(mt);
    struct Generator Gen_elem;
    Gen_elem.random = random;
    return Gen_elem;
}

int main() {
   Node_RBT* root = NULL;
   Node_BST* root1 = NULL;
   setlocale(LC_ALL, "RUS");
   /*for (int i = 0; i < 15; i++) {
       Generator num = random_uniform();
       add(num.random, root);
   }*/
   int s = 0, num = 0;
   cout << "введите количество элементов: " << '\n';
   cin >> s;
   Node_RBT* NewNode = NULL;
   if (s != 0) {
       cout << "введите элементы: " << '\n';
       for (int i = 0; i < s; i++) {
           cin >> num;
           add(num, root1);
           add(num, root);
           NewNode = search(root, num);
           add_balance_RBT(root, NewNode);
       }
   }
   else
       cout << "дерево пустое" << '\n';
   //cout << "введите элемент" << '\n';
   //int key = 0;
   //cin >> key;
   print(root1);
   for (int i = 0; i < 5; i++)
       cout << endl;
   print(root);
   //Node_RBT* rotate = search(root, key);
   //right_rotate(rotate, root);
   //if (!root) {
   //int h = height(root);
   //cout << "высота: " << h << '\n';
   //}
   //cout << "список родителей: " << '\n';
   //printParent(root);
   //print(root);
}
