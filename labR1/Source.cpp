#include <iostream>
#include <vector>
#include<time.h>


struct node {
    int data;
    node* left;
    node* right;

    node(int _data)
    {
        data = _data;
        left = nullptr;
        right = nullptr;
    }
};

class Set
{
private:
    node* root;
    size_t size;

    void print_elements(node* current)
    {
        if (!current)
        {
            std::cout << "nullptr ";
            return;
        }
        std::cout << current->data << " ";
        print_elements(current->left);
        print_elements(current->right);
    }

    node* min(node* root)
    {
        if (!root->left) { return root; }
        return min(root->left);
    }


    node* ric_erase(node*& root, int _key) {
        if (!root) {
            return root;
        }
        if (_key < root->data) {
            root->left = ric_erase(root->left, _key);
        }
        else if (_key > root->data) {
            root->right = ric_erase(root->right, _key);
        }
        else if (root->left && root->right)
        {
            root->data = min(root->right)->data;
            root->right = ric_erase(root->right, root->data);
        }
        else
        {
            if (root->left) {
                root = root->left;
            }
            else if (root->right) {
                root = root->right;
            }
            else {
                return nullptr;
            }

        }
        return root;

    }


    node* copy(node* n_root) {
        if (!n_root) {
            return nullptr;
        }
        node* temp = new node(n_root->data);
        temp->left = copy(n_root->left);
        temp->right = copy(n_root->right);
        return temp;
    }
    void clear_(node* n_root) {
        if (n_root->left) {
            clear_(n_root->left);
        }
        if (n_root->right) {
            clear_(n_root->right);
        }
        n_root = nullptr;
    }

    void GetKeys(node* n_root, std::vector<int>& result) const {
        if (n_root != nullptr) {
            GetKeys(n_root->left, result);
            result.push_back(n_root->data);
            GetKeys(n_root->right, result);
        }
    }

    int NodeCount(node* root) const
    {
        if (root->left == NULL && root->right == NULL)
            return 1;
        int left, right;
        if (root->left != NULL)
            left = NodeCount(root->left);
        else
            left = 0;
        if (root->right != NULL)
            right = NodeCount(root->right);
        else
            right = 0;

        return left + right + 1;
    }

    void difference(node* root, Set& bst, Set& result) {
        if (root == NULL) {
            return;
        }
        difference(root->left, bst, result);
        if (!bst.contains(root->data)) {
            result.insert(root->data);
        }
        difference(root->right, bst, result);
    }
public:
    Set() : root(nullptr), size(0) {}

    Set(int _data) {
        root = new node(_data);
        size = 1;
    }

    ~Set() {
        if (root) {
            clear_(root);
            delete root;
        }

    }

    Set(const Set& new_root) {
        if (!new_root.root) {
            clear_(root);
            delete root;
        }
        else {
            if (!root) {
                root = copy(new_root.root);
            }
            else {
                clear_(root);
                delete root;
                root = copy(new_root.root);
            }
        }
    }

    Set& operator=(const Set& new_root) {
        if (!new_root.root) {
            clear_(root);
            delete root;
            return *this;
        }
        else {
            if (!root) {
                root = copy(new_root.root);
            }
            else {
                clear_(root);
                delete root;
                root = copy(new_root.root);
            }
            return *this;
        }
    }

    bool contains(int _key)
    {
        node* current = root;
        while (current && current->data != _key)
        {
            if (current->data < _key)
            {
                current = current->right;

            }
            else
            {
                current = current->left;
            }
        }
        return current != nullptr;
    }

    bool insert(int _key)
    {
        if (!root) {
            root = new node(_key);
            return true;
        }

        if (contains(_key))
            return false;

        node* current = root;
        while (current) {
            if (current->data > _key)
            {
                if (current->left != nullptr)
                    current = current->left;

                else
                {
                    current->left = new node(_key);
                    return true;
                }
            }
            else
            {
                if (current->right != nullptr)
                    current = current->right;

                else
                {
                    current->right = new node(_key);
                    return true;
                }
            }
        }
    }
    bool erase(int _key) {
        if (!contains(_key)) return false;
        return ric_erase(root, _key);
    }

