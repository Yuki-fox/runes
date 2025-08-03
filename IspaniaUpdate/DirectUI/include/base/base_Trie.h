

#ifndef MING_BASE_TRIE_INCL
#define MING_BASE_TRIE_INCL
#include <base\base_base.h>

namespace base
{


//#define TRIE_SUPPORT_PRINTING	// for debugging use only -- includes entry point to print contents of Trie

// forward declare family classes
class trie_node;
class CTrieIterator;

class BASE_API CTrie
{
	friend CTrieIterator;
private:
	virtual void getKey(void *data, unsigned char **key, uint32_t *keylen) = 0;

	CTrie(CTrie &trie2);		// prevent copying (never instantiated)

#ifdef TRIE_SUPPORT_PRINTING
	virtual void printKey(const char *szPrefix, void *data) = 0;
	void printNode(const char *szPrefix, char chHeader, trie_node *pNode);
#endif

protected:
	trie_node *m_root;

public:
	CTrie();
	virtual ~CTrie();

	void *find(unsigned char *key, uint32_t keylen);
	void *add(void *data);
//	void *Add(unsigned char *key, int keylen, void *data);
	void *del(void *data);
	void *del(unsigned char *key, uint32_t keylen);

#ifdef TRIE_SUPPORT_PRINTING
	void print(void);
#endif
};

class CTrieIterator
{
private:
	CTrie     *m_trie;
	trie_node *m_node;

public:
	CTrieIterator(CTrie &trie);
	~CTrieIterator(void) { };

	bool operator --(void);		// returns false if this is the first entry in the trie
	bool operator ++(void);		// returns false if this is the last entry in the trie
	void *getData(void) const;	// returns data element at current position
	bool atEnd(void) const;		// returns true if iterator is at the end of the trie
	bool atBegin(void) const;   // returns true if iterator is at the start of the trie
	void rewind(void);
};

}
#endif // MING_BASE_TRIE_INCL
