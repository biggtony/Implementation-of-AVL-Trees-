#include <iostream>
#include <algorithm>
#include <queue>
#include <string>
#include <limits>

// Node class to represent each node in the AVL Tree
class Node {
public:
    int key;          // Value stored in the node
    int height;       // Height of the node's subtree
    Node* left;       // Pointer to left child
    Node* right;      // Pointer to right child

    // Constructor to initialize a node with a key
    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

// AVL Tree class with operations optimized for online compilers
class AVLTree {
private:
    Node* root; // Root of the tree

    // Helper function to get height of a node
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    // Helper function to compute balance factor
    int getBalance(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Helper function to update height of a node
    void updateHeight(Node* node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        }
    }

    // Right rotation to balance a left-heavy node
    Node* rightRotate(Node* y) {
        Node* x = y->left;          // Left child becomes new root
        Node* T2 = x->right;        // Right subtree of left child
        x->right = y;               // Make y the right child of x
        y->left = T2;               // Attach T2 as y's left child
        updateHeight(y);            // Update y's height first
        updateHeight(x);            // Update x's height
        std::cout << "Performed right rotation on node " << y->key << std::endl;
        return x;                   // Return new root
    }

    // Left rotation to balance a right-heavy node
    Node* leftRotate(Node* x) {
        Node* y = x->right;         // Right child becomes new root
        Node* T2 = y->left;         // Left subtree of right child
        y->left = x;                // Make x the left child of y
        x->right = T2;              // Attach T2 as x's right child
        updateHeight(x);            // Update x's height first
        updateHeight(y);            // Update y's height
        std::cout << "Performed left rotation on node " << x->key << std::endl;
        return y;                   // Return new root
    }

