#include<bits/stdc++.h>

using namespace std;

/**
* Aho-Corasick algorithm
*
* This code solves 'Elastic Search' problem which can be found in:
* https://codeforces.com/gym/103107/problem/E
*
* A short description for the problem:
* Given n string, print the maximum possible length of a multiset, where each string is substring of the next one
* an example of such set is: ["a","abc","abcd",...]
*/
struct AHO {
    // trie data structure to implement aho-corasick algorithm
    struct node {
        int to[26], suffixLink, counter, parentNode, value;
    };
    vector<node> trie;
    int ptr;

    /**
    * Constructor
    *
    * initiate trie data structure
    */
    AHO() {
        trie.push_back(node());
    }

    /**
    * Add a string to the trie
    *
    * @param str is the input string, which is going to be stored in the trie
    */
    void addString(string str) {
        int index = 0; // index of the current node, initially we start at the root node `0`
        for (char c: str) {
            int nextNode = c - 'a';
            if (!trie[index].to[nextNode]) // if the trie does not recognize the next char in the input string
            {
                trie.push_back(node()); // add a new vertex
                trie[index].to[nextNode] = ++ptr; // make a new pointer from the current index to the new added vertex
            }
            index = trie[index].to[nextNode];

        }
        trie[index].counter += 1; // indicates that a new string finished at `index`
    }

    /**
    * Build function to construct the automaton out of all added strings
    *
    * this function works using BFS algorithm
    * we use BFS because it goes layer by layer, so each time we know that for any vertex `v`
    * we have already computed its suffix link
    * caution: call this function after adding all strings
    */
    void build() {
        queue<int> automatonQueue;
        for (int i = 0; i < 26; i++) {
            if (trie[0].to[i])automatonQueue.push(trie[0].to[i]);
        }
        while (!automatonQueue.empty()) {
            auto node = automatonQueue.front();
            automatonQueue.pop();
            for (int i = 0; i < 26; i++) {
                if (trie[node].to[i]) // if the i-th character exists, compute its suffix link
                {
                    int nxt = trie[node].to[i];
                    automatonQueue.push(nxt);
                    auto prevSuffixLink = trie[node].suffixLink;
                    while (prevSuffixLink && !trie[prevSuffixLink].to[i]) // find the suffix link, like in KMP algorithm
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

    /**
    * A special function that solves the described `Elastic Search` algorithm
    *
    * The function follows the same path as building the automaton, but it calculates the answer when simulating the build process
    * it can be simply merged with the build function, but I did not do that because I want the code to be more generic
    *
    * @returns integer, the answer to the described problem
    */
    int solve() {
        queue<int> automatonQueue;
        for (int i = 0; i < 26; i++) {
            if (trie[0].to[i])automatonQueue.push(trie[0].to[i]);
        }
        int ans = 0;
        while (automatonQueue.size()) {
            auto node = automatonQueue.front();
            automatonQueue.pop();
            // a previous sub-string can appear either from the parent node directly, or from the suffix link node
            trie[node].value =
                    trie[node].counter + max(trie[trie[node].parentNode].value, trie[trie[node].suffixLink].value);
            ans = max(ans, trie[node].value);
            for (int i = 0; i < 26; i++) {
                if (trie[node].to[i]) {
                    automatonQueue.push(trie[node].to[i]);
                }
            }
        }
        return ans;
    }
} automaton;

int main() {
    int numberOfStrings;
    cin >> numberOfStrings;
    for (int i = 0; i < numberOfStrings; i++) {
        string str;
        cin >> str;
        automaton.addString(str);
    }
    automaton.build();
    cout << automaton.solve();
}
