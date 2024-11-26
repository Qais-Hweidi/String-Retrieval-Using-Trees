#include <iostream>
#include <vector>
using namespace std;

class Trie {
private:
    static const int CHAR_RANGE = 128;

    struct TrieNode {
        TrieNode* children[CHAR_RANGE] = { nullptr };

        TrieNode* addChild(unsigned char c, TrieNode* node) {
            return children[c] = node;
        }

        ~TrieNode() {
            for (int i = 0; i < CHAR_RANGE; ++i) {
                delete children[i];
            }
        }

        TrieNode* getChild(unsigned char c) {
            return children[c];
        }

        TrieNode* addChild(unsigned char c) {
            return addChild(c, new TrieNode());
        }

        void removeChild(unsigned char c) {
            delete getChild(c);
            children[c] = nullptr;
        }

        TrieNode* getOrCreateChild(unsigned char c) {
            return children[c] ? children[c] : addChild(c);
        }

        bool isLeaf() {
            for (int i = 0; i < CHAR_RANGE; ++i) {
                if (i != '$' && children[i]) {
                    return false;
                }
            }
            return true;
        }

        bool hasChild(unsigned char c) {
            return children[c] != nullptr;
        }

        bool hasOtherChildren(unsigned char c) {
            for (int i = 0; i < CHAR_RANGE; ++i) {
                if (i != c && children[i]) {
                    return true;
                }
            }
            return false;
        }
    };

    TrieNode* root = new TrieNode();

    void collectWords(vector<string>& words, TrieNode* node, const string& prefix) {
        if (node->hasChild('$')) {
            words.push_back(prefix);
        }
        for (int i = 0; i < CHAR_RANGE; ++i) {
            if (TrieNode* child = node->getChild(i)) {
                collectWords(words, child, prefix + char(i));
            }
        }
    }

public:
    void addWord(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            node = node->getOrCreateChild(c);
        }
        node->addChild('$');
    }

    bool searchWord(const string& word) { 
        TrieNode* node = root;
        for (char c : word) {
            node = node->getChild(c);
            if (!node) return false;
        }
        return node->hasChild('$');
    }

    bool deleteWord(const string& word) {
        TrieNode* node = root;
        TrieNode* lastNonLeafNode = nullptr;
        char lastNonLeafChar = 0;

        for (char c : word) {
            if (!node) return false;
            if (node->hasOtherChildren(c) || node->hasChild('$')) {
                lastNonLeafNode = node;
                lastNonLeafChar = c;
            }
            node = node->getChild(c);
        }

        if (!node || !node->hasChild('$')) return false;
        node->removeChild('$');

        if (node->isLeaf() && lastNonLeafNode) {
            lastNonLeafNode->removeChild(lastNonLeafChar);
        }

        return true;
    }

    void findWordsWithPrefix(vector<string>& words, const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            node = node->getChild(c);
            if (!node) return;
        }
        collectWords(words, node, prefix);
    }

    string findLongestWord() {
        vector<string> words;
        findWordsWithPrefix(words, "");
        string longest;
        for (const string& word : words) {
            if (word.length() > longest.length()) {
                longest = word;
            }
        }
        return longest;
    }
};

int main() {
    int commandCount;
    string command, argument;
    Trie trie;

    cin >> commandCount;
    for (int i = 0; i < commandCount; ++i) {
        cin >> command;

        if (command == "insert") {
            cin >> argument;
            trie.addWord(argument);
        }
        else if (command == "delete") {
            cin >> argument;
            trie.deleteWord(argument);
        }
        else if (command == "startwith") {
            cin >> argument;
            vector<string> results;
            trie.findWordsWithPrefix(results, argument);
            for (const string& word : results) {
                cout << word << '\n';
            }
        }
        else if (command == "search") {
            cin >> argument;
            cout << (trie.searchWord(argument) ? "YES" : "NO") << '\n';
        }
        else if (command == "longest") {
            cout << trie.findLongestWord() << '\n';
        }
    }

    return 0;
}
