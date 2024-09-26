#ifndef interface_c
#define interface_c
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>

<<<<<<< HEAD
#define TREE treerbll_t *p_tree
#define PTREE treerbll_t **p_tree
=======
#define TREE treerbb_t *p_tree
#define PTREE treerbb_t **p_tree
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
#define ROOT node_t *p_root
#define SafeDelete(x)		{ if (NULL != (x)) { free(x);     (x) = NULL; } }

#define DEBUG

#ifdef DEBUG
    #define p(...) printf("line %d ", __LINE__); printf(__VA_ARGS__);
#else
    #define p(...) /*__LINE__ __VA_ARGS__*/
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
<<<<<<< HEAD
	struct _node *p_right;
    struct _node *p_left;
=======
	struct _node *p_child[2];
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b

	void_ref_t ref;
    bool is_red;
} node_t;

<<<<<<< HEAD
typedef struct _treerbll
{
    node_t *data;
} treerbll_t;


bool insert(TREE, void_ref_t ref);
=======
typedef struct _treerbb
{
    node_t *data;
} treerbb_t;

void insert(TREE, void_ref_t ref);
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
void delete(TREE, const u_int32_t key);
void* lookup(TREE, const u_int32_t key);

#endif
