#include <glib.h>

int main(void) {
    GHashTable* ht = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(ht, "foo", "bar");
    //    g_hash_table_destroy(ht); // leak through glib2
    g_malloc(100);  // direct leak
    return 0;
}