#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {FAILED,PASSED} TestResult;

void print_result( TestResult result );

// insert all integers in [x,y] to the tree.
AVLNodePtr insert_range( AVLNodePtr root, int x, int y );

// search for all integers in [x,y]. Return FAILED if at least one search has failed.
TestResult search_range( AVLNodePtr root, int x, int y );

// delete all integers in [x,y] from teh tree.
AVLNodePtr delete_range( AVLNodePtr root, int x, int y);

// Build a fibonacci AVL tree with consecutive integer keys that start from min_val.
AVLNodePtr construct_fibonacci_tree( int h, int min_val );

AVLNodePtr fibonacci_recursive_ctor( int h, int * S, int min_val );

// check if the tree has the AVL property in every node. If not, *result is assigned FAILED.
int avl_property_test( AVLNodePtr root, TestResult * result );

TestResult search_insert_test();

TestResult delete_test();

TestResult join_test();

TestResult split_test();

TestResult split_test2();

TestResult join_test2();

TestResult delete_test2();

int avl_property_test( AVLNodePtr root, TestResult * result ){
    int h_left,h_right;
    if( !root )
        return -1;
    h_left = avl_property_test( root->child[LEFT], result );
    h_right = avl_property_test( root->child[RIGHT], result );
    *result =  (_abs(h_left-h_right) > 1 ) ? FAILED:*result;
    return 1+_max(h_left,h_right);
}

AVLNodePtr insert_range( AVLNodePtr root, int x, int y ){
    for( int i=x; i<=y; i++ )
        root = avl_insert( root, i );
    return root;
}

TestResult search_range( AVLNodePtr root, int x, int y ){
    AVLNodePtr node=NULL;
    for( int i=x; i<=y; ++i ){
        node = avl_search( root, i );
        if( !(node && node->key==i) ){
            return FAILED;
        }
    }
    return PASSED;
}

AVLNodePtr delete_range( AVLNodePtr root, int x, int y){
    for( int i=x; i<=y; i++ )
        root = avl_delete( root, i );
    return root;
}

AVLNodePtr construct_fibonacci_tree( int h, int min_val ){
    int i;
    int S[50] = {-1};
    S[0] = 1;
    S[1] = 2;
    for( i = 2; i <= h; i++ ){
        S[i] = 1 + S[i-1] + S[i-2];
    }
    return fibonacci_recursive_ctor( h, S, min_val );
}


AVLNodePtr fibonacci_recursive_ctor( int h, int * S, int min_val ){
    AVLNodePtr root = NULL;
    if( h == 0 ){
        root = new_avl_node( min_val );
    }
    else if( h == 1 ){
        root = new_avl_node( min_val );
        root->child[RIGHT] = new_avl_node( min_val + 1 );
    }
    else{
        root = new_avl_node( min_val + S[h-2] );
        root->child[LEFT] = fibonacci_recursive_ctor( h-2, S, min_val );
        root->child[RIGHT] = fibonacci_recursive_ctor( h-1, S, min_val + S[h-2] + 1 );
    }
    root->height = h;
    return root;
}

void print_result( TestResult result ){
    if( result==PASSED )
        printf("PASSED.\n");
    else
        printf("FAILED.\n");
}

TestResult search_insert_test(){
    AVLNodePtr root = NULL;
    TestResult result = PASSED;
    root = insert_range( root, 1000000, 2000000 );
    result = search_range( root, 1000000, 2000000 );
    avl_property_test( root, &result );
    printf("INSERT + SEARCH TEST ");
    print_result( result );
    delete_avl_tree( root );
    return result;
}

TestResult delete_test(){
    int i;
    AVLNodePtr root = NULL;
    TestResult result = PASSED;
    root = insert_range( root, 1000000, 2000000 );
    root = delete_range( root, 1100000, 1500000 );
    result = search_range( root, 1000000, 1099999 );
    result = search_range( root, 1500001, 2000000 )?result:FAILED;
    for( i=1100000; i<=1500000; i++){
        if( avl_search(root,i) )
            result = FAILED;
    }
    avl_property_test( root, &result );
    printf("DELETE TEST ");
    print_result( result );
    delete_avl_tree( root );
    return result;
}

