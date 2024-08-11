#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

#define ordered_set tree<int, null_type, less<int>,\
  rb_tree_tag, tree_order_statistics_node_update>

// strict less than is recommonded to avoid problems
// with equality
// order_of_key(T key):number of elements less than key
// find_by_order(int): gives the 0 based index elem