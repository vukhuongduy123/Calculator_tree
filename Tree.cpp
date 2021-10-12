#include "Tree.h"
#include <stack>
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;
Tree::Tree(string str)
{
    Infix=str;
}
Tree::Tree()
{}


void Tree::SetInfix(string input)
{
    Infix=input;

}
string Tree::GetInfix()
{
    return Infix;
}

node* Tree::GetTreeRoot()
{
    return root;
}

void Tree::insertStr(string str)
{
    Infix+=str;
}

bool is_number(string str_num) {
    int i=0;
    int str_len = str_num.size();
    while (i<str_len && str_num[i]==' ')
        i++;
    if (i<str_len && (str_num[i]=='+' || str_num[i]=='-'))
        i++;
    int digits=0, dots=0;
    while (i<str_len && ((str_num[i]>='0' && str_num[i]<='9')||(str_num[i]=='.'))) {
        if (str_num[i]>='0' && str_num[i]<='9')
            digits++;
        else if (str_num[i] == '.')
            dots++;
        i++;
    }
    if (digits == 0 || dots>1)
        return false;
    if (i<str_len && str_num[i]=='e') {
        if (++i<str_len && (str_num[i]=='+' || str_num[i]=='-'))
            i++;
        if (i==str_len || (str_num[i]<'0' || str_num[i]>'9'))
            return false;
        while (i<str_len && (str_num[i]>='0' && str_num[i]<='9'))
            i++;
    }
    while (i<str_len && str_num[i]==' ')  //optional trailing spaces
        i++;
    return (i==str_len); //if at end of string then success
}


int getOrder(string c)
{
    if(c=="!")
        return 6;
    if(c=="abs")
        return 5;
    if(c=="^"||c=="root"||c=="log")
        return 4;
    if(c=="*"||c=="/")
        return 3;
    if(c=="+"||c=="-")
        return 2;
    if(c=="(")
        return 1;
    return -1;
}

string Tree::InfixToPostfix()
{
    stack<string> s;
    string temp,prev;
    istringstream ss(Infix);
    while(ss>>temp)
    {
        if(temp=="-")
        {
            if(!is_number(prev)&&(prev!=")"&&prev!=""))
                Postfix+="0 ";
        }
        if(is_number(temp))
            Postfix+=temp+" ";
        else if (temp=="(")
            s.push(temp);
        else if(temp==")")
        {
            while(!s.empty()&&s.top()!="(")
            {
                Postfix+=s.top()+" ";
                s.pop();
            }
            s.pop();
        }
        else if( temp=="*"||temp=="/"||temp=="+"||temp=="-"||temp=="^"||temp=="root"||temp=="abs"||temp=="log"||temp=="!")
        {
            while(!s.empty()&&getOrder(temp)<=getOrder(s.top()))
            {
                Postfix+=s.top()+" ";
                s.pop();
            }
            s.push(temp);
        }
        prev=temp;
    }
    while(!s.empty())
    {
        Postfix+=s.top()+" ";
        s.pop();
    }
    return Postfix;
}

node* Tree::CreateRPNTree()
{
    stack<node*> s;
    istringstream ss(Postfix);
    string temp;
    while(ss>>temp)
    {
        node* pNode=new node(temp);
        if(temp=="+"||temp=="-"||temp=="*"||temp=="/"||temp=="^"||temp=="root"||temp=="log")
        {
            if(!s.empty())
            {
                pNode->right=s.top();
                s.pop();
            }
            if(!s.empty())
            {
                pNode->left=s.top();
                s.pop();
            }
        }
        else if(temp=="abs"||temp=="!")
        {
            pNode->right=s.top();
            s.pop();
        }
        s.push(pNode);
    }
    root=s.top();
    return root;
}
double factorial(double n)
{
    double res = 1;
    for (int i = 2; i <= n; i++)
        res *= i;
    return res;
}

double cal(string op,double a,double b)
{
    if(op=="!")
        return factorial(b);
    if(op=="log")
        return log(b)/log(a);
    if(op=="root")
        return pow(b,1/a);
    if(op=="abs")
        return abs(b);
    if(op=="+")
        return a+b;
    if(op=="-")
        return a-b;
    if(op=="*")
        return a*b;
    if(op=="/")
        return a/b;
    if(op=="^")
        return pow(a,b);
    return 0;
}
double resTree(node* root)
{
    if(!root->left&&!root->right)
        return stod(root->val);
    double left,right;
    if(root->left)
        left=resTree(root->left);
    if(root->right)
        right=resTree(root->right);

    return cal(root->val,left,right);
}

double ProcTree(Tree tree)
{
    tree.InfixToPostfix();
    tree.CreateRPNTree();
    return resTree(tree.root);
}


