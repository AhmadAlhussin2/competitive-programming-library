# Aho-Corasick Algorithm

## Construct a trie

- ### Create node structure

  ```cpp
   struct node {
       int to[26], suffixLink, counter, parentNode;
   };
  ```

  #### struct description

  - _`array to[26]`_ the number 26 is the size of the English alphabet, change it if there are more charachters.
    The _`i-th`_ element of the array describes the link between the current node and the node with label _`i`_.
  - _`suffix link`_ an integer that contains the index of a node with the longest propper suffix that ends in the current node.
    The idia of finding this suffix link is close to finding the suffix link in KMP algorithm.
  - _`counter`_ an integer contains the number of strings that ends in the current node.
  - _`parent node`_ an intger that links the current node to the parent node. _For the root `0` the link is also `0`_

- ### Adding a string

  ```cpp
  void addString(string str) {
      int index = 0;
      for (char c: str) {
          int nextNode = c - 'a';
          if (!trie[index].to[nextNode])
          {
              trie.push_back(node());
              trie[index].to[nextNode] = ++ptr;
          }
          index = trie[index].to[nextNode];
      }
      trie[index].counter += 1;
  }
  ```

  #### adding string

  - Start at the root node with index _`0`_
  - Iterate through the string character by character
  - For each character _`c`_:
    - if there is no link from the current node to a new node with label _`c`_ add a new node
    - If you added a new node link it to the previous node with label _`c`_
    - After adding the new node, move to this newly added node
    - Move to the new character
  - After iterating through all the characters mark a new string in the last node

- ### Building automaton

  ```cpp
  void build() {
      queue<int> automatonQueue;
      for (int i = 0; i < 26; i++) {
          if (trie[0].to[i])automatonQueue.push(trie[0].to[i]);
      }
      while (!automatonQueue.empty()) {
          auto node = automatonQueue.front();
          automatonQueue.pop();
          for (int i = 0; i < 26; i++) {
              if (trie[node].to[i])
              {
                  int nxt = trie[node].to[i];
                  automatonQueue.push(nxt);
                  auto prevSuffixLink = trie[node].suffixLink;
                  while (prevSuffixLink && !trie[prevSuffixLink].to[i])
                  {
                      prevSuffixLink = trie[prevSuffixLink].suffixLink;
                  }
                  prevSuffixLink = trie[prevSuffixLink].to[i];
                  trie[nxt].suffixLink = prevSuffixLink;
                  trie[nxt].parentNode = node;
              }
          }
      }
  }
  ```

  #### Building steps

  - Start a BFS from the first layer **excluding** the root node
  - During the BFS:
    - pick the node in the front of the queue
    - explore all the outgoing edges
    - For each edge, go to the deeper node _(in other words extend the suffix)_
    - Now for finding the suffix link, follow the suffix link of the parent node until you reach the root, or find a proper suffix
    - update the suffix link, and the parent node for the node

## example problems

- [Elastic search](https://codeforces.com/gym/103107/problem/E)
- [Frequency of String](https://codeforces.com/problemset/problem/963/D)
- [x-prime Substrings](https://codeforces.com/problemset/problem/1400/F)
- [Prefix Lookup](https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2637)
- [SMS](https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2112)
- [I love Strings](https://onlinejudge.org/index.php?option=onlinejudge&page=show_problem&problem=1620)
