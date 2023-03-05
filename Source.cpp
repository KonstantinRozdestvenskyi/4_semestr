#include <iostream>
#include <random>
#include <cmath>
#include <list>
#include <chrono>
#include <map>

using namespace std;

int tab = 0;
int num_of_tab = 0;

struct Node_BST
{
    int data;
    Node_BST* left;
    Node_BST* right;
    Node_BST* parent;
    Node_BST(int data, Node_BST* left, Node_BST* right, Node_BST* parent) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
};

void add_bst(int elem, Node_BST*& root){
    Node_BST* NewNode = new Node_BST(elem, NULL, NULL, NULL);
    Node_BST* y = nullptr;
    Node_BST* x = root;
    while (x != NULL) {
        y = x;
        if (NewNode->data < x->data) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    NewNode->parent = y;
    if (y == nullptr) {
        root = NewNode;
    }
    else if (NewNode->data < y->data) {
        y->left = NewNode;
    }
    else {
        y->right = NewNode;
    }
}



void print(Node_BST* root)
{
    if (!root)
        return;
    tab += 5;
    print(root->right);

    for (int i = 0; i < tab; i++)
        cout << " ";
    cout << "->";
    cout << root->data << endl;
    print(root->left);
    tab -= 5;
    return;
}

int height(Node_BST* root)
{
    // an empty tree has a height of 0
    if (root == nullptr) {
        return 0;
    }

    // create an empty queue and put the root node in the queue
    list<Node_BST*> queue;
    queue.push_back(root);

    Node_BST* front = nullptr;
    int height = 0;

    // loop until queue becomes empty
    while (!queue.empty())
    {
        // calculate the total number of nodes at the current level
        int size = queue.size();

        // we process each node of the current level and put them in the queue
        // non-empty left and right descendants
        while (size--)
        {
            front = queue.front();
            queue.pop_front();

            if (front->left) {
                queue.push_back(front->left);
            }

            if (front->right) {
                queue.push_back(front->right);
            }
        }

        // increase the height by 1 for each level
        height++;
    }

    return height;
}

struct Node_RBT
{
    int data;
    Node_RBT* left;
    Node_RBT* right;
    Node_RBT* parent;
    char color;
    Node_RBT(Node_RBT* left, Node_RBT* right, Node_RBT* parent, char color) {
        this->data = NULL;
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->color = color;
    }
    Node_RBT(int data, Node_RBT* left, Node_RBT* right, Node_RBT* parent, char color) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->color = color;
    }
};

Node_RBT* NILL = new Node_RBT(NULL, NULL, NULL, 'b');


int height(Node_RBT* root) {
    if (root == NILL)
        return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return max(hl, hr) + 1;
}

void print(Node_RBT* root)
{
    if (!root)
        return;
    tab += 5;
    if (root == NILL) {
        for (int i = 0; i < tab; i++)
            cout << " ";
        cout << "->";
        cout << "nill" << root->color << endl;
        tab -= 5;
        return;
    }
    print(root->right);

    for (int i = 0; i < tab; i++)
        cout << " ";
    cout << "->";
    cout << root->data << root->color << endl;
    print(root->left);
    tab -= 5;
    return;
}

bool is_search_second = false;

Node_RBT* search(Node_RBT* root, int key) {
    Node_RBT* tmp = NULL, * tmp1 = NULL;
    if (root == NILL) {
        return NULL;
    }
    else
        if (root->data == key) {
            tmp = root->right;
            is_search_second = false;
            while (tmp != NILL) {
                if (tmp->data == key) {
                    is_search_second = true;
                    tmp1 = tmp;
                }
                tmp = tmp->right;
            }
            if (is_search_second)
                return tmp1;
            else
                return root;
        }
        else if (key < root->data) {
            return search(root->left, key);
        }
        else {
            return search(root->right, key);
        }
}