TestResult join_test(){
    AVLNodePtr t1=NULL,t2=NULL;
    AVLNodePtr root=NULL;
    TestResult result=PASSED;
    t1 = insert_range( t1, 2, 1000000 );
    t2 = insert_range( t2, 1100000, 1200000 );
    root = avl_join( t1, new_avl_node(1050000), t2 );
    root = avl_join( root, new_avl_node(2000000), NULL );
    root = avl_join( NULL, new_avl_node(1), root );
    result = search_range( root, 1, 1000000 );
    result = search_range( root, 1100000, 1200000 )?result:FAILED;
    avl_property_test(root,&result);
    printf("JOIN TEST ");
    print_result( result );
    delete_avl_tree( root );
    return result;
}

TestResult split_test(){
    int i=1;
    TestResult result=PASSED;
    AVLNodePtr trees_out[2] = {NULL,NULL};
    AVLNodePtr root=NULL,node1=NULL,node2=NULL;
    root = insert_range( root, 1, 1000000 );
    node1 = avl_split( root, 1, trees_out );
    root = trees_out[1];
    trees_out[0] = trees_out[1] = NULL;
    node2 = avl_split( root, 300000, trees_out );
    if( !(node2 && node2->key==300000) || !(node1 && node1->key==1) )
        result = FAILED;
    if( node1 )
        free(node1);
    if( node2 )
        free(node2);
    result = search_range( trees_out[0], 2, 299999 )?result:FAILED;
    result = search_range( trees_out[1], 300001, 1000000 )?result:FAILED;
    for( i=300000; i<=1000000; i++ ){
        if( avl_search(trees_out[0],i) )
            result = FAILED;
    }
    for( i=1; i<=300000; i++ ){
        if( avl_search(trees_out[1],i) )
            result = FAILED;
    }
    avl_property_test( trees_out[0], &result );
    avl_property_test( trees_out[1], &result );
    printf("SPLIT TEST ");
    print_result( result );
    delete_avl_tree( trees_out[0] );
    delete_avl_tree( trees_out[1] );
    return result;
}


TestResult delete_test2(){
    AVLNodePtr root = NULL;
    TestResult result=PASSED;
    root = construct_fibonacci_tree( 20, 1 );
    root = avl_delete( root, root->key );
    avl_property_test(root,&result);
    printf("DELETE TEST 2 ");
    print_result(result);
    delete_avl_tree( root );
    return result;
}


/* Join two fibonacci trees
 * one tree of height 10 and size 232,
 * together with a new node and another tree of height 15 and size 2583.
 * Overall, all the keys in range 1-2816 should be present.
 * */
TestResult join_test2(){
    AVLNodePtr root1 = NULL;
    AVLNodePtr root2 = NULL;
    AVLNodePtr joined = NULL;
    TestResult result=PASSED;
    root1 = construct_fibonacci_tree( 10, 1 );
    root2 = construct_fibonacci_tree( 15, 234 );
    joined = avl_join( root1, new_avl_node( 233 ), root2 );
    result = search_range( joined, 1, 2816 );
    avl_property_test( joined, &result );
    printf("JOIN TEST 2 ");
    print_result( result );
    delete_avl_tree( joined );
    return result;
}

/* Split a fibonacci tree of height 20 and size 28656.
 * The split occurs in 20000 so there should be two trees, one with all
 * keys in range [1,19999] and another tree with all keys in range [20001,28656]
 */
TestResult split_test2(){
    AVLNodePtr root = NULL, node = NULL;
    AVLNodePtr trees_out[2] = {NULL,NULL};
    TestResult result = PASSED;
    root = construct_fibonacci_tree( 20, 1 );
    node = avl_split( root , 20000, trees_out );
    if( node )
        free(node);
    result = search_range( trees_out[0], 1, 19999 );
    result = search_range( trees_out[1], 20001, 28656 ) ? result : FAILED;
    avl_property_test( trees_out[0], &result );
    avl_property_test( trees_out[1], &result );
    printf("SPLIT TEST 2 ");
    print_result( result );
    delete_avl_tree( trees_out[0] );
    delete_avl_tree( trees_out[1] );
    return result;
}

int main(){
    search_insert_test();
    delete_test();
    join_test();
    split_test();
    delete_test2();
    join_test2();
    split_test2();
    return 0;
}
