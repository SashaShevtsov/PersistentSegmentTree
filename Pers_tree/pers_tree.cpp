#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

struct node {
	long long value;
	node* left, *right;

	node() : value(0LL), left(NULL), right(NULL) {}

	node(long long v) : value(v), left(NULL), right(NULL) {}

	node(long long v, node* l, node* r) : value(v), left(l), right(r) {}
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

bool cmp(const indexedElement &a, const indexedElement &b) {
	if (a.value < b.value)
		return true;
	return false;
}

struct request {
	int l, r;
	int x, y;

	request() : l(0), r(0), x(0), y(0) {}
};

class PersTree {
	int n;
	indexedElement* allElements;
	node** versionRoots;

	void buildZeroTree(node* root, int left, int right) {
		if (left == right)
			return;
		int mid = (left + right) / 2;
		root->left = new node();
		buildZeroTree(root->left, left, mid);
		root->right = new node();
		buildZeroTree(root->right, mid + 1, right);
	}

	node* addNewVersion(node* root, int ind, int left, int right) {
		if (ind <= left && right <= ind)
			return new node(1LL);

		if (right < ind || ind < left)
			return root;

		int mid = (left + right) / 2;
		node* res = new node(root->value + 1);
		if (ind <= mid) {
			res->right = root->right;
			res->left = addNewVersion(root->left, ind, left, mid);
		} 
		else {
			res->left = root->left;
			res->right = addNewVersion(root->right, ind, mid + 1, right);
		}
		return res;
	}

	int getLeftVersionNumber(int v) {
		if (v <= allElements[0].value)
			return -1;

		int l = 0;
		int r = n - 1;
		int mid = 0;
		while (l <= r) {
			mid = (l + r) / 2;
			if (allElements[mid].value >= v)
				r = mid - 1;
			else
				l = mid + 1;
		}
		return r;
	}

	int getRightVersionNumber(int v) {
		if (v >= allElements[n - 1].value)
			return n - 1;

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
		return r;
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

		sort(allElements, allElements + n, cmp);

		versionRoots = new node*[n + 1];
		versionRoots[0] = new node();
		buildZeroTree(versionRoots[0], 0, n - 1);

		for (int i = 0; i < n; i++)
			versionRoots[i+1] = addNewVersion(versionRoots[i], allElements[i].index, 0, n - 1);
	}

	long long processRequest(request req) {
		int indX = getLeftVersionNumber(req.x);
		int indY = getRightVersionNumber(req.y);

		long long y = getAmount(versionRoots[indY + 1], req.l - 1, req.r - 1, 0, n - 1);
		long long x = getAmount(versionRoots[indX + 1], req.l - 1, req.r - 1, 0, n - 1);

		return y - x;
	}
};

int main() {
	ios::sync_with_stdio(false);
	int n, q;
	cin >> n >> q;

	int* elements = new int[n];
	request* requests = new request[q];
	string result = "";

	for (int i = 0; i < n; i++)
		cin >> elements[i];

	for (int j = 0; j < q; j++)
		cin >> requests[j].l >> requests[j].r >> requests[j].x >> requests[j].y;

	PersTree segmentTree(n, elements);

	for (int i = 0; i < q; i++)
		result.append(to_string(segmentTree.processRequest(requests[i])) + '\n');
	cout << result;

	return 0;
}