void left_rotate(Node_RBT* node, Node_RBT*& root) {
    Node_RBT* x, * y;
    x = node;
    y = node->right;
    if (y->left != NULL) {
        x->right = y->left;
        if (y->left != NILL)
            y->left->parent = x;
    }
    //else
        //x->right = NULL;
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
        if (x->right != NILL)
            x->right->parent = y;
    }
    //else
        //y->left = NULL;
    if (y->parent == NULL) {
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

void add_balance_RBT(Node_RBT*& root, Node_RBT* elem, int& time) {
    time++;
    Node_RBT* NewNode = elem, * p, * Gp;
    p = NewNode->parent;
    if (p == NULL) {
        return;
    }
    Gp = p->parent;
    if (Gp == NULL)
        return;
    if (p->color == 'b') {
        return;
    }
    if (p == Gp->left) {
        if (Gp->right->color == 'r') {
            p->color = Gp->right->color = 'b';
            Gp->color = 'r';
            NewNode = Gp;
            p = NewNode->parent;
            if (p != NULL) {
                Gp = p->parent;
            }
        }
        else if (NewNode == p->right) {
            NewNode = p;
            left_rotate(p, root);
            p = NewNode->parent;
            if (p != NULL) {
                Gp = p->parent;
            }

        }
        else {
            p->color = 'b';
            Gp->color = 'r';
            right_rotate(Gp, root);
            p = NewNode->parent;
            if (p != NULL) {
                Gp = p->parent;
            }
        }
    }
    else {
        if (Gp->left->color == 'r') {
            p->color = Gp->left->color = 'b';
            Gp->color = 'r';
            NewNode = Gp;
            p = NewNode->parent;
            if (p != NULL) {
                Gp = p->parent;
            }
        }
        else if (NewNode == p->left) {
            NewNode = p;
            right_rotate(p, root);
            p = NewNode->parent;
            if (p != NULL) {
                Gp = p->parent;
            }
        }
        else {
            p->color = 'b';
            Gp->color = 'r';
            left_rotate(Gp, root);
            p = NewNode->parent;
            if (p != NULL) {
                Gp = p->parent;
            }
        }
    }
    if (p != NULL && NewNode->parent->color != 'b')
        add_balance_RBT(root, NewNode, time);
    root->color = 'b';

}

void add_rbt(int elem, Node_RBT*& root, int& time){
    Node_RBT* NewNode;
    NewNode = new Node_RBT(elem, NILL, NILL, NULL, 'r');
    Node_RBT* y = nullptr;
    Node_RBT* x = root;
    while (x != NILL) {
        y = x;
        if (NewNode->data < x->data) {
            x = x->left;
            time++;
        }
        else {
            x = x->right;
            time++;
        }
    }
    NewNode->parent = y;
    if (y == nullptr) {
        root = NewNode;
    }
    else if (NewNode->data < y->data) {
        y->left = NewNode;
    }
    else {
        y->right = NewNode;
    }
    if (y == NULL)
        NewNode->color = 'b';
    if (NewNode->parent != NULL && NewNode->parent->color != 'b') {
        add_balance_RBT(root, NewNode, time);
    }
    //clock_t end = clock();
    //time = ((double)end - (double)beg) / (double)CLOCKS_PER_SEC * 1000;
}
struct Node_AVLT
{
    int data;
    Node_AVLT* left;
    Node_AVLT* right;
    Node_AVLT* parent;
    int balancing_factor;
    Node_AVLT(int data, Node_AVLT* left, Node_AVLT* right, Node_AVLT* parent, int balancing_factor) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
        this->balancing_factor = balancing_factor;
    }
};

void print(Node_AVLT* root) {
    if (!root)
        return;
    tab += 5;
    print(root->right);

    for (int i = 0; i < tab; i++)
        cout << " ";
    cout << "->";
    cout << root->data << ' ' << root->balancing_factor << endl;
    print(root->left);
    tab -= 5;
    return;
}

