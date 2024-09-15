#ifndef knowhow_c
#define knowhow_c
#include <interface.c>
#include <public.c>
#include <protected.c>
#include <private.c>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
//
//	new_node()
//
node_t* new_node(void)
{
	node_t *pNew = malloc(sizeof(node_t));

	pNew->ref.key      = 0;
	pNew->ref.p_context = NULL;
	pNew->is_red        = true;
	pNew->p_child[0]    = NULL;
	pNew->p_child[1]    = NULL;

	return pNew;
}


/////////////////////////////////////////////////////////////////////////////
//
//	LookUp()
//
//	Searches the binary tree for the given key, returning the associated
//	context pointer.
//
//	Returns NULL if the key is not in the tree.
//
void* lookup(TREE, u_int32_t key)
{
	ROOT = p_tree->data;

	while (NULL != p_root) {
		// Is this the key for which we're looking?
		if (key == p_root->ref.key) {
			return p_root->ref.p_context;
		}

		// If not, we need to traverse left or right down the binary tree.
		else if (key < p_root->ref.key) {
			p_root = p_root->p_child[0];
		}
		else {
			p_root = p_root->p_child[1];
		}
	}

	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
//
//	single_rotation()
//
//	Applies a single rotation to the specified node.
//
//	The <dir> parameter controls whether this is a clockwise or counter
//	clockwise rotation.
//
//	If dir == 0, nodes N (p_root) and S (pSave) will be rotated like this:
//
//		N              S
//	 1     S   ==>  N     3
//	      2 3      1 2
//
//	If dir == 1, nodes N (p_root) and S (pSave) will be rotated like this:
//
//	     N             S
//	  S    3   ==>  1     N
//	 1 2                 2 3
//
//	Note that these two rotates are the inverse of each other.
//
static node_t* single_rotation(ROOT, int dir)
{
	node_t *pSave = p_root->p_child[!dir];

	p_root->p_child[!dir] = pSave->p_child[dir];
	pSave->p_child[dir]  = p_root;
 
	p_root->is_red = true;
	pSave->is_red = false;

	return pSave;
}


/////////////////////////////////////////////////////////////////////////////
//
//	double_rotation()
//
static node_t* double_rotation(ROOT, int dir)
{
	p_root->p_child[!dir] = single_rotation(p_root->p_child[!dir], !dir);

	return single_rotation(p_root, dir);
}


/////////////////////////////////////////////////////////////////////////////
//
//	node_is_red()
//
//	Use a function to test if a node is black, since some pointers may be
//	NULL.
//
static inline bool node_is_red(ROOT)
{
	return ((NULL != p_root) && p_root->is_red);
}

/////////////////////////////////////////////////////////////////////////////
//
//	Delete()
//
void delete(TREE, const u_int32_t key)
{
	if (NULL == p_tree->data) {
		return;
	}

	// Use a dummy placeholder to initialize the traversal pointers.
	// This will simplify looping logic, since it makes certain that
	// the pointers are never NULL even when looking at the root of
	// the tree.  This eliminates the need for special-case handling
	// of NULL pointers in the inner loop below.
	node_t head = {0};

	node_t *pPrev  = NULL;
	node_t *pCurr  = NULL;
	node_t *pScan  = &head;
	node_t *pFound = NULL;

	head.p_child[1] = p_tree->data;

	int prevDir = 1;
	int currDir = 1;

	// Keep scanning until we hit a leaf.  Once we find a leaf, the
	// key stored in that leaf can be used to replace the key to be
	// deleted (assuming that the target key is found in the tree).
	while (NULL != pScan->p_child[currDir]) {
		pPrev   = pCurr;
		pCurr   = pScan;
		pScan   = pScan->p_child[currDir];
		prevDir = currDir;
		currDir = (pScan->ref.key < key);

		// If we found the key to be deleted, record which node contains
		// it.  This value will need to be replaced with another key (the
		// in-order successor of the key to delete), but we won't know
		// what value to use until we have exited this loop.
		if (pScan->ref.key == key) {
			pFound = pScan;
		}

		// Traverse down black links in the tree.
		//
		// Red nodes are not ignored.  Instead, they are processed
		// from their parent node (which is always a black node).
		//
		if ((false == node_is_red(pScan)) &&
			(false == node_is_red(pScan->p_child[currDir])))
		{
			// Once we get into this code block, we know that pScan is a
			// black node.  Since currDir indicates which child will be
			// processed next, we need to check the other child (!currDir).
			//
			// If the opposite child is red, then the deletion may cause
			// the target child to be turned into a red node.  Since a
			// black node may not have two red children, we need to apply
			// a rotation to the opposite child to fix this.
			//
			if (node_is_red(pScan->p_child[!currDir])) {
				// A rotation is needed to avoid red violations.  Since
				// the rotation changes the local arrangement of the tree,
				// we also need to update pCurr to keep proper track of
				// how we reached this node.

				pCurr->p_child[prevDir] = single_rotation(pScan, currDir);
				pCurr                  = pCurr->p_child[prevDir];
			}

			// Otherwise the target node is black, so we need to examine
			// its children, fix coloring to avoid red violations, and
			// possibly rotate part of the tree to rebalance the tree to
			// prepare for removing a node.
			else {
				node_t *pTemp = pCurr->p_child[!prevDir];

				if (NULL != pTemp) {

					// If we end up with this particular arrangement of all
					// black nodes, we can safely convert a couple of them
					// to be red nodes without introducing a red violation.
					if ((false == node_is_red(pTemp->p_child[0])) &&
						(false == node_is_red(pTemp->p_child[1])))
					{
						pCurr->is_red = false;
						pTemp->is_red = true;
						pScan->is_red = true;
					}

					// Otherwise one of the children is red.  If a red
					// violation is detected, we have to fix the violation
					// by applying a rotation to the current node.
					else {
						int tempDir = (pPrev->p_child[1] == pCurr);

						if (node_is_red(pTemp->p_child[prevDir])) {
							pPrev->p_child[tempDir] = double_rotation(pCurr, prevDir);
						}
						else if (node_is_red(pTemp->p_child[!prevDir])) {
							pPrev->p_child[tempDir] = single_rotation(pCurr, prevDir);
						}

						// Mark all of these nodes to have a safe
						// arrangement of red and black that cannot
						// possibly have red violations.
						pScan->is_red                             = true;
						pPrev->p_child[tempDir]->is_red            = true;
						pPrev->p_child[tempDir]->p_child[0]->is_red = false;
						pPrev->p_child[tempDir]->p_child[1]->is_red = false;
					}
				}
			}
		}
	}

	// Now that we have finished scanning the tree, we know
	// whether or not the search key is present in the tree.
	//
	// If the key is in the tree, we can remove the key by replacing it
	// with the value in pScan.  Since pScan is guaranteed to be a leaf
	// node, we then delete that node, since it is no longer needed in
	// the tree.
	if (NULL != pFound) {
		pFound->ref = pScan->ref;
		pCurr->p_child[pCurr->p_child[1] == pScan] = pScan->p_child[pScan->p_child[0] == NULL];
		free(pScan);
	}

	// Rotations may have propagated back to the root of the tree.
	// If so, the new root pointer is cached in the dummy structure.
	p_tree->data = head.p_child[1];

	// The root of a red-black tree must always be black.  It is safe to
	// mark the root as always being black to satisfy this requirement.
	if (NULL != p_tree->data) {
		p_tree->data->is_red = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Insert()
//
//	This is an iterative top-down insertion.
//
void insert(TREE, void_ref_t ref)
{
	// The standard case is inserting into a non-empty tree.
	if (NULL != p_tree->data) {
		// Use a dummy placeholder to initialize the traversal pointers.
		// This will simplify looping logic, since it makes certain that
		// the pointers are never NULL even when looking at the root of
		// the tree.  This eliminates the need for special-case handling
		// of NULL pointers in the inner loop below.
		node_t head = {0};

		int prevDir = 0;
		int currDir = 0;

		head.p_child[1] = p_tree->data;

		// Use a couple pointers to traverse through the binary tree.
		// Also record the previous value for each of these pointers,
		// which are needed to update child pointers in the parent nodes.
		node_t *pParentPrev = &head;
		node_t *pParentCurr = NULL;
		node_t *pEnumPrev   = NULL;
		node_t *pEnumCurr   = p_tree->data;

		// Use an infinite loop for traversing the tree.  There is an ending
		// condition, but that condition is most easily tested in the middle
		// of the loop.
		for ( ; ; ) {
			// Once we hit a leaf node, insert the value below the last node
			// that was examined (pEnumPrev is the parent of pEnumCurr).
			if (NULL == pEnumCurr) {
				pEnumCurr                  = new_node();
				pEnumPrev->p_child[currDir] = pEnumCurr;
				pEnumCurr->ref             = ref;

				// NOTE: Even though we have inserting the new value into
				// the tree, we do not break out of the loop.  We still need
				// to execute some more code to fix any red violations that
				// were introduced by the insertion.
			}

			// Only one child of a black node can be red.  If we encounter a
			// node where both children are red, we have placed the tree in
			// an invalid state.  Apply a color flip to force the current
			// node to be red and make both of its children black.
			else if (node_is_red(pEnumCurr->p_child[0]) && node_is_red(pEnumCurr->p_child[1])) {
				pEnumCurr->is_red            = true;
				pEnumCurr->p_child[0]->is_red = false;
				pEnumCurr->p_child[1]->is_red = false;
			}

			// A red violation occurs if the both the current node and its
			// parent are red.  This indicates that the tree is not properly
			// balanced.  Apply a rotation to rebalance this part of the
			// tree, which will also fix the red violation.
			if (node_is_red(pEnumCurr) && node_is_red(pEnumPrev)) {
				int dir2 = (pParentPrev->p_child[1] == pParentCurr);

				if (pEnumCurr == pEnumPrev->p_child[prevDir]) {
					pParentPrev->p_child[dir2] = single_rotation(pParentCurr, !prevDir);
				}
				else {
					pParentPrev->p_child[dir2] = double_rotation(pParentCurr, !prevDir);
				}
			}

			// If we see the key we're trying to write, we can break out
			// of the loop.  In most cases the key was just inserted, and
			// this code is being applied after fixing any red violations.
			// However, it is possible that the key was already stored in
			// the tree, so we need to write the key/value pair again to
			// handle the case where an existing key is being updated with
			// a new value.
			if (pEnumCurr->ref.key == ref.key) {
				pEnumCurr->ref = ref;
				break;
			}

			// Keep track of whether we traversed left or right on the
			// previous iteration, so we know which child pointer to update.
			prevDir = currDir;
			currDir = pEnumCurr->ref.key < ref.key;

			// Keep track of the previous parent pointer so we can update
			// its child pointers on subsequent iterations.
			if (NULL != pParentCurr) {
				pParentPrev = pParentCurr;
			}

			pParentCurr = pEnumPrev;
			pEnumPrev   = pEnumCurr;
			pEnumCurr   = pEnumCurr->p_child[currDir];
		}

		// If rotations propagated all the way to the root, the updated
		// root pointer will now be stored in the dummy structure.
		p_tree->data = head.p_child[1];
	}

	// Special case for inserting into an empty tree.
	else {
		p_tree->data = new_node();
		p_tree->data->ref = ref;
	}

	// Rotations may have resulted in a red node becoming the root of the
	// tree.  Force the root node to always be black to conform with the
	// rules of a red-black tree.
	p_tree->data->is_red = false;
}



/////////////////////////////////////////////////////////////////////////////
//
//	sanity_check_rec()
//
void sanity_check_rec(ROOT, bool isParentRed, u_int32_t blackDepth, u_int32_t *minBlack, u_int32_t *maxBlack)
{
	if (p_root->is_red) {
		// The parent of a red node must be black.
		printf("%d %s\n",__LINE__,(false == isParentRed)?"ok":"assert");

	}
	else {
		++blackDepth;
	}

	// If the node has two children, only one of them may be red.
	// The other must be black.
/* Do not apply this test.  This version of red-black trees does not enforce this rule.
	if ((NULL != p_root->p_child[0]) && (NULL != p_root->p_child[1])) {
		if (node_is_red(p_root->p_child[0])) {
			printf("%d %s\n",__LINE__,(false == node_is_red(p_root->p_child[1]))?"ok":"assert");
		}
		if (node_is_red(p_root->p_child[1])) {
			printf("%d %s\n",__LINE__,(false == node_is_red(p_root->p_child[0]))?"ok":"assert");
		}
	}
*/

	// The child of a red node must be black.
	if (isParentRed) {
		printf("%d %s\n",__LINE__,(false == p_root->is_red)?"ok":"assert");
	}

	if (NULL != p_root->p_child[0]) {
		// The left child must come before this node in sorting order.
		printf("%d %s\n",__LINE__,(p_root->p_child[0]->ref.key < p_root->ref.key)?"ok":"assert");

		sanity_check_rec(p_root->p_child[0], p_root->is_red, blackDepth, minBlack, maxBlack);
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}

	if (NULL != p_root->p_child[1]) {
		// The right child must come after this node in sorting order.
		printf("%d %s\n",__LINE__,(p_root->p_child[1]->ref.key > p_root->ref.key)?"ok":"assert");

		sanity_check_rec(p_root->p_child[1], p_root->is_red, blackDepth, minBlack, maxBlack);
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	SanityCheck()
//
//	Verifies that the red-black tree is properly formed.
//
void sanity_check(TREE)
{
	if (NULL != p_tree->data) {
		u_int32_t minBlack = 0xFFFFFFFF;
		u_int32_t maxBlack = 0;

		// The root of the tree must always be a black node.
		printf("%d %s\n",__LINE__,(false == p_tree->data->is_red)?"ok":"assert");

		sanity_check_rec(p_tree->data, false, 0, &minBlack, &maxBlack);

		printf("%d %s\n",__LINE__,(minBlack == maxBlack)?"ok":"assert");
	}
}




/////////////////////////////////////////////////////////////////////////////
//
//	KeyCountRec()
//
u_int32_t key_count_rec(ROOT)
{
	u_int32_t tally = 0;

	if (NULL != p_root) {
		tally += 1;

		if (NULL != p_root->p_child[0]) {
			tally += key_count_rec(p_root->p_child[0]);
		}

		if (NULL != p_root->p_child[1]) {
			tally += key_count_rec(p_root->p_child[1]);
		}
	}

	return tally;
}
/////////////////////////////////////////////////////////////////////////////
//
//	KeyCount()
//
//	Recursively count the number of keys in the tree.
//
u_int32_t key_count(TREE)
{
	return key_count_rec(p_tree->data);
}

/////////////////////////////////////////////////////////////////////////////
//
//	LeafDepthRec()
//
u_int32_t leaf_depth_rec(ROOT, u_int32_t depth, u_int32_t ary[], u_int32_t depthLimit, u_int32_t blackDepth, u_int32_t *minBlack, u_int32_t *maxBlack)
{
	// Increment the count of nodes at the current depth of the tree.
	ary[depth] += 1;

	// If this is a black node, increment the count of black nodes along
	// this path from the root.
	if (false == p_root->is_red) {
		++blackDepth;
	}

	// If this is a leaf node, return the total depth of this node.
	if ((NULL == p_root->p_child[0]) && (NULL == p_root->p_child[1])) {
		return depth + 1;
	}

	// Protect against overflowing the array.  This should not be necessary,
	// since the tree would have to contain on the order of 2^64 nodes to
	// overflow the array.
	if (depth >= depthLimit) {
		return depth;
	}

	// Record the total depth along both the left and right children.
	u_int32_t d1 = 0;
	u_int32_t d2 = 0;

	if (NULL != p_root->p_child[0]) {
		d1 = leaf_depth_rec(p_root->p_child[0], depth + 1, ary, depthLimit, blackDepth, minBlack, maxBlack);
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}

	if (NULL != p_root->p_child[1]) {
		d2 = leaf_depth_rec(p_root->p_child[1], depth + 1, ary, depthLimit, blackDepth, minBlack, maxBlack);
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}

	// Return the maximum node depth found along this path from the root.
	return Max(d1, d2);
}

/////////////////////////////////////////////////////////////////////////////
//
//	LeafDepth()
//
//	Tests the "black depth" of every leaf node.  This measure is the number
//	of black nodes between each leaf and the root (including the root, since
//	the root of a red-black tree must be black).  The tree depth of every
//	leaf node will be greater, since red nodes are not included in this
//	measure.
//
//	Once complete, it will report the minimum and maximum number of black
//	nodes found for each leaf.  Due to the nature of a red-black tree, these
//	two values must be the same.
//
void leaf_depth(TREE)
{
	// This array will be used to track the number of nodes at each depth of
	// the binary tree.  This is the true depth of each node, not the black
	// depth.
	u_int32_t ary[64];
	for (u_int32_t i = 0; i < ArraySize(ary); ++i) {
		ary[i] = 0;
	}

	u_int32_t maxDepth = 0;
	u_int32_t minBlack = 0xFFFFFFFF;
	u_int32_t maxBlack = 0;

	if (NULL != p_tree->data) {
		maxDepth = leaf_depth_rec(p_tree->data, 0, ary, ArraySize(ary), 0, &minBlack, &maxBlack);
	}

	u_int32_t tally = 0;

	for (u_int32_t i = 0; i < maxDepth; ++i) {
		tally += ary[i];

		printf("%3d: %5d = %5d\n", i, ary[i], tally);
	}

	printf("black depth: %d to %d\n", minBlack, maxBlack);

	// All leaf nodes must have the same number of black nodes between the
	// root and each leaf.  Therefore the minimum and maximum number of black
	// nodes must be the same.
	printf("%d %s\n",__LINE__,(minBlack == maxBlack)?"ok":"assert");
}

/////////////////////////////////////////////////////////////////////////////
//
//	TraverseRec()
//
void traverse_rec(ROOT, u_int32_t *prev)
{
	if (NULL != p_root->p_child[0]) {
		traverse_rec(p_root->p_child[0], prev);
	}

	*prev = p_root->ref.key;
	printf("%4d", p_root->ref.key);

	if (NULL != p_root->p_child[1]) {
		traverse_rec(p_root->p_child[1], prev);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Traverse()
//
//	Perform an in-order traversal of the tree, printing out all of the key
//	values in the tree.  This should display all keys in their sorted order.
//
void traverse(TREE)
{
	if (NULL != p_tree->data) {
		u_int32_t prev = 0;
		traverse_rec(p_tree->data, &prev);
		printf("\n\n");
	}
}
#endif
