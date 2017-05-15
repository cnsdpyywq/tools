#include "rb_tree.h"
#include "utils.h"

typedef enum {
  IS_RB_TREE = 0,
  TREE_PARAMS,
  ROOT_NOT_BLACK,
  LEAF_NOT_BLACK,
  RED_PARENT_HAS_RED_CHILD,
  BLACK_HEIGHT_NOT_EQUAL
} is_rbtree_t;

static is_rbtree_t g_is_rbtree = IS_RB_TREE;

static inline uval_t
rb_tree_height(rb_tree_node_t* node,
               rb_tree_node_t* leaf)
{
  uval_t left_height;
  uval_t right_height;

  if (node == leaf) {
    return 0;
  }
  left_height = rb_tree_height(node->left, leaf) + 1;
  right_height = rb_tree_height(node->right, leaf) + 1;
  return left_height > right_height ? left_height : right_height;
}

static inline null_t
_rb_tree_check_color(rb_tree_node_t* root,
                     rb_tree_node_t* leaf)
{
  if (root == leaf) {
    if (is_node_red(root)) {
      g_is_rbtree = LEAF_NOT_BLACK;
    }
    return;
  }
  if (is_node_red(root)) {
    if (is_node_red(root->left) ||
        is_node_red(root->right)) {
      g_is_rbtree = RED_PARENT_HAS_RED_CHILD;
    }
  }
  _rb_tree_check_color(root->left, leaf);
  _rb_tree_check_color(root->right, leaf);
}

static inline uval_t
_rb_tree_bh(rb_tree_node_t* root,
                  rb_tree_node_t* leaf)
{
  uval_t left_bh;
  uval_t right_bh;
  uval_t bh = 0;

  if (root == leaf) {
    return 0;
  }
  if (is_node_black(root)) {
    bh = 1;
  }
  left_bh = _rb_tree_bh(root->left, leaf) + bh;
  right_bh = _rb_tree_bh(root->right, leaf) +bh;
  if (left_bh != right_bh) {
    g_is_rbtree = BLACK_HEIGHT_NOT_EQUAL;
  }
  return left_bh;
}

static inline is_rbtree_t
_rb_tree_check(rb_tree_node_t* root,
               rb_tree_node_t* leaf)
{
  uval_t bh;

  _rb_tree_check_color(root, leaf);
  if (g_is_rbtree != IS_RB_TREE) {
    return g_is_rbtree;
  }
  bh = _rb_tree_bh(root, leaf);
  return g_is_rbtree;
}

static inline is_rbtree_t
rb_tree_check(rb_tree_t* tree)
{
  g_is_rbtree = IS_RB_TREE;
  if (!tree || !tree->root) {
    return TREE_PARAMS;
  }
  else if (is_node_red(tree->root)) {
    return ROOT_NOT_BLACK;
  }
  return _rb_tree_check(tree->root, &tree->leaf);
}

static inline uval_t
get_rb_tree_nodes(rb_tree_node_t* node,
                  rb_tree_node_t* leaf)
{
  uval_t num_nodes = 0;

  if (node == leaf) {
    return 0;
  }
  else {
    num_nodes += get_rb_tree_nodes(node->left, leaf);
    num_nodes += get_rb_tree_nodes(node->right, leaf);
    num_nodes++;
  }
  return num_nodes;
}

null_t rb_tree_dump(rb_tree_t *tree)
{
  uval_t height;
  uval_t num_nodes;
  is_rbtree_t ret;

  if (!tree || !tree->root) {
    return;
  }

  ret = rb_tree_check(tree);
  if (ret == IS_RB_TREE) {
    num_nodes = get_rb_tree_nodes(tree->root, &tree->leaf);
    height = rb_tree_height(tree->root, &tree->leaf);
  }
  else {
    DEBUG_PRINT("[ERR]: Not a valid RB_TREE\n");
  }
}
