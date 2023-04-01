# Treap (Cartesian tree)

A detailed tutorial can be found in the following [link](https://cp-algorithms.com/data_structures/treap.html). 

In this repository only implemntation details are described.

To test your implementaion of the treap you can check the following [mashup](https://codeforces.com/contestInvitation/26bc7df1712fe6c7c50a7220dbb875619f541b5a) in codeforces.

## Treap basig functions

__NOTE :__ the full code can be found inside `main.cpp` file. 

- ## Split function

    This function takes a pointer to the node that want to split, and splitting point. 

    The function will reurn to nodes (they can be seen as two seperate treaps). The first node contains $idx$ element and the second node contains the rest.

    The idea of split function is to do the following recursively:
        
    - if the left child of the current node contains more than $idx$ element then our split will occur in that part. So, set the left child of the current node to be the result of cutting the left node with $idx$ element.

    - otherwise, we need to keep the left side as it is and split the right size. However, since we kept all the left child and the root, we have to decrease $idx$ when going to the right child of the current node.

    - return the result of the split and the current node.
    
    The code will look loke the following:
    
 ```cpp
    pair<item* , item*> split(item *&node, int idx)
    {
        if (node == nullptr)
        {
            return {nullptr, nullptr};
        }
        if (size(node->left) >= idx)
        {
            pair<item *, item *> ret = split(node->left, idx);
            node->left = ret.second;
            node->sz = 1 + size(node->left) + size(node->right);
            return {ret.first, node};
        }
        else
        {
            pair<item *, item *> ret = split(node->right, idx - size(node->left) - 1);
            node->right = ret.first;
            node->sz = 1 + size(node->left) + size(node->right);
            return {node, ret.second};
        }
    }
 ```

- ##  merge function

    The idea of this function is to merge two nodes into one new node, so basically the reverse of split function.

    The implemntation of the function is based on the following recursive algorithm:

    - if the priority of the first node is higher than the priority of the second node, then the first node must be a parent to the second node. Hence, set the right child of the first node to be the second node.

    - otherwise, the second node is the parent of the first node, and set the first node to be the left child of the second node.

  ```cpp
    item *merge(item *L, item *R)
    {
        if (L == nullptr)
            return R;
        if (R == nullptr)
            return L;
        if (L->priority > R->priority)
        {
            L->right = merge(L->right, R);
            L->sz = 1 + size(L->left) + size(L->right);
            return L;
        }
        else
        {
            R->left = merge(L, R->left);
            R->sz = 1 + size(R->left) + size(R->right);
            return R;
        }
    }
  ```

Split and merge functions provide everything we need for treaps. The following sections give some cases of using these functions:

- ## reverse segment

    __NOTE :__ the following code require some modifications to the split and merge functions, refer to `main.cpp` for the full code.

    Moreover, we nned to add propagating mechanism to our treap. To get more familiar with propagating you can check segment tree with lazy propagation. The propagation for split will look like the following:

  ```cpp
    void push(item *&parent, item *&node,int type)
    {
        if (node == nullptr)
            return;
        node->rev ^= parent->rev;
    }
    void prop(item *&node)
    {
        if (node == nullptr)
            return;
        if (node->rev == 1)
        {
            swap(node->left, node->right);
            push(node, node->left,3), push(node, node->right,3);
            node->rev = 0;
        }
    }
  ```

    To do any operation with a segement in the treap, we can simply cut the treap to isolate this segment, and do any operation we want with this segment. Examples of such operations:

    - add value to segment

    - assign all elements in this segment to a value

    - reverse segment

    - cylce shift 

    All of those operators are specified inside `main.cpp`.
    
    To have a better idea of such operators we can take a look at reversing a segment code, the code is as simple as the following:

  ```cpp
    void reverseSegment(int l, int r)
    {
        pair<item* , item*> part1 = split(root, l);
        pair<item* , item*> part2 = split(part1.second, r - l + 1);
        part2.first->rev ^= 1;
        root = merge(part1.first, merge(part2.first, part2.second));
    }
  ```

## Practice problems

- [Second thread gym problems](https://codeforces.com/gym/102787)

- [Ada and Harvest](https://www.spoj.com/problems/ADACROP/)

- [Ghost Town](https://www.spoj.com/problems/COUNT1IT/)

- [Arrangement Validity](https://www.spoj.com/problems/IITWPC4D/)

- [Yet Another Array Queries Problem](https://codeforces.com/contest/863/problem/D)

Solution for most of those problems is provided inside `pracitice-problems` folder