    void print()
    {
        print_elements(root);
        std::cout << std::endl;
    }
    void clear() {
        clear_(root);
    }

    /*int const GetKey() const
    {
        return root->data;
    }*/
    std::vector<int> GetAllKeys() const {
        std::vector<int> result;
        GetKeys(root, result);
        return result;
    }

    int const TreeCount() const {
        int count;
        count = NodeCount(root);
        return count;
    }

    Set& setDifference(Set& deductibleBST) {
        Set* result = new Set();
        difference(root, deductibleBST, *result);
        return *result;
    }
};



Set union_set(const Set& a, Set& b) {
    Set result;
    std::vector<int> elementsA = a.GetAllKeys();
    std::vector<int> elementsB = b.GetAllKeys();
    for (int i = 0; i < a.TreeCount(); i++) {
        result.insert(elementsA[i]);
    }
    for (int i = 0; i < b.TreeCount(); i++) {
        result.insert(elementsB[i]);
    }
    return result;
}

Set Difference(Set& a, Set& b) {
    Set result;
    result = union_set(a, b);
    int n;
    std::vector<int> elements;
    if (a.TreeCount() < b.TreeCount()) {
        n = a.TreeCount();
        elements = a.GetAllKeys();
        for (int i = 0; i < a.TreeCount(); i++) {
            if (elements[i]) {
                result.erase(elements[i]);
            }
        }
    }
    else {
        n = b.TreeCount();
        elements = b.GetAllKeys();
        for (int i = 0; i < b.TreeCount(); i++) {
            if (elements[i]) {
                result.erase(elements[i]);
            }
        }
    }
    return result;
}

Set SymDif(Set& a, Set& b) {
    auto res1 = Difference(a, b);
    auto res2 = Difference(b, a);

    return union_set(res1, res2);
}




size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

double avg(const double* ar) {
    double sum = 0;
    for (size_t i = 0; i < 100; ++i) {
        sum += ar[i];
    }
    return sum / 100;
}

double avg1000(const double* ar) {
    double sum = 0;
    for (size_t i = 0; i < 1000; ++i) {
        sum += ar[i];
    }
    return sum / 1000;
}


