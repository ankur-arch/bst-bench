#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;

// function to swap elements
void swap(double *a, double *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// function to print the array
void printArray(double array[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

// function to rearrange array (find the partition point)
int partition(double array[], int low, int high)
{

    // select the rightmost element as pivot
    int pivot = array[high];

    // pointer for greater element
    int i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++)
    {
        if (array[j] <= pivot)
        {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&array[i], &array[j]);
        }
    }

    // swap pivot with the greater element at i
    swap(&array[i + 1], &array[high]);

    // return the partition point
    return (i + 1);
}

void quickSort(double array[], int low, int high)
{
    if (low < high)
    {

        // find the pivot element such that
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on righ of pivot
        int pi = partition(array, low, high);

        // recursive call on the left of pivot
        quickSort(array, low, pi - 1);

        // recursive call on the right of pivot
        quickSort(array, pi + 1, high);
    }
}

template <typename T>
const void LOG(const T message, bool withNewLine = true)
{
    const string newline = withNewLine ? "\n" : "";
    cout << message << newline;
    return;
}

bool compareDouble(double a, double b, double epsilon)
{
    return abs(a - b) < epsilon;
}

class Node
{
public:
    Node *left = nullptr, *right = nullptr;
    double value;

    Node(double val) : value(val){};

    Node(Node *lchild, Node *rchild, double val) : left(lchild),
                                                   right(rchild),
                                                   value(val){};

    Node()
    {
        // LOG("created new Node");
    }

    ~Node()
    {
        // LOG("Node Object Destroyed");
        delete left, right;
    }
};

class BinaryTree
{
    Node *root = nullptr;

    Node *insert(double value, Node *cursor)
    {
        if (cursor == nullptr && root == nullptr)
        {
            // LOG("Inserting root node in BST "+to_string(value));
            root = new Node(value);
            return root;
        }

        if (cursor == nullptr)
        {
            // LOG("Inserting node in BST "+to_string(value));
            return new Node(value);
        }

        if (value < cursor->value)
        {
            cursor->left = insert(value, cursor->left);
        }
        else
        {
            cursor->right = insert(value, cursor->right);
        }

        return cursor;
    }

    void inorder(Node *root)
    {
        if (root == nullptr)
        {
            return;
        }
        inorder(root->left);
        LOG(" " + to_string(root->value) + " ", false);
        inorder(root->right);
    }

    Node *search(Node *cursor, double value)
    {

        if (cursor == nullptr)
        {
            // LOG("Key not found "+ to_string(value));
            return cursor;
        }

        if (compareDouble(cursor->value, value, 0.000000001))
        {
            // LOG("Key found "+ to_string(value));
            return cursor;
        }

        if (value < cursor->value)
        {

            search(cursor->left, value);
        }

        if (value > cursor->value)
        {
            search(cursor->right, value);
        }

        return cursor;
    }

    int depth(Node *cursor)
    {
        if (cursor == nullptr)
        {
            return 0;
        }

        int leftTreeHeight = depth(cursor->left);
        int rightTreeHeight = depth(cursor->right);
        return max(leftTreeHeight, rightTreeHeight) + 1;
    }

    int size(Node *cursor)
    {
        if (cursor == nullptr)
        {
            return 0;
        }

        int leftTreeHeight = depth(cursor->left);
        int rightTreeHeight = depth(cursor->right);
        return leftTreeHeight + rightTreeHeight + 1;
    }

public:
    BinaryTree()
    {
        // LOG("Binary tree initialized");
    }

    BinaryTree(Node *rootVal)
    {
        root = rootVal;
    };

    bool insert(double val)
    {
        return insert(val, root) != nullptr;
    }

    void inorder()
    {
        inorder(root);
        LOG("");
        return;
    }

    Node *search(double value)
    {
        return search(root, value);
    }

    int depth()
    {
        const int result = depth(root);
        // LOG("Depth of tree is "+ to_string(result));
        return result;
    }

    int size()
    {
        const int result = size(root);
        // LOG("Size of tree is "+ to_string(result));
        return result;
    }

    ~BinaryTree()
    {
        delete root;
        // LOG("Binary tree destroyed");
    };
};

double generateRandomDouble(double max = 1000000.0)
{
    return double(rand()) / (double(max) + 1.0);
}

void generateRandomNumbers(double n[], int size, bool sort = false)
{

    // seed
    srand(static_cast<unsigned int>(clock()));
    for (int i = 0; i < size; i++)
    {
        n[i] = generateRandomDouble();
    }

    if (sort)
    {
        quickSort(n, 0, size);
    }

    return;
}

void benchmark(int size, bool sorted)
{
    double arr[size];
    BinaryTree bst = BinaryTree();
    generateRandomNumbers(arr, size, sorted);
    long long total_timings[4] = {0, 0, 0, 0};

    for (int i = 0; i < size; i++)
    {
        auto insert_start = high_resolution_clock::now();
        bst.insert(arr[i]);
        auto insert_end = high_resolution_clock::now();
        auto size_start = high_resolution_clock::now();
        bst.size();
        auto size_end = high_resolution_clock::now();
        auto depth_start = high_resolution_clock::now();
        bst.depth();
        auto depth_end = high_resolution_clock::now();
        auto search_start = high_resolution_clock::now();
        bst.search(arr[i]);
        auto search_end = high_resolution_clock::now();

        auto insert_time = insert_end - insert_start;
        auto size_time = size_end - size_start;
        auto depth_time = depth_end - depth_start;
        auto search_time = search_end - search_start;

        total_timings[0] = total_timings[0] + (duration_cast<nanoseconds>(insert_time).count());
        total_timings[1] = total_timings[1] + (duration_cast<nanoseconds>(size_time).count());
        total_timings[2] = total_timings[2] + (duration_cast<nanoseconds>(depth_time).count());
        total_timings[3] = total_timings[3] + (duration_cast<nanoseconds>(search_time).count());
    }

    string sortString = (sorted == true ? "sorted" : "unsorted");
    string title = "BST Operations time taken for " + sortString + " input of array of size " + to_string(size);
    LOG(title);
    LOG("----------- AVERAGE TIME ---------------");
    LOG("INSERT " + to_string(total_timings[0] / size) + " nanoseconds");
    LOG("SIZE " + to_string(total_timings[1] / size) + " nanoseconds");
    LOG("DEPTH " + to_string(total_timings[2] / size) + " nanoseconds");
    LOG("SEARCH " + to_string(total_timings[3] / size) + " nanoseconds");
}

int main()
{

    LOG("----------size: 100------------");

    benchmark(100, false);
    benchmark(100, true);

    LOG("----------size: 1000-----------");

    benchmark(1000, false);
    benchmark(1000, true);

    LOG("----------size: 10000----------");

    benchmark(10000, false);
    benchmark(10000, true);


    return 0;
};