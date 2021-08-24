#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

/****
  Student1 name: ----- -----
  Student2 name: ----- -----

  Student1 ID: ----- -----
  Student2 ID: ----- -----
****/
int height(AVLNodePtr node){
    if(node==NULL){
        return 0;
    }
    return node->height;
}

AVLNodePtr rightRotate(AVLNodePtr node){
    AVLNodePtr x=node->child[0];
    AVLNodePtr t=x->child[1];
    x->child[1]=node;
    node->child[0]=t;
    node->height=_max(height(node->child[1]),height(node->child[0]))+1;
    x->height=_max(height(x->child[0]),height(x->child[1]))+1;
    return x;
}
AVLNodePtr leftRotate(AVLNodePtr node){
    AVLNodePtr x=node->child[1];
    AVLNodePtr t=x->child[0];
    x->child[0]=node;
    node->child[1]=t;
    node->height=_max(height(node->child[1]),height(node->child[0]))+1;
    x->height=_max(height(x->child[0]),height(x->child[1]))+1;
    return x;
}
AVLNodePtr balance(AVLNodePtr node){
    AVLNodePtr y;
    if(_abs(height(node->child[0])-height(node->child[1]))<=1){
        return node;
    }else if(height(node->child[0])>height(node->child[1])){
        y=node->child[0];
        if(height(y->child[0])<height(y->child[1])){
            leftRotate(y);
        }
        return rightRotate(node);
    }else{
        y=node->child[1];
        if(height(y->child[0])>height(y->child[1])){
            rightRotate(y);
        }
        return leftRotate(node);
    }
}

// Do not change this. You can use but do not touch.
int _max( int x, int y ){
    return (x < y) ? y : x;
}

// Do not change this. You can use but do not touch.
int _abs( int x ){
    return (x < 0) ? -x : x;
}

// Operations implementation

AVLNodePtr avl_search( AVLNodePtr root, int x ){
    if(root==NULL){
        return NULL;
    }
    if(root->key==x){
        return root;
    }
    if(root->key<x){
        return avl_search(root->child[1],x);
    }
    if(root->key>x){
        return avl_search(root->child[0],x);
    }
    return NULL; // Student code goes here. Feel free to remove this line.
}

AVLNodePtr avl_insert( AVLNodePtr root, int x ){
    if(root==NULL){
        root=new_avl_node(x);
        return root;
    }
    if(avl_search(root,x)!=NULL){
        return root;
    }
    if(root->key>x){
        root->child[0]=avl_insert(root->child[0],x);
    }else{
        root->child[1]=avl_insert(root->child[1],x);
    }
    root->height=_max(height(root->child[0]),height(root->child[1]))+1;
    root=balance(root);
    return root;

}

AVLNodePtr minValueNode(AVLNodePtr node)
{
    AVLNodePtr current = node;

    // find the leftmost leaf */
    while (current->child[0] != NULL)
        current = current->child[0];

    return current;
}
int getBalance(AVLNodePtr N)
{
    if (N == NULL)
        return 0;
    return height(N->child[0]) -
           height(N->child[1]);
}
AVLNodePtr avl_delete( AVLNodePtr root, int x ) {
    if (root == NULL)
        return root;

    //perform BST delete
    if ( x < root->key )
        root->child[0] = avl_delete(root->child[0], x);

    else if(x > root->key )
        root->child[1] = avl_delete(root->child[1], x);

    else
    {
        // node with only one child or no child
        if( (root->child[0] == NULL) ||
            (root->child[1]== NULL) )
        {
            AVLNodePtr temp = root->child[0] ?
                            root->child[0] :
                            root->child[1];

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp;
            free(temp);
        }
        else
        {
            AVLNodePtr temp = minValueNode(root->child[1]);

            root->key = temp->key;

            // Delete the inorder successor
            root->child[1] = avl_delete(root->child[1],
                                     temp->key);
        }
    }

    if (root == NULL)
        return root;

    // update depth
    root->height = 1 + _max(height(root->child[0]),
                          height(root->child[1]));

    // get balance factor
    int balance = getBalance(root);

    //rotate the tree if unbalanced

    // Left Left Case
    if (balance > 1 &&
        getBalance(root->child[0]) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 &&  getBalance(root->child[0]) < 0)  {
        root->child[0] = leftRotate(root->child[0]);
        return rightRotate(root);
    }
    // Right Right Case
    if (balance < -1 &&  getBalance(root->child[1]) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->child[1]) > 0)   {
        root->child[1] = rightRotate(root->child[1]);
        return leftRotate(root);
    }
    return root;

}


AVLNodePtr avl_join( AVLNodePtr root1, AVLNodePtr xnode, AVLNodePtr root2 )
{
    AVLNodePtr root;
    if(root1==NULL&&root2==NULL){
        return xnode;
    }
    if(root1==NULL){
        return avl_insert(root2,xnode->key);
    }
    if(root2==NULL){
        return avl_insert(root1,xnode->key);
    }
    if(_abs(height(root1)-height(root2))<=1){
        xnode->child[0]=root1;
        xnode->child[1]=root2;
        xnode->height=_max(height(root1),height(root2))+1;
        return xnode;
    }else if(height(root1)>height(root2)){
       root=avl_join(root1->child[1],xnode,root2);
       root1->child[1]=root;
       balance(root1);
        return root1;
    }else if(height(root2)>height(root1)){
        root=avl_join(root1,xnode,root2->child[0]);
        root2->child[0]=root;
        balance(root2);
        return root2;
    }
}

AVLNodePtr avl_split( AVLNodePtr root, int x, AVLNodePtr trees_out[2] ){
    AVLNodePtr out[2],r;
    if(root==NULL){
        trees_out[0]=NULL;
        trees_out[0]=NULL;
        return NULL;
    }
    if(root->key==x){
        trees_out[0]=root->child[0];
        trees_out[1]=root->child[1];
        return root;
    }
    if(root->key>x){
      r=avl_split(root->child[0],x,out);
        trees_out[0]=out[0];
        trees_out[1]=avl_join(out[1],root,root->child[1]);
        return r;
    }
    if(root->key<x){
        r=avl_split(root->child[1],x,out);
        trees_out[0]=avl_join(root->child[0],root,out[0]);
        trees_out[1]=out[1];
        return r;
    }
    return NULL; // Student code goes here. Feel free to remove this line.
}

AVLNodePtr new_avl_node( int x ){
    AVLNodePtr new_node=(AVLNodePtr)malloc(sizeof(*new_node));
    if(new_node!=NULL){
        new_node->key=x;
        new_node->child[0]=NULL;
        new_node->child[1]=NULL;
        new_node->height=1;
        return new_node;
    }
    return NULL; // Student code goes here. Feel free to remove this line.
}

void delete_avl_tree( AVLNodePtr root ){
    if(root!=NULL){
        delete_avl_tree(root->child[0]);
        delete_avl_tree(root->child[1]);
        free(root);
    }
    root=NULL;
}
