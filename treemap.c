#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
  if (new == NULL)
    return NULL;

  new->lower_than = lower_than;
  return new;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) 
{
  tree->current = tree -> root;
  TreeNode* padre = NULL;

  TreeNode *new = createTreeNode(key, value);

  if(tree->current == NULL)
  {
    tree->current = new;
    return;
  }

  while(tree->current != NULL)
  {
    padre = tree->current;
    if(is_equal(tree, tree->current->pair->key , key))
    {
      return;
    }
    else if(tree->lower_than(key, tree->current->pair->key ))
    {
      tree-> current = tree->current->left;
    }
    else
    {
      tree->current = tree->current->right;
    }
  }

  new->parent = padre;
  
  if (tree->lower_than(key, padre->pair->key ))
  {
    padre->left = new;
  }
  else
  {
    padre->right = new;
  }

  tree->current = new;

  
}

TreeNode * minimum(TreeNode * x){

  while(x->left != NULL)
  {
    x = x -> left;
  }
  
  return x;
}

void removeNode(TreeMap *tree, TreeNode *node) {
  if (tree == NULL || node == NULL) return; // verificar si el árbol o el nodo son nulos

  // encontrar el nodo que se desea eliminar
  TreeNode *parent = NULL;
  TreeNode *current = tree->root;

  while (current != NULL && current != node) {
    parent = current;
    if (tree->lower_than(node->pair->key, current->pair->key)) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  if (current == NULL) return; // el nodo no se encontró en el árbol

  // el nodo tiene dos hijos
  if (current->left != NULL && current->right != NULL) {
    TreeNode *successor = current->right;
    while (successor->left != NULL) {
      successor = successor->left;
    }
    current->pair = successor->pair;
    current = successor;
  }

  // el nodo tiene cero o un hijo
  TreeNode *child = NULL;
  if (current->left != NULL) {
    child = current->left;
  } else if (current->right != NULL) {
    child = current->right;
  }

  // eliminar el nodo del árbol
  if (parent == NULL) {
    tree->root = child;
  } else if (current == parent->left) {
    parent->left = child;
  } else {
    parent->right = child;
  }
  
  if (child != NULL) {
    child->parent = parent;
  }
  
  free(current);
}


void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) 
{ 
  tree->current = tree->root;
  if (tree->root == NULL) return NULL;


  while (tree->current != NULL)
  {
    if (is_equal(tree, tree->current->pair->key, key))
    {
      return (tree->current->pair);
    }
    if (tree->lower_than(tree->current->pair->key, key))
    {
      tree->current = tree->current->right; 
    }
    else
    {
      tree->current = tree->current->left;
    } 
  }
  
  return NULL; 
}

Pair *upperBound(TreeMap *tree, void *key) 
{
  tree->current = tree->root;
  TreeNode *ub_node = NULL;
  
  if(tree->root == NULL) return NULL;
  
  while(tree->current != NULL)
  {
    if(is_equal(tree, tree->current->pair->key , key))
    {
      return (tree->current->pair);
    }
    if(tree->lower_than(key, tree->current->pair->key))
    {
      ub_node = tree->current;
      tree-> current = tree->current->left;
    }
    else
    {
      tree->current = tree->current->right;
    }
  }

  if(ub_node != NULL){
    return ub_node->pair;
  }
  else {
    return NULL;
  }

}

Pair *firstTreeMap(TreeMap *tree) 
{
  if (tree->root == NULL) return NULL;

  TreeNode *node = tree->root; 
  while(node->left != NULL)
  {
    node = node->left;
  }
  tree->current = node;
  return node->pair;
}

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
