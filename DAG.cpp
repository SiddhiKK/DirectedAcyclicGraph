#include<stdlib.h>
#include<iostream>
using namespace std;

struct bin_tree
{
	char data;
	int label;
	struct bin_tree *right, *left;
};
typedef bin_tree node;

class dag
{
 private:
         int R[10];      /* R is stack for storing registers */
         int top;
         char *op;  /* op used for opcode name w.r.t. arithmetic operator e.g. ADD for + */ 
 public:
        void initializestack(node*);
	void insertnode(node**,char);    /*functions are for adding nodes to tree(DAG) */
	void insert(node**,char);	
	void leafLabel(node*,int);
	void intLabel(node*);
	void gencode(node*);
	void deltree(node*);
        void push(int);   //increment top by one and will insert element at top position
        void swap();  //swap the top and second top elements of Register stack R 
	int pop();  //remove and return topmost element of stack
	void operate(char);
	void print_inorder(node*);
};



void dag::initializestack(node *root)
{
 /* value of top = index of topmost element of stack R = label of Root of tree(DAG) - 1 */
    top=root->label - 1;
    /* Allocating Stack Registers */
    int temp=top;
    for(int i=0;i<=top;i++)
       {  R[i]=temp;
          temp--;
       }
}

void dag::insertnode(node **tree,char val)
{	node *temp = NULL;
	if(!(*tree))
	{
        	temp = (node *)malloc(sizeof(node));
        	temp->left = temp->right = NULL;
        	temp->data = val;
        	temp->label=-1;
        	*tree = temp;
    	}
}

void dag::insert(node **tree,char val)
{
    char l,r;
    int noc;
    insertnode(tree, val);
    cout << "\nEnter number of children of " << val <<" :";
    cin >> noc;
    if(noc==2)
    {
     cout << "\nEnter Left Child of " << val <<" :";
     cin >> l;
     insertnode(&(*tree)->left,l);
     cout << "\nEnter Right Child of " << val <<" :";
     cin >> r;
     insertnode(&(*tree)->right,r);
     insert(&(*tree)->left,l);
     insert(&(*tree)->right,r);
   }
}

void dag::leafLabel(node *tree,int val)
{
	if(tree->left != NULL && tree->right !=NULL)
	{
		leafLabel(tree->left,1);
		leafLabel(tree->right,0);
	}
	else
		tree->label=val;
}

void dag::intLabel(node *tree)
{
	if(tree->left->label==-1)
		intLabel(tree->left);
	else if(tree->right->label==-1)
		intLabel(tree->right);
	else
	{
		if(tree->left != NULL && tree->right !=NULL)
		{
			if(tree->left->label == tree->right->label)
				tree->label=(tree->left->label)+1;
			else
			{
				if(tree->left->label > tree->right->label)
					tree->label=tree->left->label;
				else
					tree->label=tree->right->label;
			}
		}
	}
}

void dag::print_inorder(node * tree)
{   if (tree)
    {
        print_inorder(tree->left);
        cout << tree->data <<" with Label "<< tree->label << "\n";
        print_inorder(tree->right);
    }
}

void dag::swap()
{	int temp;
	temp=R[0];
	R[0]=R[1];
	R[1]=temp;
}

int dag::pop()
{	int temp=R[top];
	top--;
	return temp;
}

void dag::push(int temp)
{	top++;
	R[top]=temp;
}

void dag::operate(char temp)
{	switch(temp)
	{
	case '+': op =(char *)"ADD"; break;
	case '-': op =(char *)"SUB"; break;
	case '*': op =(char *)"MUL"; break;
	case '/': op =(char *)"DIV"; break;
	}
}

void dag::gencode(node * tree)
{
	if(tree->left != NULL && tree->right != NULL)
	{
		if(tree->left->label == 1 && tree->right->label == 0)
		{
		 	cout << "MOV "<< tree->left->data << "," << "R[" << R[top] << "]\n";
		 	operate(tree->data);
		 	cout << op << " " << tree->right->data << ",R[" << R[top] << "]\n";
		}
		else if(tree->left->label >= 1 && tree->right->label == 0)
		{
		 	gencode(tree->left);
		 	operate(tree->data);
			cout << op << " " << tree->right->data << "R[" << R[top] << "]\n";
		}
		else if(tree->left->label < tree->right->label)
		{
			int temp;
			swap();
			gencode(tree->right);
			temp=pop();
			gencode(tree->left);
			push(temp);
			swap();
			operate(tree->data);
			cout << op << " " << "R[" << R[top-1] <<"],R[" << R[top] << "]\n";
		}
		else if(tree->left->label >= tree->right->label)
		{
			int temp;
			gencode(tree->left);
			temp=pop();
			gencode(tree->right);
			push(temp);
			operate(tree->data);
			cout << op << " " << "R[" << R[top-1] << "],R[" << R[top] <<"]\n";
		}
	}
	else if(tree->left == NULL && tree->right == NULL && tree->label == 1)
	{
		cout << "MOV " << tree->data << ",R[" << R[top] << "]\n";
	}
}

void dag::deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

int main()
{
    node *root=NULL,*temp;
    char val;
    int i;
    dag d;
    cout << "\nEnter root of tree:";
    cin >> val;
    d.insert(&root,val);
    d.leafLabel(root,1);
    while(root->label == -1)	   	
    {
     	d.intLabel(root);
    }
    d.initializestack(root);
    cout << "\nInorder Display:\n";
    d.print_inorder(root);
    cout << "\nAssembly Code:\n";
    d.gencode(root);
    d.deltree(root);
}
