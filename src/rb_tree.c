#include "rb_tree.h"
#include "utils.h"

static inline rb_tree_node_t*
init_node(rb_tree_t *tree,
          ptr_t key)
{
  rb_tree_node_t *node;
  uval_t size;

  size = sizeof(*node);
  node = mem_alloc(size);
  if (node) {
    node->key = key;
    node->left = &tree->leaf;
    node->right = &tree->leaf;
    node->parent = &tree->leaf;
    set_node_red(node);
  }
  return node;
}

static inline null_t
left_rotation(rb_tree_t* tree,
              rb_tree_node_t *node,
              rb_tree_node_t *leaf)
{
  rb_tree_node_t *right;

  right = node->right;
  node->right = right->left;
  if (right->left != leaf) {
      right->left->parent = node;
  }
  right->parent = node->parent;

  if (node == tree->root) {
    tree->root = right;
  }
  else if (node == node->parent->left) {
    node->parent->left = right;
  }
  else {
    node->parent->right = right;
  }
  right->left = node;
  node->parent = right;
}

static inline null_t
right_rotation(rb_tree_t* tree,
               rb_tree_node_t *node,
               rb_tree_node_t *leaf)
{
  rb_tree_node_t  *left;

  left = node->left;
  node->left = left->right;
  if (left->right != leaf) {
    left->right->parent = node;
  }
  left->parent = node->parent;

  if (node == tree->root) {
    tree->root = left;
  }
  else if (node == node->parent->right) {
    node->parent->right = left;
  }
  else {
    node->parent->left = left;
  }
  left->right = node;
  node->parent = left;
}

static inline null_t
tree_insert(rb_tree_t *tree,
            rb_tree_node_t *node)
{
  rb_tree_node_t *parent;
  rb_tree_node_t *leaf;

  parent = tree->root;
  leaf = &tree->leaf;
  while (parent != leaf) {
    if (tree->comp_func(parent->key, node->key) >= 0) {
      if (parent->left != leaf) {
        parent = parent->left;
      }
      else {
        parent->left = node;
        node->parent = parent;
        break;
      }
    }
    else {
      if (parent->right != leaf) {
        parent = parent->right;
      }
      else {
        parent->right = node;
        node->parent = parent;
        break;
      }
    }
  }
  if (parent == leaf) {
   tree->root = node;
   tree->root->parent = leaf;
  }
}

#define parent_is_grandpa_leftchild(node) \
  ((node)->parent == (node)->parent->parent->left)

static inline rb_tree_node_t*
get_uncle(rb_tree_node_t *node)
{
  if (parent_is_grandpa_leftchild(node)) {
    return node->parent->parent->right;
  }
  else {
    return node->parent->parent->left;
  }
}

static inline null_t
tree_insert_fixup(rb_tree_t *tree,
                  rb_tree_node_t *node)
{
  rb_tree_node_t *uncle;
  rb_tree_node_t *leaf;

  while (is_node_red(node->parent)) {
    leaf = &tree->leaf;
    uncle = get_uncle(node);
    if (is_node_red(uncle)) {
      /* Case 1 */
      set_node_black(node->parent);
      set_node_black(uncle);
      set_node_red(node->parent->parent);
      node = node->parent->parent;
    }
    else if (parent_is_grandpa_leftchild(node)) {
      /* Left Subtree */
      if (node == node->parent->right) {
        /* Case 2 */
        node = node->parent;
        left_rotation(tree, node, leaf);
      }
      /* Case 3 */
      set_node_black(node->parent);
      set_node_red(node->parent->parent);
      right_rotation(tree, node->parent->parent, leaf);
    }
    else {
      /* Right Subtree */
      if (node == node->parent->left) {
        /* Case 2 */
        node = node->parent;
        right_rotation(tree, node, leaf);
      }
      /* Case 3 */
      set_node_black(node->parent);
      set_node_red(node->parent->parent);
      left_rotation(tree, node->parent->parent, leaf);
    }
  }
  set_node_black(tree->root);
}

static inline null_t
tree_delete(rb_tree_t *tree,
            rb_tree_node_t *node)
{

}

static inline null_t
tree_delete_fixup(rb_tree_t *tree,
                  rb_tree_node_t *node)
{

}

val_t
rb_tree_insert(rb_tree_t *tree,
               ptr_t key)
{
  rb_tree_node_t *node;

  if (!tree || !key) {
    return ERR_PARAMS;
  }
  node = init_node(tree, key);
  if (!node) {
    return ERR_NO_MEM;
  }
  tree_insert(tree, node);
  tree_insert_fixup(tree, node);
  return SUCCESS;
}

val_t
rb_tree_delete(rb_tree_t *tree,
               ptr_t key)
{
  rb_tree_node_t* node;
  if (!tree || !key) {
    return ERR_PARAMS;
  }
  node = rb_tree_find(tree, key);
  if (node != nullptr && node != &tree->leaf) {
    tree_delete(tree, node);
    tree_delete_fixup(tree, node);
  }
  return SUCCESS;
}

rb_tree_node_t*
rb_tree_find(rb_tree_t *tree,
             ptr_t key)
{
  rb_tree_node_t *node;
  val_t comp_val;

  if (!tree || !tree->root || !key) {
    return nullptr;
  }

  node = tree->root;
  while (node != &tree->leaf) {
    comp_val = tree->comp_func(node->key, key);
    if (comp_val < 0) {
      node = node->right;
    }
    else if (comp_val > 0) {
      node = node->left;
    }
    else {
      break;
    }
  }
  return node;
}
