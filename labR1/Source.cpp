#include <iostream>


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

	void print_elements(node*current)
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


	node* r_erase(node*& root, int _key) {
		if (!root) {
			return root;
		}
		if (_key < root->data) {
			root->left = r_erase(root->left, _key);
		}
		else if (_key > root->data) {
			root->right = r_erase(root->right, _key);
		}
		else if (root->left && root->right)
		{
			root->data = min(root->right)->data;
			root->right = r_erase(root->right, root->data);
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


public:
	Set() : root(nullptr) {}
	
	Set(int _data) {
		root = new node(_data);
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
		while(current && current->data != _key)
		{
			if(current->data < _key)
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
			if(current->data > _key)
			{
				if(current->left != nullptr)
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
		return r_erase(root, _key);
	}

	void print()
	{
		print_elements(root);
		std::cout << std::endl;
	}
	void clear() {
		clear_(root);
	}
};

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


int main(){
	Set set1;
	Set set2(9);

	for(int i = 1; i < 6; i++)
	{
		set1.insert(i);
	}

	set2.insert(12);
	set2.insert(15);
	set2.insert(1);

	set1.print();
	set2.print();

	

	std::cout << "Contains 40: " << set1.contains(40) << std::endl;
	std::cout << "Contains 21: " << set1.contains(21) << std::endl;


}