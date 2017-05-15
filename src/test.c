#include "rb_tree.h"
#include "utils.h"

#define MAX_NUM_NODE  (16*1024)

static val_t key_comp_func(ptr_t org_key, ptr_t new_key)
{
  return *(val_t*)org_key - *(val_t*)new_key;
}

int main()
{
  rb_tree_t *tree;
  val_t i;
  val_t *data;

  tree = mem_alloc(sizeof(*tree));
  if (!tree) {
    DEBUG_PRINT("[ERR]: tree ptr is %p\n", tree);
    goto done;
  }
  rb_tree_init(tree, key_comp_func);
  for (i = 0; i < MAX_NUM_NODE; i++) {
    data = mem_alloc(sizeof(*data));
    if (!data) {
      DEBUG_PRINT("[ERR]; data ptr is %p\n", data);
      goto done;
    }
    *data = rand() % MAX_NUM_NODE;
    /* Insert Node */
    if (rb_tree_insert(tree, data) == SUCCESS) {
      rb_tree_dump(tree);
      if (rb_tree_find(tree, data) == &tree->leaf) {
        printf("[ERROR]: Cannot found key %u\n", *data);
        exit(-1);
      }
    }
  }
  rb_tree_dump(tree);
  for (i = 0; i < 1024; i++) {
    if (rb_tree_find(tree, &i) == &tree->leaf) {
      printf("[INFO]: Cannot found key %u\n", i);
    }
  }
  mem_free(tree);
done:
  return 0;
}
