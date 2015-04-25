#include<iostream>
#include<fstream>
#include <vector>
#include <queue>


using namespace std;

struct tree_node //to represent binary trees
{
	int leftchild;
	int rightchild;
	int parent;
};

struct symbol_info//record the frequency of each character
{
	char symbol;
	int frequency;
	int leaf;


};
bool cmpAlphabet(const symbol_info *x, const symbol_info *y)//overload < for comparision 
{
	return x->frequency <y->frequency;
}


struct forest_roots//a Priority Queue of structs that represent the trees themselves
{
	int weight;
	int root;

	bool  operator<(const  forest_roots* x)  const
	{
		return  weight < x->weight;
	}
	bool  operator>(const  forest_roots* x)  const
	{
		return  weight > x->weight;
	}
};
struct cmpForest// overload < or > for comparision 
{

	int weight;
	int root;
	bool operator()(const forest_roots *x, const forest_roots *y)
	{
		return  x->weight>y->weight;
	}

};



vector<tree_node*> Tree;
priority_queue<forest_roots*, vector<forest_roots*>, cmpForest> Forest;
vector<symbol_info*> Alphabet;
int lastnode;

void Huffman()//create Huffman Tree. Using Tree array, array index records roots and leafs
{

	tree_node* T = new tree_node();
	forest_roots *fr = new forest_roots();
	Tree.push_back(T);

	Tree[lastnode]->leftchild = Forest.top()->root;
	fr->weight = Forest.top()->weight;
	Tree[Forest.top()->root]->parent = lastnode;

	Forest.pop();

	Tree[lastnode]->rightchild = Forest.top()->root;
	fr->weight += Forest.top()->weight;
	Tree[Forest.top()->root]->parent = lastnode;
	Forest.pop();

	Tree[lastnode]->parent = 0;
	fr->root = lastnode;
	Forest.push(fr);
}

vector<size_t> *encodenode;
void encode()// start encoding every character
{
	encodenode = new vector<size_t>[Alphabet.size()];

	int start = 1;
	int locate;
	int currentnode;
	for (size_t i = 0; i < Alphabet.size(); i++)
	{
		locate = Tree[Alphabet[i]->leaf]->parent;
		currentnode = Alphabet[i]->leaf;
		while (currentnode != 0)
		{
			if (Tree[locate]->leftchild == currentnode)
				encodenode[i].push_back(0);//leftchild


			if (Tree[locate]->rightchild == currentnode)
				encodenode[i].push_back(1);//rightchild

			currentnode = locate;
			locate = Tree[locate]->parent;
		}
	}
}

int main()
{

	int size = 0;
	bool frequencyAdd = false;
	char input;
	ifstream ifs("infile.dat");
	if (!ifs)
	{
		cout << "Unable to open the file! The file maybe in the wrong location(wrong path), or doesn't exist. " << endl;
	}

	ifs >> input;
	while (!ifs.eof())
	{
		if (!((input >= 65 && input <= 122) || (input >= 48 && input <= 57)))//Ignore all blanks, all punctuation marks, all special symbols
			ifs >> input;
		else
		{
			for (size_t i = 0; i<Alphabet.size(); i++)
			{

				if (Alphabet[i]->symbol == input)
				{
					Alphabet[i]->frequency++;
					frequencyAdd = true;
				}
			}
			if (frequencyAdd == false)
			{
				size++;
				symbol_info *si = new symbol_info();
				si->frequency = 1;
				si->symbol = input;
				Alphabet.push_back(si);

			}
			frequencyAdd = false;
			ifs >> input;
		}
	}

	ifs.close();
	int leaf = 1;
	for (int i = 0; i<size; i++)
		Alphabet[i]->leaf = leaf++;
	for (size_t i = 0; i<Alphabet.size(); i++)
	{
		forest_roots *fr = new forest_roots();
		fr->root = Alphabet[i]->leaf;
		fr->weight = Alphabet[i]->frequency;
		Forest.push(fr);

		tree_node *T = new tree_node();
		Tree.push_back(T);//store all the nodes into tree

	}
	tree_node *R = new tree_node();/*because the Tree array index starting from 0,
								   I store all the nodes in the tree array starting from 1,so I just create one more "tree_node" in the tree array*/
	Tree.push_back(R);
	lastnode = Forest.size();



	if (Forest.size() == 0)
	{
		cout << "No any character in the file";

	}
	if (Forest.size() == 1)//when the file contains only one character, the Huffman tree has not left and right child, so push code 0.
	{
		encodenode = new vector<size_t>[Alphabet.size()];
		encodenode[0].push_back(0);
	}
	if (Forest.size()>1)
	{
		while (Forest.size()>1)
		{
			lastnode++;
			Huffman();	//create Huffman Tree
		}
		encode();// encode Huffman Tree
	}

	if (Forest.size() >= 1)
	{
		FILE *ofs;
		ofs = fopen("outfile.dat", "w");//for the output
		int length;
		int maxc = encodenode[0].size();
		int countmaxc = 0;
		int copymax;
		int fillspace;
		for (size_t i = 0; i<Alphabet.size(); i++)
		{
			countmaxc = encodenode[i].size();
			if (countmaxc>maxc)
			{
				maxc = countmaxc;
			}
		}

		cout << "Symbol" << "    " << "Frequency       " << "    " << "Huffman Code                    " << "    " << "Length of code" << endl;
		fprintf(ofs, "Symbol    Frequency           Huffman Code                        Length of code");
		fprintf(ofs, "\n");

		countmaxc = 0;
		for (size_t i = 0; i<Alphabet.size(); i++)
		{
			printf("%-6c", Alphabet[i]->symbol);
			fprintf_s(ofs, "%-6c", Alphabet[i]->symbol);

			cout << "    ";
			fprintf_s(ofs, "    ");

			printf("%-16d", Alphabet[i]->frequency);
			fprintf_s(ofs, "%-16d", Alphabet[i]->frequency);

			cout << "    ";
			fprintf_s(ofs, "    ");

			countmaxc = encodenode[i].size();
			length = encodenode[i].size();

			while (!encodenode[i].empty())
			{
				cout << encodenode[i][encodenode[i].size() - 1];
				fprintf(ofs, "%d", encodenode[i][encodenode[i].size() - 1]);
				encodenode[i].pop_back();
			}


			fillspace = 32 - countmaxc;
			while (fillspace>0)
			{
				cout << " ";
				fprintf_s(ofs, " ");
				fillspace--;
			}

			cout << "    ";
			fprintf_s(ofs, "    ");

			cout << length;
			fprintf_s(ofs, "%d", length);

			cout << endl;
			fprintf_s(ofs, "\n");
		}

		fclose(ofs);
	}
	return 1;
}
