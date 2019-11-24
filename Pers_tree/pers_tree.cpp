#include <iostream>

using namespace std;

struct node {
	long long value;

	node* nextVersion;
	node* left, *right;

	node() : value(0LL), nextVersion(NULL), left(NULL), right(NULL) {}

	node(long long v) : value(v), nextVersion(NULL), left(NULL), right(NULL) {}

	node(long long v, node* l, node* r) : value(v), nextVersion(NULL), left(l), right(r) {}
};

struct indexedElement {
	long long value;
	int index;

	void set(long long v, int ind) {
		value = v;
		index = ind;
	}

	void copy(indexedElement el) {
		value = el.value;
		index = el.index;
	}
};

struct request {
	int l, r;
	int x, y;

	request() : l(0), r(0), x(0), y(0) {}

	request(int l, int r, int x, int y) : l(l), r(r), x(x), y(y) {}
};

class PersTree {
	int n;
	indexedElement* allElements;
	node* zeroVerRoot;

	void sortElements(indexedElement* elements, int k) {
		int i = 0, j = k - 1;
		indexedElement temp, p;

		p = elements[k >> 1];

		while (i <= j) {
			while (elements[i].value < p.value)
				i++;
			while (elements[j].value > p.value)
				j--;

			if (i <= j) {
				temp.copy(elements[i]);
				elements[i].copy(elements[j]);
				elements[j].copy(temp);
				i++;
				j--;
			}
		}

		if (j > 0)
			sortElements(elements, j);
		if (k > i)
			sortElements(elements + i, k - i);
	}

	void buildZeroTree(node* root, int left, int right) {
		if (left == right)
			return;
		int mid = (left + right) / 2;
		root->left = new node();
		buildZeroTree(root->left, left, mid);
		root->right = new node();
		buildZeroTree(root->right, mid + 1, right);
	}

	void addNewVersion(node* root, int ind, int left, int right) {
		if (ind <= left && right <= ind)
			root->nextVersion = new node(1LL);

		if (right < ind || ind < left)
			return;

		int mid = (left + right) / 2;
		addNewVersion(root->left, ind, left, mid);
		addNewVersion(root->right, ind, mid + 1, right);

		if (root->left->nextVersion != NULL)
			root->nextVersion = new node(root->value + 1LL, root->left->nextVersion, root->right);
		if (root->right->nextVersion != NULL)
			root->nextVersion = new node(root->value + 1LL, root->left, root->right->nextVersion);
	}

	int getVersionNumber(int v) {
		int l = 0;
		int r = n - 1;
		int mid = 0;
		while (l <= r) {
			mid = (l + r) / 2;
			if (allElements[mid].value > v)
				r = mid - 1;
			else
				l = mid + 1;
		}
		return allElements[r].index;
	}

	node* getRootVersion(int vers) {
		node* root = zeroVerRoot;
		for (int i = 0; i < vers; i++)
			root = zeroVerRoot->nextVersion;
		return root;
	}

	long long getAmount(node* root, int l, int r, int left, int right) {
		if (l <= left && right <= r)
			return root->value;
		
		if (right < l || r < left)
			return 0LL;

		int mid = (left + right) / 2;
		return getAmount(root->left, l, r, left, mid) + 
			getAmount(root->right, l, r, mid + 1, right);
	}

public:
	PersTree(int n, int* values) : n(n) {
		allElements = new indexedElement[n];
		for (int i = 0; i < n; i++)
			allElements[i].set((long long)values[i], i);

		sortElements(allElements, n);

		zeroVerRoot = new node();
		buildZeroTree(zeroVerRoot, 0, n - 1);

		node* root = zeroVerRoot;
		for (int i = 0; i < n; i++) {
			addNewVersion(root, allElements[i].index, 0, n - 1);
			root = root->nextVersion;
		}
	}

	long long processRequest(request req) {
		int indX = getVersionNumber(req.x);
		int indY = getVersionNumber(req.y);


	}
};
