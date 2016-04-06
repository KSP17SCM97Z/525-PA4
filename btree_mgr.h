#ifndef BTREE_MGR_H
#define BTREE_MGR_H

#include "dberror.h"
#include "tables.h"
#include "storage_mgr.h"
#include "buffer_mgr.h"

// structure for accessing btrees
typedef struct BTreeHandle {
  DataType keyType;
  int rootPage;
  int nodeNum;
  int entryNum;
  BM_BufferPool  *bufferPool;
  SM_FileHandle *fileHandle;
  char *idxId;
} BTreeHandle;

typedef struct BT_ScanHandle {
  BTreeHandle *tree;
  void *mgmtData;
} BT_ScanHandle;

// Add by Xincheng Yang 4/5/2016
typedef union BT_Element {
  int node;
  RID id;
} BT_Element;

// Add by Xincheng Yang 4/5/2016
typedef struct BT_Node {
  int parent;
  int current;
  int size;
  BT_Element *element;
} BT_Node;


// init and shutdown index manager
extern RC initIndexManager (void *mgmtData);
extern RC shutdownIndexManager ();

// create, destroy, open, and close an btree index
extern RC createBtree (char *idxId, DataType keyType, int n);
extern RC openBtree (BTreeHandle **tree, char *idxId);
extern RC closeBtree (BTreeHandle *tree);
extern RC deleteBtree (char *idxId);

// access information about a b-tree
extern RC getNumNodes (BTreeHandle *tree, int *result);
extern RC getNumEntries (BTreeHandle *tree, int *result);
extern RC getKeyType (BTreeHandle *tree, DataType *result);

// index access
extern RC findKey (BTreeHandle *tree, Value *key, RID *result);
extern RC insertKey (BTreeHandle *tree, Value *key, RID rid);
extern RC deleteKey (BTreeHandle *tree, Value *key);
extern RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle);
extern RC nextEntry (BT_ScanHandle *handle, RID *result);
extern RC closeTreeScan (BT_ScanHandle *handle);

// debug and test functions
extern char *printTree (BTreeHandle *tree);

// Add by Xincheng Yang 4/5/2016
extern RC getNode (BTreeHandle *tree, int nodeNum, BT_Node **node);
extern RC setNode (BTreeHandle *tree, int nodeNum, BT_Node *node);

#endif // BTREE_MGR_H
