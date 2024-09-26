#ifndef knowhow_c
#define knowhow_c
#include <interface.c>
#include <public.c>
#include <protected.c>
#include <private.c>
#include <stdlib.h>

<<<<<<< HEAD

=======
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
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b


/////////////////////////////////////////////////////////////////////////////
//
//	LookUp()
//
<<<<<<< HEAD
//	Since a red-black tree is a binary tree, look-up operations are done
//	using iterative traversal.
//
//	This assumes that all keys stored in the tree have been assigned non-NULL
//	values.
//
//	If the key is not in the tree, this will return NULL.
//
void* lookup(TREE, u_int32_t key)
{
	node_t *p  = p_tree->data;

	while (NULL != p) {
		if (key == p->ref.key) {
			return p->ref.p_context;
		}
		else if (key < p->ref.key) {
			p = p->p_left;
		}
		else {
			p = p->p_right;
=======
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
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
		}
	}

	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
//
<<<<<<< HEAD
//	node_is_red()
//
=======
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
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
static inline bool node_is_red(ROOT)
{
	return ((NULL != p_root) && p_root->is_red);
}

<<<<<<< HEAD

/////////////////////////////////////////////////////////////////////////////
//
//	rotate_left()
//
//	Rotate the subtree to the left:
//
//	   4            6
//	  / \          /
//	 2   6  -->   4
//	             /
//	            2
//
//	Node '6' will take on the color '4' used to have, while '4' becomes a
//	red node.
//
static node_t* rotate_left(ROOT)
{
	node_t *pTemp = p_root->p_right;
	p_root->p_right   = pTemp->p_left;
	pTemp->p_left    = p_root;
	pTemp->is_red    = p_root->is_red;
	p_root->is_red    = true;

	return pTemp;
}


/////////////////////////////////////////////////////////////////////////////
//
//	rotate_right()
//
//	Rotate the subtree to the right:
//
//	   4        2
//	  / \        \
//	 2   6  -->   4
//	               \
//	                6
//
//	Node '2' will take on the color '4' used to have, while '4' becomes a
//	red node.
//
static node_t* rotate_right(ROOT)
{
	node_t *pTemp = p_root->p_left;
	p_root->p_left    = pTemp->p_right;
	pTemp->p_right   = p_root;
	pTemp->is_red    = p_root->is_red;
	p_root->is_red    = true;

	return pTemp;
}


/////////////////////////////////////////////////////////////////////////////
//
//	color_flip()
//
//	Flips the color of the node and both of its children from black to red
//	(or red to black).
//
//	Any time that a color flip is applied, it may cause p_root to end up
//	with an invalid color relative to its parent node.  Therefore some kind
//	of fix-up operation will be required after this function is called.
//
static void color_flip(ROOT)
{
	p_root->is_red = !p_root->is_red;

	if (NULL != p_root->p_left) {
		p_root->p_left->is_red  = !p_root->p_left->is_red;
	}

	if (NULL != p_root->p_right) {
		p_root->p_right->is_red = !p_root->p_right->is_red;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	insert_rec()
//
//	Note that during rebalancing, a rotation may change the value assigned
//	to p_root.  This will change which node is the root of the subtree that
//	is being modified.  The caller will need to use the returned pointer
//	to replace whichever value was passed into insert_rec.
//
node_t* insert_rec(ROOT, void_ref_t ref)
{
	// Special case for inserting a leaf.  Just return the pointer;
	// the caller will insert the new node into the parent node.
	if (NULL == p_root) {
		p_root      = new_node();
		p_root->ref = ref;
		return p_root;
	}

	// If we perform the color flip here, the tree is assembled as a
	// mapping of a 2-3-4 tree.
#if defined(USE_234_TREE)
	// This color flip will effectively split 4-nodes on the way down
	// the tree (since 4-nodes must be represented by a node with two
	// red children).  By performing the color flip here, the 4-nodes
	// will remain in the tree after the insertion.
	if (node_is_red(p_root->p_left) && node_is_red(p_root->p_right)) {
		color_flip(p_root);
	}
#endif

	// Check to see if the value is already in the tree.  If so, we
	// simply replace the value of the key, since duplicate keys are
	// not allowed.
	if (ref.key == p_root->ref.key) {
		p_root->ref = ref;
	}

	// Otherwise recurse left or right depending on key value.
	//
	// Note: p_left or p_right may be a NULL pointer before recursing.
	// This indicates that p_root is a leaf (or only has one child),
	// so the new node will be inserted using the return value.
	//
	// The other reason for pass-by-value, followed by an assignment,
	// is that the recursive call may perform a rotation, so the
	// pointer that gets passed in may end up not being the root of
	// the subtree once the recursion returns.
	//
	else {
		if (ref.key < p_root->ref.key) {
			p_root->p_left = insert_rec(p_root->p_left, ref);
		}
		else {
			p_root->p_right = insert_rec(p_root->p_right, ref);
		}
	}

	// If necessary, apply a rotation to get the correct representation
	// in the parent node as we're backing out of the recursion.  This
	// places the tree in a state where the parent can safely apply a
	// rotation to restore the required black/red balance of the tree.

	// Fix a right-leaning red node: this will assure that a 3-node is
	// the left child.
	if (node_is_red(p_root->p_right) && (false == node_is_red(p_root->p_left))) {
		p_root = rotate_left(p_root);
	}

	// Fix two reds in a row: this will rebalance a 4-node.
	if (node_is_red(p_root->p_left) && node_is_red(p_root->p_left->p_left)) {
		p_root = rotate_right(p_root);
	}

	// If we perform the color flip here, the tree is assembled as a
	// mapping of a 2-3 tree.
#if !defined(USE_234_TREE)
	// This color flip will effectively split 4-nodes on the way back
	// out of the tree.  By doing this here, there will be no 4-nodes
	// left in the tree after the insertion is complete.
	if (node_is_red(p_root->p_left) && node_is_red(p_root->p_right)) {
		color_flip(p_root);
	}
#endif

	// Return the new root of the subtree that was just updated,
	// since rotations may have changed the value of this pointer.
	return p_root;
=======
/////////////////////////////////////////////////////////////////////////////
//
//	Delete()
//сверка с https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
//не совпала
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
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
}

/////////////////////////////////////////////////////////////////////////////
//
//	Insert()
//
<<<<<<< HEAD
bool insert(TREE, void_ref_t ref)
{
	p_tree->data = insert_rec(p_tree->data, ref);

	// The root node of a red-black tree must be black.
	p_tree->data->is_red = false;

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
//	move_red_left()
//
//	This code assumes that either p_root or p_root->p_left are red.
//
static node_t* move_red_left(ROOT)
{
	// If both children are black, we turn these three nodes into a
	// 4-node by applying a color flip.
	color_flip(p_root);

	// But we may end up with a case where p_right has a red child.
	// Apply a pair of rotations and a color flip to make p_root a
	// red node, both of its children become black nodes, and p_left
	// becomes a 3-node.
	if ((NULL != p_root->p_right) && node_is_red(p_root->p_right->p_left)) {
		p_root->p_right = rotate_right(p_root->p_right);
		p_root         = rotate_left(p_root);

		color_flip(p_root);
	}

	return p_root;
}


/////////////////////////////////////////////////////////////////////////////
//
//	move_red_right()
//
//	This code assumes that either p_root or p_root->Right is a red node.
//
static node_t* move_red_right(ROOT)
{
	// Applying a color flip may turn p_root into a 4-node,
	// with both of its children being red.
	color_flip(p_root);

	// However, this may cause a situation where both of p_root's
	// children are red, along with p_root->p_left->p_left.  Applying a
	// rotation and a color flip will fix this special case, since
	// it makes p_root red and p_root's children black.
	if ((NULL != p_root->p_left) && node_is_red(p_root->p_left->p_left)) {
		p_root = rotate_right(p_root);

		color_flip(p_root);
	}

	return p_root;
}


/////////////////////////////////////////////////////////////////////////////
//
//	find_min()
//
//	Find the node under p_root that contains the smallest key value.
//
static node_t* find_min(ROOT)
{
	while (NULL != p_root->p_left) {
		p_root = p_root->p_left;
	}

	return p_root;
}


/////////////////////////////////////////////////////////////////////////////
//
//	fixup()
//
//	Apply fix-up logic during deletion.  Applying color flips and rotations
//	will preserve perfect black-link balance.  This will also fix any
//	right-leaning red nodes and eliminate 4-nodes when backing out of
//	recursive calls.
//
static node_t* fixup(ROOT)
{
	// Fix right-leaning red nodes.
	if (node_is_red(p_root->p_right)) {
		p_root = rotate_left(p_root);
	}

	// Detect if there is a 4-node that traverses down the left.
	// This is fixed by a right rotation, making both of the red
	// nodes the children of p_root.
	if (node_is_red(p_root->p_left) && node_is_red(p_root->p_left->p_left)) {
		p_root = rotate_right(p_root);
	}

	// Split 4-nodes.
	if (node_is_red(p_root->p_left) && node_is_red(p_root->p_right)) {
		color_flip(p_root);
	}

	return p_root;
}
/////////////////////////////////////////////////////////////////////////////
//
//	delete_min()
//
//	Delete the bottom node on the left spine while maintaining balance.
//	To do so, we maintain the invariant that the current node or its left
//	child is red.
//
//	The only reason this is a member function instead of a static function
//	like all of the other support functions is because it needs to call
//	free().
//
node_t* delete_min(ROOT)
{
	// If this node has no children, we're done.
	// Due to the arrangement of an LLRB tree, the node cannot have a
	// right child.
	if (NULL == p_root->p_left) {
		free(p_root);
		return NULL;
	}

	// If these nodes are black, we need to rearrange this subtree to
	// force the left child to be red.
	if ((false == node_is_red(p_root->p_left)) && (false == node_is_red(p_root->p_left->p_left))) {
		p_root = move_red_left(p_root);
	}

	// Continue recursing to locate the node to delete.
	p_root->p_left = delete_min(p_root->p_left);

	// Fix right-leaning red nodes and eliminate 4-nodes on the way up.
	// Need to avoid allowing search operations to terminate on 4-nodes,
	// or searching may not locate intended key.
	return fixup(p_root);
}

/////////////////////////////////////////////////////////////////////////////
//
//	delete_rec()
//
node_t* delete_rec(ROOT, const u_int32_t key)
{
	if (key < p_root->ref.key) {
		if (NULL != p_root->p_left) {
			// If p_root and p_root->p_left are black, we may need to
			// move p_right to become the left child if a deletion
			// would produce a red node.
			if ((false == node_is_red(p_root->p_left)) && (false == node_is_red(p_root->p_left->p_left))) {
				p_root = move_red_left(p_root);
			}

			p_root->p_left = delete_rec(p_root->p_left, key);
		}
	}
	else {
		// If the left child is red, apply a rotation so we make
		// the right child red.
		if (node_is_red(p_root->p_left)) {
			p_root = rotate_right(p_root);
		}

		// Special case for deletion of a leaf node.
		// The arrangement logic of LLRBs assures that in this case,
		// p_root cannot have a left child.
		if ((key == p_root->ref.key) && (NULL == p_root->p_right)) {
			free(p_root);
			return NULL;
		}

		// If we get here, we need to traverse down the right node.
		// However, if there is no right node, then the target key is
		// not in the tree, so we can break out of the recursion.
		if (NULL != p_root->p_right) {
			if ((false == node_is_red(p_root->p_right)) && (false == node_is_red(p_root->p_right->p_left))) {
				p_root = move_red_right(p_root);
			}

			// Deletion of an internal node: We cannot delete this node
			// from the tree, so we have to find the node containing
			// the smallest key value that is larger than the key we're
			// deleting.  This other key will replace the value we're
			// deleting, then we can delete the node that previously
			// held the key/value pair we just moved.
			if (key == p_root->ref.key) {
				p_root->ref    = find_min(p_root->p_right)->ref;
				p_root->p_right = delete_min(p_root->p_right);
			}
			else {
				p_root->p_right = delete_rec(p_root->p_right, key);
			}
		}
	}

	// Fix right-leaning red nodes and eliminate 4-nodes on the way up.
	// Need to avoid allowing search operations to terminate on 4-nodes,
	// or searching may not locate intended key.
	return fixup(p_root);
}


/////////////////////////////////////////////////////////////////////////////
//
//	Delete()
//
void delete(TREE, const u_int32_t key)
{
	if (NULL != p_tree->data) {
		p_tree->data = delete_rec(p_tree->data, key);

		// Assuming we have not deleted the last node from the tree, we
		// need to force the root to be a black node to conform with the
		// the rules of a red-black tree.
		if (NULL != p_tree->data) {
			p_tree->data->is_red = false;
		}
	}
}

=======
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
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b



/////////////////////////////////////////////////////////////////////////////
//
//	sanity_check_rec()
//
<<<<<<< HEAD
//	Verifies that the red-black tree is properly formed.
//
=======
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
void sanity_check_rec(ROOT, bool isParentRed, u_int32_t blackDepth, u_int32_t *minBlack, u_int32_t *maxBlack)
{
	if (p_root->is_red) {
		// The parent of a red node must be black.
		printf("%d %s\n",__LINE__,(false == isParentRed)?"ok":"assert");

	}
	else {
		++blackDepth;
	}

<<<<<<< HEAD
=======
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

>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
	// The child of a red node must be black.
	if (isParentRed) {
		printf("%d %s\n",__LINE__,(false == p_root->is_red)?"ok":"assert");
	}

<<<<<<< HEAD
	// Only apply this test if the LLRB is arranged as a 2-3 tree.
	// Otherwise it is a 2-3-4 tree, in which case this rule does
	// not apply.
#if !defined(USE_234_TREE)
	// If the node has two children, only one of them may be red.
	// The other must be black.
	if ((NULL != p_root->p_left) && (NULL != p_root->p_right)) {
		if (node_is_red(p_root->p_left)) {
			printf("%d %s\n",__LINE__,(false == node_is_red(p_root->p_right))?"ok":"assert");
		}
		if (node_is_red(p_root->p_right)) {
			printf("%d %s\n",__LINE__,(false == node_is_red(p_root->p_left))?"ok":"assert");
		}
	}
#endif

	if (NULL != p_root->p_left) {
		// The left child must come before this node in sorting order.
		printf("%d %s\n",__LINE__,(p_root->p_left->ref.key < p_root->ref.key)?"ok":"assert");

		sanity_check_rec(p_root->p_left, p_root->is_red, blackDepth, minBlack, maxBlack);
=======
	if (NULL != p_root->p_child[0]) {
		// The left child must come before this node in sorting order.
		printf("%d %s\n",__LINE__,(p_root->p_child[0]->ref.key < p_root->ref.key)?"ok":"assert");

		sanity_check_rec(p_root->p_child[0], p_root->is_red, blackDepth, minBlack, maxBlack);
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}

<<<<<<< HEAD
	if (NULL != p_root->p_right) {
		// The right child must come after this node in sorting order.
		printf("%d %s\n",__LINE__,(p_root->p_right->ref.key > p_root->ref.key)?"ok":"assert");

		sanity_check_rec(p_root->p_right, p_root->is_red, blackDepth, minBlack, maxBlack);
=======
	if (NULL != p_root->p_child[1]) {
		// The right child must come after this node in sorting order.
		printf("%d %s\n",__LINE__,(p_root->p_child[1]->ref.key > p_root->ref.key)?"ok":"assert");

		sanity_check_rec(p_root->p_child[1], p_root->is_red, blackDepth, minBlack, maxBlack);
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}
}

<<<<<<< HEAD

=======
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
/////////////////////////////////////////////////////////////////////////////
//
//	SanityCheck()
//
<<<<<<< HEAD
=======
//	Verifies that the red-black tree is properly formed.
//
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
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


<<<<<<< HEAD
/////////////////////////////////////////////////////////////////////////////
//
//	key_count_rec()
=======


/////////////////////////////////////////////////////////////////////////////
//
//	KeyCountRec()
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
//
u_int32_t key_count_rec(ROOT)
{
	u_int32_t tally = 0;

	if (NULL != p_root) {
		tally += 1;

<<<<<<< HEAD
		if (NULL != p_root->p_left) {
			tally += key_count_rec(p_root->p_left);
		}

		if (NULL != p_root->p_right) {
			tally += key_count_rec(p_root->p_right);
=======
		if (NULL != p_root->p_child[0]) {
			tally += key_count_rec(p_root->p_child[0]);
		}

		if (NULL != p_root->p_child[1]) {
			tally += key_count_rec(p_root->p_child[1]);
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
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

<<<<<<< HEAD



/////////////////////////////////////////////////////////////////////////////
//
//	leaf_depth_rec()
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
	if ((NULL == p_root->p_left) && (NULL == p_root->p_right)) {
        *minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
=======
/////////////////////////////////////////////////////////////////////////////
//
//	LeafDepthRec()
//
u_int32_t leaf_depth_rec(ROOT, u_int32_t depth, u_int32_t ary[], u_int32_t depthLimit, u_int32_t blackDepth, u_int32_t *minBlack, u_int32_t *maxBlack)
{
    p("enter %p[%p,%p] depth %d blackDepth %d\n",p_root,p_root->p_child[0],p_root->p_child[1], depth, blackDepth)
	// Increment the count of nodes at the current depth of the tree.
	ary[depth] += 1;
   // p("current count: %d\n",ary[depth]);
    //  Если это черный узел, увеличьте количество черных узлов на этом пути, начиная с корня.
	if (false == p_root->is_red) {
        //p("inc blackDepth\n")
		++blackDepth;
	}

    //Если это листовой узел, верните общую глубину этого узла. 
	if ((NULL == p_root->p_child[0]) && (NULL == p_root->p_child[1])) {
        //p("leaf depth %d\n\n", depth)
        *minBlack = Min((u_int32_t)(*minBlack), (u_int32_t)blackDepth);
		*maxBlack = Max((u_int32_t)(*maxBlack), (u_int32_t)blackDepth);
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
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

<<<<<<< HEAD
	if (NULL != p_root->p_left) {
		d1 = leaf_depth_rec(p_root->p_left, depth + 1, ary, depthLimit, blackDepth, minBlack, maxBlack);
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}

	if (NULL != p_root->p_right) {
		d2 = leaf_depth_rec(p_root->p_right, depth + 1, ary, depthLimit, blackDepth, minBlack, maxBlack);
	}
	else {
		*minBlack = Min(*minBlack, blackDepth);
		*maxBlack = Max(*maxBlack, blackDepth);
	}

	// Return the maximum node depth found along this path from the root.
=======
    //p("child1\n");
	if (NULL != p_root->p_child[0]) {
       	//p("in: %p leaf_depth_rec(%p, %d, %p, %d, %d, %d, %d)\n",p_root,
		//    p_root->p_child[0], depth + 1, ary, depthLimit, blackDepth, *minBlack, *maxBlack
        //        );

		d1 = leaf_depth_rec(p_root->p_child[0], depth + 1, ary, depthLimit, blackDepth, minBlack, maxBlack);
		//p("out: %p d1: %d = leaf_depth_rec(%p, %d, %p, %d, %d, %d, %d)\n",p_root,d1,
		//    p_root->p_child[0], depth + 1, ary, depthLimit, blackDepth, *minBlack, *maxBlack
        //        );
	}
	else {
        //p("!\n");
        //p("in: minBlack %d maxBlack %d blackdepth %d ",*minBlack, *maxBlack, blackDepth);
		*minBlack = Min((u_int32_t)(*minBlack), (u_int32_t)blackDepth);
		*maxBlack = Max((u_int32_t)(*maxBlack), (u_int32_t)blackDepth);
        p("out: minBlack %d maxBlack %d\n",*minBlack, *maxBlack);

	}
    //p("child2\n");
	if (NULL != p_root->p_child[1]) {
	    //p("in: %p leaf_depth_rec(%p, %d, %p, %d, %d, %d, %d)\n",p_root, 
		//    p_root->p_child[1], depth + 1, ary, depthLimit, blackDepth, *minBlack, *maxBlack
        //        );
        d2 = leaf_depth_rec(p_root->p_child[1], depth + 1, ary, depthLimit, blackDepth, minBlack, maxBlack);
	    //p("out: %p d2: %d = leaf_depth_rec(%p, %d, %p, %d, %d, %d, %d)\n",p_root, d2,
		//    p_root->p_child[1], depth + 1, ary, depthLimit, blackDepth, *minBlack, *maxBlack
        //        );

    }
	else {
        //p("!\n");
		*minBlack = Min((u_int32_t)(*minBlack), (u_int32_t)blackDepth);
		*maxBlack = Max((u_int32_t)(*maxBlack), (u_int32_t)blackDepth);
        p("out: minBlack %d maxBlack %d\n",*minBlack, *maxBlack);
	}

    //Возвращает максимальную глубину узла, найденную на этом пути от корня. 
    p("%p ret: Max: %d d1: %d, d2: %d\n\n",p_root, Max(d1,d2), d1,d2);
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
	return Max(d1, d2);
}

/////////////////////////////////////////////////////////////////////////////
//
//	LeafDepth()
//
<<<<<<< HEAD
//	Tests the "black depth" of every leaf node.  This measure is the number
//	of black nodes between each leaf and the root (including the root, since
//	the root of a red-black tree must be black).  The tree depth of every
//	leaf node will be greater, since red nodes are not included in this
//	measure.
//
//	Once complete, it will report the minimum and maximum number of black
//	nodes found for each leaf.  Due to the nature of a red-black tree, these
//	two values must be the same.
=======
// Проверяет "глубину черного" каждого узла листа.  Эта мера представляет собой количество  чёрных узлов между каждым листом и корнем (включая корень, так как   так как корень красно-черного дерева должен быть черным).  Глубина дерева для каждого   листового узла будет больше, так как красные узлы не учитываются в этом   измерение.
//    По окончании работы будет сообщено минимальное и максимальное количество черных    узлов, найденных для каждого листа.  В силу природы красно-черного дерева эти   эти два значения должны быть одинаковыми.
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
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
<<<<<<< HEAD
		tally += ary[i];
=======
		tally += ary[i]; //подсчеты
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b

		printf("%3d: %5d = %5d\n", i, ary[i], tally);
	}

	printf("black depth: %d to %d\n", minBlack, maxBlack);

<<<<<<< HEAD
	// All leaf nodes must have the same number of black nodes between the
	// root and each leaf.  Therefore the minimum and maximum number of black
	// nodes must be the same.
	printf("%d %s\n",__LINE__,(minBlack == maxBlack)?"ok":"assert");
}


/////////////////////////////////////////////////////////////////////////////
//
//	traverse_rec()
//
void traverse_rec(ROOT, u_int32_t *prev)
{
	printf("%d %s\n",__LINE__,(NULL != p_root)?"ok":"assert");

	if (NULL != p_root->p_left) {
		traverse_rec(p_root->p_left, prev);
	}

	printf("%d %s\n",__LINE__,(*prev < p_root->ref.key)?"ok":"assert");
	*prev = p_root->ref.key;
	printf("%p key: %4d\n",p_root, p_root->ref.key);

	if (NULL != p_root->p_right) {
		traverse_rec(p_root->p_right, prev);
	}
}



=======
/*
    Все узлы листьев должны иметь одинаковое количество черных узлов между
  корнем и каждым листом.  Поэтому минимальное и максимальное количество черных
 узлов должно быть одинаковым.
 */
	printf("%d %s\n",__LINE__,(minBlack == maxBlack)?"ok":"assert");
}

/////////////////////////////////////////////////////////////////////////////
//
//	TraverseRec()
//
//void traverse_rec(ROOT, u_int32_t *prev)
void traverse_rec(ROOT)
{
    if (NULL != p_root->p_child[0]) {
		//traverse_rec(p_root->p_child[0], prev);
		traverse_rec(p_root->p_child[0]);
	}

	//*prev = p_root->ref.key;
	printf("%4d", p_root->ref.key);

	if (NULL != p_root->p_child[1]) {
		//traverse_rec(p_root->p_child[1], prev);
		traverse_rec(p_root->p_child[1]);
	}
}

>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
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
<<<<<<< HEAD
		u_int32_t prev = 0;
		traverse_rec(p_tree->data, &prev);
		printf("\n\n");
	}
}




=======
		//u_int32_t prev = 0;
		//traverse_rec(p_tree->data, &prev);
		traverse_rec(p_tree->data);
		printf("\n\n");
	}
}
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
#endif
