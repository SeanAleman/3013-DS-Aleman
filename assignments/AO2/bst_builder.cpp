/********************************************************************
Sean Aleman - Program 1 - BST
This program accepts an input file which has 1 number per line on it.
It takes these numbers and puts them in a vector.
Then it sorts the vector.
Then it takes the sorted vector, and makes a binary tree out of it.
Then the program prints out the binary tree in a way to upload it 
to webgraphiz easily for easy viewing. Is a complete tree when 2^n-1 
values are on the input file.
The BST outputs to a file, and there is some informative information 
about the vector printed to the screen, but not to the output file.
*******************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
/////http://www.webgraphviz.com/

using namespace std;
// creates node structure that is going to make up the BST
struct node
{
	int data;
	node *left;
	node *right;
	node()
	{
		data = -1;
		left = NULL;
		right = NULL;
	}
	node(int x)
	{
		data = x;
		left = NULL;
		right = NULL;
	}
};

class BSTree
{
private:
public:
	node *root;
	// counts to see how many are in the tree
	int count(node *root)
	{
		if (!root)
		{
			return 0;
		}
		else
		{
			return 1 + count(root->left) + count(root->right);
		}
	}
	// inserts value into the node. Checks to see if there is a value first, 
	// before traversing down the tree.
	void insert(node *&root, node *&temp)
	{
		if (!root)
		{
			root = temp;
		}
		else
		{
			if (temp->data < root->data)
			{
				insert(root->left, temp);
			}
			else
			{
				insert(root->right, temp);
			}
		}
	}
	// takes a sorted vector and calls the helper correctly.
	// For convenience, it is set up this way
	void sortedvectorToBST(vector<int> nums) {
		helper(nums, 0, nums.size() - 1);
	}
	// the helper calls the insert when it needs to make the new node. This is the 
	// real function that takes the vector and cuts it in half to grab the mid value 
	// so that the binary tree can be created correctly. 
	void helper(vector<int>& nums, int l, int r) {
		if (l > r)
			return;
		int mid = (l + r) / 2;
		insert(nums[mid]);

		helper(nums, mid + 1, r);
		helper(nums, l, mid - 1);
		return;
	}
	// Traverses the BST and prints out in a way to use graphiz easily
	void print_node(node *n, string label = "")
	{
		if (label != "")
		{
			cout << "[" << label << "]";
		}
		cout << "[[" << n << "][" << n->data << "]]\n";
		if (n->left)
		{
			cout << "\t|-->[L][[" << n->left << "][" << n->left->data << "]]\n";
		}
		else
		{
			cout << "\t\\-->[L][null]\n";
		}
		if (n->right)
		{
			cout << "\t\\-->[R][[" << n->right << "][" << n->right->data << "]]\n";
		}
		else
		{
			cout << "\t\\-->[R][null]\n";
		}
	}
	// finds the min value in the bst
	node *minValueNode(node *root)
	{
		node *current = root;

		if (root->right)
		{
			current = root->right;
			while (current->left != NULL)
			{
				current = current->left;
			}
		}
		else if (root->left)
		{
			current = root->left;
			while (current->right != NULL)
			{
				current = current->right;
			}
		}

		return current;
	}
	// finds height of tree, but first checks to see if tree is empty. 
	int height(node *root)
	{
		if (!root)
		{
			return 0;
		}
		else
		{
			int left = height(root->left);
			int right = height(root->right);
			if (left > right)
			{
				return left + 1;
			}
			else
			{
				return right + 1;
			}
		}
	}

	/* Print nodes at a given level */
	void printGivenLevel(node *root, int level)
	{
		if (root == NULL)
			return;
		if (level == 1)
		{
			print_node(root);
		}
		else if (level > 1)
		{
			printGivenLevel(root->left, level - 1);
			printGivenLevel(root->right, level - 1);
		}
	}
	//************************************************************************
	// Method to help create GraphViz code so the expression tree can
	// be visualized. This method prints out all the unique node id's
	// by traversing the tree.
	// Recivies a node pointer to root and performs a simple recursive
	// tree traversal.
	//************************************************************************
	void GraphVizGetIds(node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizGetIds(nodePtr->left, VizOut);
			VizOut << "node" << nodePtr->data
				<< "[label=\"" << nodePtr->data << "\\n"
				//<<"Add:"<<nodePtr<<"\\n"
				//<<"Par:"<<nodePtr->parent<<"\\n"
				//<<"Rt:"<<nodePtr->right<<"\\n"
				//<<"Lt:"<<nodePtr->left<<"\\n"
				<< "\"]" << endl;
			if (!nodePtr->left)
			{
				NullCount++;
				VizOut << "nnode" << NullCount;
				VizOut << "[label=\"X\",shape=point,width=.15]" << endl;
			}
			GraphVizGetIds(nodePtr->right, VizOut);
			if (!nodePtr->right)
			{
				NullCount++;
				VizOut << "nnode" << NullCount;
				VizOut << "[label=\"X\",shape=point,width=.15]" << endl;
			}
		}
	}
	//************************************************************************
	// This method is partnered with the above method, but on this pass it
	// writes out the actual data from each node.
	// Don't worry about what this method and the above method do, just
	// use the output as your told:)
	//************************************************************************
	void GraphVizMakeConnections(node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizMakeConnections(nodePtr->left, VizOut);
			if (nodePtr->left)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->left->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			if (nodePtr->right)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->right->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			GraphVizMakeConnections(nodePtr->right, VizOut);
		}
	}

