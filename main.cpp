#include <iostream>
#include <queue>
#include <fstream>

typedef int item_type;
typedef struct tree{
    item_type  item;
    struct tree *parent;
    struct tree *left;
    struct tree *right;
}tree;


void add_tree(tree **t, item_type item, tree *parent) {
    tree *p;
    if (*t == NULL) {
        p = (tree *)malloc(sizeof(tree));
        p ->left = p ->right = NULL;
        p ->parent = parent;
        p ->item = item;
        *t = p;
        return;
    }

    if(item < (*t) -> item){
        add_tree(&((*t) ->left), item, *t);
    } else{
        add_tree(&((*t)-> right), item, *t);
    }
}


tree *search_tree(tree *t, item_type i){
    if(t == NULL){
        return NULL;
    }
    if(t->item == i){
        return t;
    }
    if(i < t->item){
        return search_tree(t->left, i);
    } else{
        return search_tree(t->right, i);
    }
}

tree *find_min(tree *t){
    if (t == NULL) {
        return  NULL;
    }
    tree  *min = t;
    while (min ->left !=NULL){
        min = min->left;
    }
    return min;
}

void del_element(tree **t, item_type item) {
    if (t == NULL) return;
    tree *element=search_tree(*t, item);
    if (element == NULL) return;
    int hasParent=element->parent != NULL;
    int isLeft=hasParent && element->item < element->parent->item;
    if (element->left == NULL && element->right == NULL) { // bez deca
        if (hasParent) {
            if (isLeft) {
                element->parent->left = NULL;
            } else {
                element->parent->right = NULL;
            }
        }
        free(element);
    } else if (element->left != NULL && element->right == NULL) { // samo lqvo
        element->left->parent = element->parent;
        if (hasParent) {
            if (isLeft) {
                element->parent->left = element->left;
            } else {
                element->parent->right = element->left;
            }
        } else {
            *t = element->left;
        }
        free(element);
    } else if (element->left == NULL && element->right != NULL) {
        element->right->parent = element->parent;
        if (hasParent) {
            if(isLeft) {
                element->parent->left = element->right;
            } else {
                element->parent->right = element->right;
            }

            free(element);
        } else {
            tree *right_min = find_min(element->right);
            element->item = right_min->item;
            del_element(&right_min, right_min->item);
        }
    }
}


void printInOrder(tree *t) {
    if (t == NULL) return;
    printInOrder(t->left);
    std::cout << t->item << " ";
    printInOrder(t->right);
}

void printInPreOrder(tree *t) {
    if (t == NULL) return;
    std::cout << t->item << " ";
    printInPreOrder(t->left);
    printInPreOrder(t->right);
}

void printInPostOrder(tree *t) {
    if (t == NULL) return;
    printInPostOrder(t->left);
    printInPostOrder(t->right);
    std::cout << t->item << " ";
}

item_type find_max(tree *t) {
    if (t == NULL) {
        return 0;
    }

    while (t->right != NULL) {
        t = t->right;
    }

    return t->item;
}

void increase_tree_elements(tree *t, int valueToAdd) {
    if (t == NULL) {
        return;
    }

    t->item += valueToAdd;

    increase_tree_elements(t->left, valueToAdd);
    increase_tree_elements(t->right, valueToAdd);
}

int countDeepestLevelElements(tree *root) {
    if (root == NULL) {
        return 0;
    }

    std::queue<tree*> q;
    q.push(root);
    int deepestLevelCount = 0;

    while (!q.empty()) {
        int levelSize = q.size();


        for (int i = 0; i < levelSize; i++) {
            tree *current = q.front();
            q.pop();

            if (current->left != NULL) {
                q.push(current->left);
            }
            if (current->right != NULL) {
                q.push(current->right);
            }
        }

        deepestLevelCount = levelSize;
    }

    return deepestLevelCount;
}

void readNumbersFromFile(tree** root, const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open the file " << filename << std::endl;
        return;
    }

    item_type number;
    while (file >> number) {
        add_tree(root, number, NULL);
    }

    file.close();
}


int main() {
    tree *root = NULL;

//    add_tree(&root, 9, NULL);
//    add_tree(&root, 16, NULL);
//    add_tree(&root, 6, NULL);
//    add_tree(&root, 5, NULL);
//    add_tree(&root, 62, NULL);
//    add_tree(&root, 126, NULL);
//    add_tree(&root, 10, NULL);
    const char* filename = "/Users/moldovexc/CLionProjects/binary]/input.txt";
    readNumbersFromFile(&root, filename);

    std::cout << "In-Order: ";
    printInOrder(root);
    std::cout << std::endl;

    std::cout << "Pre-Order: ";
    printInPreOrder(root);
    std::cout << std::endl;

    std::cout << "Post-Order: ";
    printInPostOrder(root);
    std::cout << std::endl;

    item_type max_element = find_max(root); //podavame korena
    std::cout << "Maximum Element: " << max_element << std::endl;

    int valueToAdd = 5;
    increase_tree_elements(root, valueToAdd);

    std::cout << "Increased Tree Elements by " << valueToAdd << std::endl;

    std::cout << "In-Order after increase: ";
    printInOrder(root);
    std::cout << std::endl;

    std::cout << "Pre-Order after increase: ";
    printInPreOrder(root);
    std::cout << std::endl;

    std::cout << "Post-Order after increase: ";
    printInPostOrder(root);
    std::cout << std::endl;


    int deepestLevelCount = countDeepestLevelElements(root);
    std::cout << "Number of elements in the deepest level: " << deepestLevelCount << std::endl;
    return 0;
}
