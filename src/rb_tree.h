#ifndef RB_TREE_H
#define RB_TREE_H

#include "type.h"

typedef struct rb_tree_node_s rb_tree_node_t;
typedef struct rb_tree_s rb_tree_t;
typedef val_t (*key_comp_func_t)(ptr_t org_key, ptr_t new_key);

struct rb_tree_node_s {
  ptr_t key;
  rb_tree_node_t *left;
  rb_tree_node_t *right;
  rb_tree_node_t *parent;
  val_t color;
};

struct rb_tree_s {
  rb_tree_node_t *root;
  rb_tree_node_t leaf;
  key_comp_func_t comp_func;
};

#define RB_TREE_NODE_BLACK    (0xdead)
#define RB_TREE_NODE_RED      (0xbeef)

#define set_node_black(node)      ((node)->color = RB_TREE_NODE_BLACK)
#define set_node_red(node)        ((node)->color = RB_TREE_NODE_RED)
#define is_node_black(node)       ((node)->color == RB_TREE_NODE_BLACK)
#define is_node_red(node)         ((node)->color == RB_TREE_NODE_RED)

#define rb_tree_init(tree, func)   do { \
  set_node_black(&(tree)->leaf);        \
  (tree)->root = &(tree)->leaf;         \
  (tree)->comp_func = (func);           \
} while(0)

val_t rb_tree_insert(rb_tree_t *tree, ptr_t key);
val_t rb_tree_delete(rb_tree_t *tree, ptr_t key);
rb_tree_node_t* rb_tree_find(rb_tree_t *tree, ptr_t key);
null_t rb_tree_dump(rb_tree_t *tree);

#endif /* RB_TREE_H */
