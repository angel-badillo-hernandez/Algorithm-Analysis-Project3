#include <iostream>
#include <ctime>
#include <cstddef>

using namespace std;

template <class T>
class AVLTree
{
private:
    struct Node
    {
        T data{};
        size_t count{0};        // Count of duplicate values
        long balance_factor{0}; // AVL balance factor
        Node *left{nullptr};
        Node *right{nullptr};

        // Constructor for creating a new Node
        Node() {}

        Node(T data, Node *left = nullptr, Node *right = nullptr, size_t count = 1, long balance_factor = 0) : data(this->data), left(this->left), right(this->right), count(this->count), balance_factor(this->balance_factor) {}
    };

    Node *root{nullptr};
    void InsertNode(Node *&ptr, T data);
    void inorder(Node *nodePtr);
    void preorder(Node *nodePtr);
    void postorder(Node *nodePtr);
    void deleteNode(T, Node *&);
    void makeDeletion(Node *&nodePtr);
    size_t Height(Node *nodePtr);
    void ComputeAvlValues(Node *&nodePtr);
    bool LeftHeavy(Node *ptr);
    bool RightHeavy(Node *ptr);

public:
    long balance_factor(Node *nodePtr);
    AVLTree() {}
    ~AVLTree();

    void Insert(T data);
    void ShowInorder() { inorder(root); };
    void ShowPreorder() { preorder(root); };
    void ShowPostorder() { postorder(root); };
    bool SearchNode(T value);
    void Remove(T value) { deleteNode(value, root); };
    size_t TreeHeight();
    void GraphVizGetIds(Node *nodePtr, ofstream &VizOut);
    void GraphVizMakeConnections(Node *nodePtr, ofstream &VizOut);
    void GraphVizOut(string filename);

private:
    void RotateLeft(Node *&Node);
    void RotateRight(Node *&SubRoot);
};

template <class T>
AVLTree<T>::~AVLTree()
{
}

// The InsertNode method is a recursive private method that will be passed
// a pointer (root initially) and an integer to be added to the tree.
///////////////////////////////////////////////////////////////////////////////
template <class T>
void AVLTree<T>::InsertNode(Node *&ptr, T data)
{
    if (!ptr) // Insertion position found
        ptr = new Node(data);
    else if (x == ptr->data)
        ptr->count++;       // Update count of duplicate value
    else if (x < ptr->data) // Insert in the left subtree
        InsertNode(ptr->left, data);
    else // Insert in the right subtree
        InsertNode(ptr->right, data);
}

// The Insert method is a public method that calls InsertNode.
////////////////////////////////////////////////////////////////
template <class T>
void AVLTree<T>::Insert(T data)
{
    InsertNode(root, data);
    ComputeAvlValues(root);
}

// Prints the inorder traversal of the tree.
template <class T>
void AVLTree<T>::inorder(Node *nodePtr)
{
    if (nodePtr)
    {
        inorder(nodePtr->left);
        cout << nodePtr->value << " " << "(" << nodePtr->balance_factor << "/" << nodePtr->count << ")\n";
        inorder(nodePtr->right);
    }
}

// Prints the postorder traversal of the tree.
template <class T>
void AVLTree<T>::postorder(Node *nodePtr)
{
    if (nodePtr)
    {
        postorder(nodePtr->left);
        postorder(nodePtr->right);
        cout << nodePtr->value << " " << "(" << nodePtr->balance_factor << "/" << nodePtr->count << ")\n";
    }
}

// Prints the postorder traversal of the tree.
template <class T>
void AVLTree<T>::preorder(Node *nodePtr)
{
    if (nodePtr)
    {
        cout << nodePtr->value << " " << "(" << nodePtr->balance_factor << "/" << nodePtr->count << ")\n";
        preorder(nodePtr->left);
        preorder(nodePtr->right);
    }
}

template <class T>
bool AVLTree<T>::SearchNode(T value)
{
    Node *nodePtr = root;
    while (nodePtr)
    {
        if (nodePtr->value == value)
            return true;
        else if (value < nodePtr->value)
            nodePtr = nodePtr->left;
        else
            nodePtr = nodePtr->right;
    }
    return false;
}

template <class T>
void AVLTree<T>::deleteNode(T value, Node *&nodePtr)
{
    if (value < nodePtr->value)
        deleteNode(value, nodePtr->left);
    else if (value > nodePtr->value)
        deleteNode(value, nodePtr->right);
    else
        makeDeletion(nodePtr);
}

