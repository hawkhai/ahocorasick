#include "stdafx.h"
#include <map>
#include <vector>
#include <queue>

typedef unsigned char BYTE;
typedef std::vector<BYTE> WORD;
typedef struct TrieNode NODE;
typedef std::map<BYTE, NODE*> CHILDREN;
typedef std::vector<WORD> WORDS;

#define CONTAIN(children, value) (children.find(value) != children.end())

struct TrieNode {
    BYTE value;
    NODE* fail;
    int tail;
    CHILDREN children;

    TrieNode(BYTE value = NULL) {
        this->value = value;
        this->fail = NULL;
        this->tail = 0;
    }
};

class Trie {
public:
    Trie(const WORDS& words) {
        this->root = new NODE();
        this->count = 0;
        this->words = words;
        int size = words.size();
        for (int i = 0; i < size; i++) {
            insert(words[i]);
        }
        automation();
    }
    ~Trie() {
        // ÊÍ·ÅÄÚ´æ¡£
    }

private:
    void insert(const WORD& word) {
        this->count += 1;
        NODE* curnode = this->root;
        int size = word.size();
        for (int i = 0; i < size; i++) {
            BYTE item = word[i];
            if (!CONTAIN(curnode->children, item)) {
                NODE* child = new NODE(item);
                curnode->children[item] = child;
                curnode = child;
            } else {
                curnode = curnode->children[item];
            }
        }
        curnode->tail = this->count;
    }

    void automation() {
        std::queue<NODE*> queue;
        queue.push(this->root);
        while (!queue.empty()) {
            NODE* tempnode = queue.front();
            queue.pop();
            CHILDREN::iterator iter = tempnode->children.begin();
            for (; iter != tempnode->children.end(); iter++) {
                NODE* value = iter->second;
                if (tempnode == this->root) {
                    value->fail = this->root;
                } else {
                    NODE* p = tempnode->fail;
                    while (p) {
                        if (CONTAIN(p->children, value->value)) {
                            value->fail = p->children[value->value];
                            break;
                        }
                        p = p->fail;
                    }
                    if (!p) {
                        value->fail = this->root;
                    }
                }
                queue.push(value);
            }
        }
    }

public:
    void search(const WORD& text) {
        NODE* p = this->root;
        int startindex = 0;
        int size = text.size();
        for (int i = 0; i < size; i++) {
            BYTE singlechar = text[i];

            while ((!CONTAIN(p->children, singlechar)) && p != this->root) {
                p = p->fail;
            }
            if (CONTAIN(p->children, singlechar) && p == this->root) {
                startindex = i;
            }
            if (CONTAIN(p->children, singlechar)) {
                p = p->children[singlechar];
            } else {
                startindex = i;
                p = this->root;
            }
            NODE* temp = p;
            while (temp != this->root) {
                if (temp->tail) {
                    WORD& word = words[temp->tail - 1];
                    int start = startindex;
                    int end = i;
                    printf("%s %d %d \r\n", &word[0], start, end);
                }
                temp = temp->fail;
            }
        }
    }

private:
    NODE* root;
    int count;
    WORDS words;
};

WORD& setword(WORD& temp, const char* data) {
    int size = strlen(data);
    temp.resize(size);
    memcpy(&temp[0], data, size);
    return temp;
}

int _tmain(int argc, _TCHAR* argv[])
{
    WORDS words;
    WORD temp;
    words.push_back(setword(temp, "12"));
    words.push_back(setword(temp, "13"));
    words.push_back(setword(temp, "456"));

    WORD test_text;
    setword(test_text, "12z13d78~|~9a456bc");
    Trie model(words);
    // {'12': [(0, 1)], '13': [(3, 4)], '456': [(13, 15)]})
    model.search(test_text);
    return 0;
}