    // Recursive insertion function with balance checks
    Node* insertNode(Node* node, int key) {
        // Perform standard BST insertion
        if (!node) {
            std::cout << "Created new node with key " << key << std::endl;
            return new Node(key);
        }
        if (key < node->key) {
            std::cout << "Moving left from node " << node->key << std::endl;
            node->left = insertNode(node->left, key);
        } else if (key > node->key) {
            std::cout << "Moving right from node " << node->key << std::endl;
            node->right = insertNode(node->right, key);
        } else {
            std::cout << "Duplicate key " << key << " ignored" << std::endl;
            return node; // Duplicate keys not allowed
        }

        // Update height of current node
        updateHeight(node);
        std::cout << "Updated height of node " << node->key << " to " << node->height << std::endl;

        // Check balance factor
        int balance = getBalance(node);
        std::cout << "Balance factor of node " << node->key << ": " << balance << std::endl;

        // Left-Left Case
        if (balance > 1 && key < node->left->key) {
            std::cout << "Left-Left imbalance detected at node " << node->key << std::endl;
            return rightRotate(node);
        }
        // Right-Right Case
        if (balance < -1 && key > node->right->key) {
            std::cout << "Right-Right imbalance detected at node " << node->key << std::endl;
            return leftRotate(node);
        }
        // Left-Right Case
        if (balance > 1 && key > node->left->key) {
            std::cout << "Left-Right imbalance detected at node " << node->key << std::endl;
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right-Left Case
        if (balance < -1 && key < node->right->key) {
            std::cout << "Right-Left imbalance detected at node " << node->key << std::endl;
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Find the node with the minimum key in a subtree
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left) {
            std::cout << "Finding successor, moving left from node " << current->key << std::endl;
            current = current->left;
        }
        std::cout << "Minimum value node found: " << current->key << std::endl;
        return current;
    }

    // Recursive deletion function with balance checks
    Node* deleteNode(Node* node, int key) {
        if (!node) {
            std::cout << "Node with key " << key << " not found for deletion" << std::endl;
            return node;
        }

        // Perform standard BST deletion
        if (key < node->key) {
            std::cout << "Moving left from node " << node->key << " to delete " << key << std::endl;
            node->left = deleteNode(node->left, key);
        } else if (key > node->key) {
            std::cout << "Moving right from node " << node->key << " to delete " << key << std::endl;
            node->right = deleteNode(node->right, key);
        } else {
            // Node with one or no child
            if (!node->left) {
                Node* temp = node->right;
                std::cout << "Deleting node " << node->key << " with 0 or 1 child (right)" << std::endl;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                std::cout << "Deleting node " << node->key << " with 0 or 1 child (left)" << std::endl;
                delete node;
                return temp;
            }

            // Node with two children
            Node* temp = minValueNode(node->right);
            std::cout << "Replacing node " << node->key << " with successor " << temp->key << std::endl;
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }

        // Update height
        updateHeight(node);
        std::cout << "Updated height of node " << node->key << " to " << node->height << std::endl;

        // Check balance factor
        int balance = getBalance(node);
        std::cout << "Balance factor of node " << node->key << ": " << balance << std::endl;

        // Left-Left Case
        if (balance > 1 && getBalance(node->left) >= 0) {
            std::cout << "Left-Left imbalance detected at node " << node->key << " after deletion" << std::endl;
            return rightRotate(node);
        }
        // Left-Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            std::cout << "Left-Right imbalance detected at node " << node->key << " after deletion" << std::endl;
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right-Right Case
        if (balance < -1 && getBalance(node->right) <= 0) {
            std::cout << "Right-Right imbalance detected at node " << node->key << " after deletion" << std::endl;
            return leftRotate(node);
        }
        // Right-Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            std::cout << "Right-Left imbalance detected at node " << node->key << " after deletion" << std::endl;
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Recursive search function
    Node* searchNode(Node* node, int key) {
        if (!node) {
            std::cout << "Reached null node while searching for " << key << std::endl;
            return nullptr;
        }
        if (node->key == key) {
            std::cout << "Found node with key " << key << std::endl;
            return node;
        }
        if (key < node->key) {
            std::cout << "Moving left from node " << node->key << std::endl;
            return searchNode(node->left, key);
        }
        std::cout << "Moving right from node " << node->key << std::endl;
        return searchNode(node->right, key);
    }

    // In-order traversal for printing keys in sorted order
    void inOrder(Node* node) {
        if (node) {
            inOrder(node->left);
            std::cout << node->key << " ";
            inOrder(node->right);
        }
    }

    // Pre-order traversal for additional tree exploration
    void preOrder(Node* node) {
        if (node) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    // Level-order traversal for tree structure visualization
    void levelOrder(Node* node) {
        if (!node) {
            std::cout << "Tree is empty" << std::endl;
            return;
        }
        std::queue<Node*> q;
        q.push(node);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            std::cout << current->key << "(h=" << current->height << ") ";
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        std::cout << std::endl;
    }

    // Text-based tree visualization (Bonus feature for console output)
    void printTree(Node* node, int level, std::string prefix) {
        if (!node) return;
        printTree(node->right, level + 1, prefix + "    ");
        std::cout << prefix << node->key << "(h=" << node->height << ")" << std::endl;
        printTree(node->left, level + 1, prefix + "    ");
    }

    // Helper function to clear input buffer
    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Helper function to validate integer input
    bool getValidIntegerInput(int& value) {
        std::cout << "Enter an integer value: ";
        if (std::cin >> value) {
            clearInputBuffer();
            return true;
        }
        std::cout << "Invalid input. Please enter a valid integer." << std::endl;
        clearInputBuffer();
        return false;
    }

public:
    AVLTree() : root(nullptr) {}

    // Public insert function
    void insert(int key) {
        std::cout << "\nInserting key " << key << " into AVL Tree" << std::endl;
        root = insertNode(root, key);
        std::cout << "Insertion complete. Current tree state:" << std::endl;
        printInOrder();
        printLevelOrder();
        printTreeStructure();
    }

    // Public delete function
    void remove(int key) {
        std::cout << "\nDeleting key " << key << " from AVL Tree" << std::endl;
        root = deleteNode(root, key);
        std::cout << "Deletion complete. Current tree state:" << std::endl;
        printInOrder();
        printLevelOrder();
        printTreeStructure();
    }

    // Public search function
    bool search(int key) {
        std::cout << "\nSearching for key " << key << " in AVL Tree" << std::endl;
        Node* result = searchNode(root, key);
        if (result) {
            std::cout << "Key " << key << " found in the tree" << std::endl;
            return true;
        } else {
            std::cout << "Key " << key << " not found in the tree" << std::endl;
            return false;
        }
    }

    // Public in-order print function
    void printInOrder() {
        std::cout << "In-order traversal: ";
        if (!root) std::cout << "Tree is empty";
        else inOrder(root);
        std::cout << std::endl;
    }

    // Public pre-order print function
    void printPreOrder() {
        std::cout << "Pre-order traversal: ";
        if (!root) std::cout << "Tree is empty";
        else preOrder(root);
        std::cout << std::endl;
    }

    // Public level-order print function
    void printLevelOrder() {
        std::cout << "Level-order traversal: ";
        levelOrder(root);
    }

    // Public tree visualization function
    void printTreeStructure() {
        std::cout << "Tree structure:\n";
        if (!root) std::cout << "Tree is empty" << std::endl;
        else printTree(root, 0, "");
    }

    // Interactive menu for user operations, limited to 10 iterations for online compilers
    void runInteractiveMenu() {
        const int MAX_OPERATIONS = 10; // Limit iterations for online compiler compatibility
        int operationCount = 0;

        while (operationCount < MAX_OPERATIONS) {
            std::cout << "\n=== AVL Tree Interactive Menu (Operation " << operationCount + 1 << "/" << MAX_OPERATIONS << ") ===\n";
            std::cout << "1. Insert a key\n";
            std::cout << "2. Delete a key\n";
            std::cout << "3. Search for a key\n";
            std::cout << "4. Print In-order Traversal\n";
            std::cout << "5. Print Pre-order Traversal\n";
            std::cout << "6. Print Level-order Traversal\n";
            std::cout << "7. Print Tree Structure\n";
            std::cout << "8. Exit\n";
            std::cout << "Enter your choice (1-8): ";

            int choice;
            if (std::cin >> choice && choice >= 1 && choice <= 8) {
                clearInputBuffer();
                if (choice == 8) {
                    std::cout << "Exiting interactive menu." << std::endl;
                    break;
                }

                operationCount++; // Increment operation count

                switch (choice) {
                    case 1: {
                        int key;
                        if (getValidIntegerInput(key)) {
                            insert(key);
                        }
                        break;
                    }
                    case 2: {
                        int key;
                        if (getValidIntegerInput(key)) {
                            remove(key);
                        }
                        break;
                    }
                    case 3: {
                        int key;
                        if (getValidIntegerInput(key)) {
                            search(key);
                        }
                        break;
                    }
                    case 4:
                        printInOrder();
                        break;
                    case 5:
                        printPreOrder();
                        break;
                    case 6:
                        printLevelOrder();
                        break;
                    case 7:
                        printTreeStructure();
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            } else {
                std::cout << "Invalid input. Please enter a number between 1 and 8." << std::endl;
                clearInputBuffer();
            }

            if (operationCount >= MAX_OPERATIONS) {
                std::cout << "Maximum operations reached. Exiting interactive menu." << std::endl;
                break;
            }
        }
    }
};

// Main function to demonstrate AVL Tree operations
int main() {
    AVLTree tree;

    // Demonstrate initial operations as per assignment requirements
    std::cout << "=== Demonstrating Initial AVL Tree Operations ===\n";
    int initialValues[] = {10, 20, 30, 40, 50, 25, 15, 5, 60, 70};
    for (int val : initialValues) {
        tree.insert(val);
    }

    std::cout << "\nInitial Tree State:\n";
    tree.printInOrder();
    tree.printLevelOrder();
    tree.printTreeStructure();

    tree.search(25);
    tree.search(100);

    tree.remove(20);
    tree.remove(30);

    std::cout << "\nFinal Tree State after Deletions:\n";
    tree.printInOrder();
    tree.printLevelOrder();
    tree.printTreeStructure();

    // Start interactive menu
    std::cout << "\n=== Starting Interactive Mode (Limited to 10 Operations) ===\n";
    tree.runInteractiveMenu();

    return 0;
}