int height(Node_AVLT* root) {
    if (!root)
        return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return max(hl, hr) + 1;
}
void left_rotate(Node_AVLT* node, Node_AVLT*& root) {
    Node_AVLT* x, * y;
    x = node;
    y = node->right;
    if (y->left != NULL) {
        x->right = y->left;
        y->left->parent = x;
    }
    else
        x->right = NULL;
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

void right_rotate(Node_AVLT* node, Node_AVLT*& root) {
    Node_AVLT* x, * y;
    y = node;
    x = node->left;
    if (x->right != NULL) {
        y->left = x->right;
        x->right->parent = y;
    }
    else
        y->left = NULL;
    if (y->parent == NULL) {
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

bool is_balance = true;

bool set_balansing_factors(Node_AVLT*& root) {
    int k = 0, n;
    if (root == NULL) {
        return is_balance;
    }
    n = root->data;
    k = root->balancing_factor = height(root->left) - height(root->right);
    if (abs(k) > 1) {
        is_balance = false;
    }
    is_balance = set_balansing_factors(root->left);
    is_balance = set_balansing_factors(root->right);
    return is_balance;
}

void add_balance_AVLT(Node_AVLT*& root, Node_AVLT*& elem, int& time) {
    auto beg = chrono::high_resolution_clock::now();
    bool is_balance = false;
    Node_AVLT* p = NULL;
    Node_AVLT* x = NULL;
    int k = 0, l = 0;
    while (!is_balance) {
        time++;
        k = elem->balancing_factor = height(elem->left) - height(elem->right);
        if (abs(elem->balancing_factor) <= 1) {
            elem = elem->parent;
            l++;
            if (elem != NULL) {
                continue;
            }
            else {
                break;
            }
        }
        p = elem->parent;
        if (elem->balancing_factor > 1) {
            k = elem->left->balancing_factor = height(elem->left->left) - height(elem->left->right);
            if (elem->left->balancing_factor >= 0) {
                right_rotate(elem, root);
            }
            else {
                left_rotate(elem->left, root);
                right_rotate(elem, root);
            }
        }
        if (elem->balancing_factor < -1) {
            k = elem->right->balancing_factor = height(elem->right->left) - height(elem->right->right);
            if (elem->right->balancing_factor <= 0) {
                left_rotate(elem, root);
            }
            else {
                right_rotate(elem->right, root);
                left_rotate(elem, root);
            }
        }
        //is_balance = set_balansing_factors(root, elem);
        if (p == NULL)
            break;
        elem = elem->parent;
        k = height(elem->left) - height(elem->right);
        if (abs(k) <= 1) {
            break;
        }
        //is_balance = set_balansing_factors(root, elem);
    }
}

void add_avlt(int elem, Node_AVLT*& root, int& time)
{
    time = 0;
    Node_AVLT* NewNode = new Node_AVLT(elem, NULL, NULL, NULL, 0);
    Node_AVLT* y = NULL;
    Node_AVLT* x = root;
    bool is_balance = true;
    //time = 0;
    int k = 0;
    while (x != NULL) {
        y = x;
        if (NewNode->data < x->data) {
            x = x->left;
            time++;
        }
        else {
            x = x->right;
            time++;
        }
    }
    NewNode->parent = y;
    if (y == NULL) {
        root = NewNode;
    }
    else if (NewNode->data < y->data) {
        y->left = NewNode;
    }
    else {
        y->right = NewNode;
    }
    Node_AVLT* Gp = NULL;
    if (NewNode->parent != NULL) {
        Gp = NewNode->parent->parent;
        if (Gp != NULL) {
            //k = height(Gp->left) - height(Gp->right);
            //if (abs(k) > 1) {
                add_balance_AVLT(root, Gp, time);
            //}
        }
    }
    //k = height(root->left) - height(root->right);
    /*if (abs(k) > 1) {
        is_balance = false;

    }*/
    /*if (!is_balance)
        add_balance_AVLT(root, NewNode, time);
    set_balansing_factors(root, NewNode);*/
}

struct Generator {
    int random;
};

Generator random_uniform() {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> point(0, 100);
    int random = point(mt);
    struct Generator Gen_elem;
    Gen_elem.random = random;
    return Gen_elem;
}

Generator random_exponential() {
    random_device rd;
    mt19937 mt(rd());
    exponential_distribution<> point (1);
    int random = (int)round(point(mt)*100/150);
    struct Generator Gen_elem;
    Gen_elem.random = random;
    return Gen_elem;
}

int main() {
    Node_RBT* root_RBT = NILL;
    Node_AVLT* root_AVLT = NULL;
    Node_BST* root_BST = NULL;
    setlocale(LC_ALL, "RUS");
    int num = 0;
    int count = 0;
    int time;
    cout << "Please, put the count of the element: " << '\n';
    cin >> count;
    cout << "Please, put the elements: " << '\n';
    for (int i = 0; i < count; i++) {
        cin >> num;
        add_bst(num, root_BST);
        add_rbt(num, root_RBT, time);
        add_avlt(num, root_AVLT, time);
    }
    set_balansing_factors(root_AVLT);
    cout << "Binary search tree: " << '\n';
    print(root_BST);
    cout << "\nheight = " << height(root_BST) << '\n';
    cout << "red-black tree: " << '\n';
    print(root_RBT);
    cout << "\nheight = " << height(root_RBT) << '\n';
    cout << "AVL-tree: " << '\n';
    print(root_AVLT);
    cout << "\nheight = " << height(root_AVLT) << '\n';
    /*map<int, int> height_rbt, height_avlt, height_bst;
    map<int, int> time_rbt, time_avlt;
    int time = 0;
    double time1 = 0;
    height_rbt[0] = 0;
    time_rbt[0] = 0;
    height_avlt[0] = 0;
    time_avlt[0] = 0;
    height_bst[0] = 0;
    for (int j = 1; j <= 100; j++) {
        for (int i = 0; i < 10000 - 1; i++) {
            num = random_uniform().random;
            add_bst(num, root_BST);
            add_rbt(num, root_RBT, time);
            add_avlt(num, root_AVLT, time);
        }
        num = random_exponential().random;
        time = 0;
        add_rbt(num, root_RBT, time);
        time = time * 1.27;
        time_rbt[j * 10000] = time;
        height_rbt[j * 10000] = height(root_RBT);
        time = 0;
        add_avlt(num, root_AVLT, time);
        time = time * 1.27;
        time_avlt[j * 10000] = time;
        height_avlt[j * 10000] = height(root_RBT);
        add_bst(num, root_BST);
        height_bst[j * 10000] = height(root_BST);
    }
    set_balansing_factors(root_AVLT);
    cout << "Normal distribution: " << '\n';
    cout << "BST: " << '\n';
    cout << "height: " << '\n';
    for (auto i : height_bst) {
        cout << '(' << i.first << ';' << i.second << ')' << ',';
    }
    cout << "\nRBT: " << '\n';
    cout << "\nheight: " << '\n';
    for (auto i : height_rbt) {
        cout << '(' << i.first << ';' << i.second << ')' << ',';
    }
    cout << "\ntime: " << '\n';
    for (auto i : time_rbt) {
        cout << '(' << i.first << ';' << i.second << ')' << ',';
    }
    cout << "\nAVLT: " << '\n';
    cout << "height: " << '\n';
    for (auto i : height_avlt) {
        cout << '(' << i.first << ';' << i.second << ')' << ',';
    }
    cout << "\ntime: " << '\n';
    for (auto i : time_avlt) {
        cout << '(' << i.first << ';' << i.second << ')' << ',';
    }*/
    return 0;
}
