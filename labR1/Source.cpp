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


public:
	Set() : root(nullptr) {}
	
	Set(int _data) {
		root = new node(_data);
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

	void print()
	{
		print_elements(root);
		std::cout << std::endl;
	}
};

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


}