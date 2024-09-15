#ifndef interface_c
#define interface_c
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>

#define TREE treerbb_t *p_tree
#define PTREE treerbb_t **p_tree
#define ROOT node_t *p_root
#define SafeDelete(x)		{ if (NULL != (x)) { free(x);     (x) = NULL; } }

//#define DEBUG

#ifdef DEBUG
    #define p(...) printf("line %d ", __LINE__); printf(__VA_ARGS__);
#else
    #define p /**/
#endif

#define ArraySize(x) (sizeof(x) / (sizeof((x)[0])))
#define Max(x, y) ((x)>(y)) ? (x) : (y)
#define Min(x, y) ((x)>(y)) ? (y) : (x)

typedef struct _void_ref
{
	u_int32_t  key;
	void* p_context;
} void_ref_t;

typedef struct _node
{
	struct _node *p_child[2];

	void_ref_t ref;
    bool is_red;
} node_t;

typedef struct _treerbb
{
    node_t *data;
} treerbb_t;

void insert(TREE, void_ref_t ref);
void delete(TREE, const u_int32_t key);
void* lookup(TREE, const u_int32_t key);

#endif