public:
	BSTree()
	{
		root = NULL;
	}
	~BSTree()
	{
	}

	int count()
	{
		return count(root);
	}

	void insert(int x)
	{
		node *temp = new node(x);
		insert(root, temp);
	}

	void minValue()
	{
		print_node(minValueNode(root), "minVal");
	}

	int height()
	{

		return height(root);
	}

	int top()
	{
		if (root)
			return root->data;
		else
			return 0;
	}

	/* Function to line by line print level order traversal a tree*/
	void printLevelOrder()
	{
		cout << "Begin Level Order===================\n";
		int h = height(root);
		int i;
		for (i = 1; i <= h; i++)
		{
			printGivenLevel(root, i);
			cout << "\n";
		}
		cout << "End Level Order===================\n";
	}

	//************************************************************************
	// Recieves a filename to place the GraphViz data into.
	// It then calls the above two graphviz methods to create a data file
	// that can be used to visualize your expression tree.
	//************************************************************************
	void GraphVizOut(string filename)
	{
		ofstream VizOut;
		VizOut.open(filename);
		VizOut << "Digraph G {\n";
		GraphVizGetIds(root, VizOut);
		GraphVizMakeConnections(root, VizOut);
		VizOut << "}\n";
		VizOut.close();
	}
};

// a bubble sort to sort the vector
void bubbleSort(vector<int>& a)
{
	bool swapp = true;
	while (swapp) {
		swapp = false;
		for (size_t i = 0; i < a.size() - 1; i++) {
			if (a[i] > a[i + 1]) {
				a[i] += a[i + 1];
				a[i + 1] = a[i] - a[i + 1];
				a[i] -= a[i + 1];
				swapp = true;
			}
		}
	}
}

// to print the vector to verify functionality
void printVector(vector<int> a) {
	for (size_t i = 0; i < a.size(); i++) {
		cout << a[i] << " ";

	}
	cout << endl;
}

int main()
{
	ifstream infile;
	ofstream outfile;
	BSTree bst;
	int val;
	vector<int> A;
	infile.open("input.dat"); // open input file for reading

	// inserts all the values of an input file into a vector
	while (infile >> val)
	{
		A.push_back(val);
	}

	cout << "List of values in vector before sorting \n";
	printVector(A);
	//bubblesort sorts the vector
	bubbleSort(A);

	cout << "List of values in vector after sorting \n";
	printVector(A);

	/* Convert List to BST */
	cout << "After this should be the tree, before this was the vector \n";
	bst.sortedvectorToBST(A);
	bst.printLevelOrder();
	bst.GraphVizOut("bstoutput.txt");

	return 0;
}
