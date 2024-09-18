#ifndef public_c
#define public_c
#include <interface.c>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bool new(PTREE)
{
    *p_tree = malloc(sizeof(treerbll_t));
    if(NULL !=*p_tree)
    {
        memset(*p_tree, 0, sizeof(treerbll_t));
        return true;
    }
    return false;
}

void free_node(ROOT)
{
    p("free node %p\n", p_root);
	if (NULL != p_root) {
		if (NULL != p_root->p_left) {
			free_node(p_root->p_left);
		}
		if (NULL != p_root->p_right) {
			free_node(p_root->p_right);
		}
		SafeDelete(p_root);
	}
}

node_t* new_node(void)
{
	node_t *pNew = malloc(sizeof(node_t));

	pNew->ref.key      = 0;
	pNew->ref.p_context = NULL;
	pNew->is_red        = true;
	pNew->p_left        = NULL;
	pNew->p_right       = NULL;

	return pNew;
}

void destructor(PTREE)
{
    p("destructor enter\n");
    p("destructor: %p\n", (*p_tree));
    free_node((*p_tree)->data);
    free(*p_tree);
    p_tree = NULL;
}

void print_rec(ROOT, char* end, char* start)
{
    char *pr = (end-3<start)?"...":end-3;
    printf("%s %p color: %5s, [%d-%s]\n", pr,  p_root, (p_root->is_red)?"red":"black", p_root->ref.key, (char*)p_root->ref.p_context);
    if(NULL != p_root->p_left)
        print_rec(p_root->p_left, pr, start);
    if(NULL != p_root->p_right)
        print_rec(p_root->p_right, pr, start);
}

void print(TREE)
{
    static char prefix[] = "-----------------------------------------------------------------------";
    if(NULL != p_tree && NULL !=p_tree->data)
	    print_rec(p_tree->data, &prefix[sizeof(prefix)-2], &prefix[0]);
}

#endif