int main() {
    Set set1;
    Set set2(9);

    for (int i = 1; i < 6; i++)
    {
        set1.insert(i);
    }

    set2.insert(12);
    set2.insert(15);
    set2.insert(1);

    set1.print();
    set2.print();
    set1.erase(3);
    set1.print();
    Set res;
    res = union_set(set1, set2);
    res.print();

    Set SimRes;
    SimRes = SymDif(set1, set2);
    SimRes.print();



    std::cout << "Contains 40: " << set1.contains(40) << std::endl;
    std::cout << "Contains 1: " << set1.contains(1) << std::endl;

    Set timetest1000, timetest10000, timetest100000;
    std::vector<int> filltest1000, filltest10000, filltest100000;

    double fill_vattempts_1000[100], fill_vattempts_10000[100], fill_vattempts_100000[100];
    double fillattempts_1000[100], fillattempts_10000[100], fillattempts_100000[100];

    //«¿œŒÀÕ≈Õ»≈ Õ¿ 1000

    std::cout << "-------------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < 100; ++i) {
        int counter = 0;

        clock_t begin = clock();
        while (counter != 1000) {
            timetest1000.insert(lcg());
            ++counter;
        }
        clock_t end = clock();

        if (i != 99) { timetest1000.clear(); }
        fillattempts_1000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to fill tree with 1000 elem: " << avg(fillattempts_1000) << std::endl;

    //«¿œŒÀÕ≈Õ»≈ Õ¿ 10000
    for (size_t i = 0; i < 100; ++i) {
        int counter = 0;

        clock_t begin = clock();
        while (counter != 10000) {
            timetest10000.insert(lcg());
            ++counter;
        }
        clock_t end = clock();

        if (i != 99) { timetest10000.clear(); }
        fillattempts_10000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to fill tree with 10000 elem: " << avg(fillattempts_10000) << std::endl;

    //«¿œŒÀÕ≈Õ»≈ Õ¿ 100000
    for (size_t i = 0; i < 100; ++i) {
        int counter = 0;

        clock_t begin = clock();
        while (counter != 100000) {
            timetest100000.insert(lcg());
            ++counter;
        }
        clock_t end = clock();

        if (i != 99) { timetest100000.clear(); }
        fillattempts_100000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to fill tree with 100000 elem: " << avg(fillattempts_100000) << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;

    //«¿œŒÀÕ≈Õ»≈ std::vector Õ¿ 1000
    for (size_t i = 0; i < 100; ++i) {
        int counter = 0;

        clock_t begin = clock();
        while (counter != 1000) {
            filltest1000.push_back(lcg());
            ++counter;
        }
        clock_t end = clock();

        if (i != 99) { filltest1000.clear(); }
        fill_vattempts_1000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to fill vector with 1000 elem: " << avg(fill_vattempts_1000) << std::endl;

    //«¿œŒÀÕ≈Õ»≈ std::vector Õ¿ 10000
    for (size_t i = 0; i < 100; ++i) {
        int counter = 0;

        clock_t begin = clock();
        while (counter != 10000) {
            filltest10000.push_back(lcg());
            ++counter;
        }
        clock_t end = clock();

        if (i != 99) { filltest10000.clear(); }
        fill_vattempts_10000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to fill vector with 10000 elem: " << avg(fill_vattempts_10000) << std::endl;

    //«¿œŒÀÕ≈Õ»≈ std::vector Õ¿ 100000
    for (size_t i = 0; i < 100; ++i) {
        int counter = 0;

        clock_t begin = clock();
        while (counter != 100000) {
            filltest100000.push_back(lcg());
            ++counter;
        }
        clock_t end = clock();

        if (i != 99) { filltest100000.clear(); }
        fill_vattempts_100000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to fill vector with 100000 elem: " << avg(fill_vattempts_100000) << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;

    double findattempts_1000[1000], findattempts_10000[1000], findattempts_100000[1000];
    //œŒ»—  ›À≈Ã≈Õ“¿ ¬ ƒ≈–≈¬≈ 1000
    for (size_t i = 0; i < 1000; ++i) {

        clock_t begin = clock();
        timetest1000.contains(lcg());
        clock_t end = clock();


        findattempts_1000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to find elem in tree with 1000 elem: " << avg1000(findattempts_1000) << std::endl;

    //œŒ»—  ›À≈Ã≈Õ“¿ ¬ ƒ≈–≈¬≈ 10000
    for (size_t i = 0; i < 1000; ++i) {

        clock_t begin = clock();
        timetest10000.contains(lcg());
        clock_t end = clock();


        findattempts_10000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to find elem in tree with 10000 elem: " << avg1000(findattempts_10000) << std::endl;

    //œŒ»—  ›À≈Ã≈Õ“¿ ¬ ƒ≈–≈¬≈ 100000
    for (size_t i = 0; i < 1000; ++i) {

        clock_t begin = clock();
        timetest100000.contains(lcg());
        clock_t end = clock();
        //end

        findattempts_100000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to find elem in tree with 100000 elem: " << avg1000(findattempts_100000) << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;

    double vfindattempts_1000[1000], vfindattempts_10000[1000], vfindattempts_100000[1000];
    //œŒ»—  ›À≈Ã≈Õ“¿ ¬ ¬≈ “Œ–≈ 1000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        for (size_t i = 0; i < filltest1000.size(); ++i) {
            if (filltest1000[i] == elem) { break; }
        }
        clock_t end = clock();
        //end

        vfindattempts_1000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to find elem in std::vector with 1000 elem: " << avg1000(vfindattempts_1000) << std::endl;

    //œŒ»—  ›À≈Ã≈Õ“¿ ¬ ¬≈ “Œ–≈ 10000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        for (size_t i = 0; i < filltest10000.size(); ++i) {
            if (filltest10000[i] == elem) { break; }
        }
        clock_t end = clock();
        //end

        vfindattempts_10000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to find elem in std::vector with 10000 elem: " << avg1000(vfindattempts_10000) << std::endl;

    //œŒ»—  ›À≈Ã≈Õ“¿ ¬ ¬≈ “Œ–≈ 100000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        for (size_t i = 0; i < filltest100000.size(); ++i) {
            if (filltest100000[i] == elem) { break; }
        }
        clock_t end = clock();
        //end

        vfindattempts_100000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to find elem in std::vector with 100000 elem: " << avg1000(vfindattempts_100000) << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;

    double add_del_attempts_1000[1000], add_del_attempts_10000[1000], add_del_attempts_100000[1000];
    //ƒŒ¡¿¬À≈Õ»≈ » ”ƒ¿À≈Õ»≈ ¬ ƒ≈–≈¬≈ 1000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        timetest1000.insert(elem);
        timetest1000.erase(elem);
        clock_t end = clock();
        //end

        add_del_attempts_1000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to add and delete elem in set with 1000 elem: " << avg1000(add_del_attempts_1000) << std::endl;

    //ƒŒ¡¿¬À≈Õ»≈ » ”ƒ¿À≈Õ»≈ ¬ ƒ≈–≈¬≈ 10000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        timetest10000.insert(elem);
        timetest10000.erase(elem);
        clock_t end = clock();
        //end

        add_del_attempts_10000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to add and delete elem in set with 10000 elem: " << avg1000(add_del_attempts_10000) << std::endl;

    //ƒŒ¡¿¬À≈Õ»≈ » ”ƒ¿À≈Õ»≈ ¬ ƒ≈–≈¬≈ 100000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        timetest100000.insert(elem);
        timetest1000.erase(elem);
        clock_t end = clock();
        //end

        add_del_attempts_100000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to add and delete elem in set with 100000 elem: " << avg1000(add_del_attempts_100000) << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;

    double add_del_vecattempts_1000[1000], add_del_vecattempts_10000[1000], add_del_vecattempts_100000[1000];
    //ƒŒ¡¿¬À≈Õ»≈ » ”ƒ¿À≈Õ»≈ ¬ VECTOR 1000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        filltest1000.push_back(elem);
        filltest1000.pop_back();
        clock_t end = clock();
        //end

        add_del_vecattempts_1000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to add and delete elem in set with 100000 elem: " << avg1000(add_del_vecattempts_1000) << std::endl;



    //ƒŒ¡¿¬À≈Õ»≈ » ”ƒ¿À≈Õ»≈ ¬ VECTOR 10000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        filltest10000.push_back(elem);
        filltest10000.pop_back();
        clock_t end = clock();
        //end

        add_del_vecattempts_10000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to add and delete elem in set with 100000 elem: " << avg1000(add_del_vecattempts_10000) << std::endl;



    //ƒŒ¡¿¬À≈Õ»≈ » ”ƒ¿À≈Õ»≈ ¬ VECTOR 100000
    for (size_t i = 0; i < 1000; ++i) {
        int elem = lcg();
        //start
        clock_t begin = clock();
        filltest100000.push_back(elem);
        filltest100000.pop_back();
        clock_t end = clock();
        //end

        add_del_vecattempts_100000[i] = double(end - begin) / double(CLOCKS_PER_SEC);
    }
    std::cout << "Average time to add and delete elem in set with 100000 elem: " << avg1000(add_del_vecattempts_100000) << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;

}