template <class T>
void AVLTree<T>::makeDeletion(Node *&nodePtr)
{
    Node *tempNodePtr;
    if (nodePtr == nullptr)
        cout << "Cannot delete empty node.\n";
    else if (nodePtr->right == nullptr)
    {
        tempNodePtr = nodePtr;
        nodePtr = nodePtr->left;
        delete tempNodePtr;
    }
    else if (nodePtr->left == nullptr)
    {
        tempNodePtr = nodePtr;
        nodePtr = nodePtr->right;
        delete tempNodePtr;
    }
    else
    {
        tempNodePtr = nodePtr->right;
        while (tempNodePtr->left)
            tempNodePtr = tempNodePtr->left;
        tempNodePtr->left = nodePtr->left;
        tempNodePtr = nodePtr;
        nodePtr = nodePtr->right;
        delete tempNodePtr;
    }
    ComputeAvlValues(nodePtr);
}

template <class T>
size_t AVLTree<T>::Height(Node *nodePtr)
{
    int left_height = 0;
    int right_height = 0;
    if (nodePtr == nullptr)
        return 0;
    else
    {
        left_height = Height(nodePtr->left);
        right_height = Height(nodePtr->right);
    }
    if (left_height > right_height)
        return 1 + left_height;
    else
        return 1 + right_height;
}

template <class T>
size_t AVLTree<T>::TreeHeight()
{
    return Height(root);
}

// Credit to:  Terry Griffin
// Method to help create GraphViz code so the expression tree can
// be visualized. This method prints out all the unique node id's
// by traversing the tree.
// Recivies a node pointer to root and performs a simple recursive
// tree traversal.
//////////////////////////////////////////////////////////////////////
template <class T>
void AVLTree<T>::GraphVizGetIds(Node *nodePtr, ofstream &VizOut)
{
    if (nodePtr)
    {
        GraphVizGetIds(nodePtr->left, VizOut);
        VizOut << "node" << nodePtr->value << "[label=\"" << nodePtr->value << " (" << nodePtr->balance_factor << "/" << nodePtr->count << ")\"]" << endl;
        GraphVizGetIds(nodePtr->right, VizOut);
    }
}

// Credit to:  Terry Griffin
// This method is partnered with the above method, but on this pass it
// writes out the actual data from each node.
///////////////////////////////////////////////////////////////////////
template <class T>
void AVLTree<T>::GraphVizMakeConnections(Node *nodePtr, ofstream &VizOut)
{
    static int x = 0;
    if (nodePtr)
    {
        x++;
        if (nodePtr->left)
            VizOut << "node" << nodePtr->value << "->" << "node" << nodePtr->left->value << endl;
        if (nodePtr->right)
            VizOut << "node" << nodePtr->value << "->" << "node" << nodePtr->right->value << endl;
        GraphVizMakeConnections(nodePtr->left, VizOut);
        GraphVizMakeConnections(nodePtr->right, VizOut);
    }
}

// Recieves a filename to place the GraphViz data into.
// It then calls the above two graphviz methods to create a data file
// that can be used to visualize your expression tree.
//////////////////////////////////////////////////////////////////////
template <class T>
void AVLTree<T>::GraphVizOut(string filename)
{
    ofstream VizOut;
    VizOut.open(filename.c_str());
    VizOut << "digraph { \n";
    GraphVizGetIds(root, VizOut);
    GraphVizMakeConnections(root, VizOut);
    VizOut << "}";
    VizOut.close();
}

template <class T>
bool AVLTree<T>::LeftHeavy(Node *ptr)
{
    return (Height(ptr->left) > Height(ptr->right));
}

template <class T>
bool AVLTree<T>::RightHeavy(Node *ptr)
{
    return (Height(ptr->right) > Height(ptr->left));
}

template <class T>
void AVLTree<T>::RotateLeft(Node *&SubRoot)
{
    if (SubRoot->right != nullptr && LeftHeavy(SubRoot->right))
    {
        RotateRight(SubRoot->right);
    }

    Node *Temp;
    Temp = SubRoot->right;
    SubRoot->right = Temp->left;
    Temp->left = SubRoot;
    SubRoot = Temp;

    ComputeAvlValues(SubRoot);
}

template <class T>
void AVLTree<T>::RotateRight(Node *&SubRoot)
{
    if (SubRoot->left != nullptr && RightHeavy(SubRoot->left))
    {
        RotateLeft(SubRoot->left);
    }
    Node *Temp;
    Temp = SubRoot->left;
    SubRoot->left = Temp->right;
    Temp->right = SubRoot;
    SubRoot = Temp;
    ComputeAvlValues(SubRoot);
}

template <class T>
long AVLTree<T>::balance_factor(Node *nodePtr)
{
    size_t leftheight = Height(nodePtr->left);
    size_t rightheight = Height(nodePtr->right);
    return leftheight - rightheight;
}

template <class T>
void AVLTree<T>::ComputeAvlValues(Node *&nodePtr)
{
    if (nodePtr)
    {
        ComputeAvlValues(nodePtr->left);
        ComputeAvlValues(nodePtr->right);
        nodePtr->balance_factor = balance_factor(nodePtr);
        if (nodePtr->balance_factor > 1)
            RotateRight(nodePtr);
        else if (nodePtr->balance_factor < -1)
            RotateLeft(nodePtr);
    }
}