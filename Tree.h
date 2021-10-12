#include <string>
using namespace std;
class node;
class Tree
{
private:
    node* root;
    string Infix,Postfix;
    node* CreateRPNTree();
    string InfixToPostfix();
public:
    Tree();
    Tree(string str);
    void SetInfix(string input);
    string GetInfix();
    node* GetTreeRoot();
    void insertStr(string str);
    friend double ProcTree(Tree tree);


};


class node
{
private:
    string val;
    node* left,*right;
    friend class Tree;
public:
    node(string str)
    {
        val=str;
        left=right=NULL;
    }
    friend double resTree(node* root);
};


