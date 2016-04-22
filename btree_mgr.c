#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "buffer_mgr.h" 
#include "storage_mgr.h" 

#include "dberror.h" 
#include "btree_mgr.h" 
#include "tables.h" 
#include "expr.h" 

// init and shutdown index manager

/***************************************************************
 * Function Name: initIndexManager 
 * 
 * Description: initial index manager
 *
 * Parameters: void *mgmtData
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
 * History:
 *      Date            Name                        Content
 *      04/05/16        Xiaoliang Wu                Complete.
 *
***************************************************************/

RC initIndexManager (void *mgmtData){
    printf("------------------------ Initial Index Manager ------------------------ \n");
    return RC_OK;
}

/***************************************************************
 * Function Name: shutdownIndexManager 
 * 
 * Description: shutdown index manager
 *
 * Parameters:
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
 * History:
 *      Date            Name                        Content
 *      04/05/16        Xiaoliang Wu                Complete.
 *
***************************************************************/

RC shutdownIndexManager (){
    printf("------------------------ Shutdown Index Manager ------------------------ \n");
    return RC_OK;
}


// create, destroy, open, and close an btree index

/***************************************************************
 * Function Name: createBtree 
 * 
 * Description: create a btree file
 *
 * Parameters: char *idxId, DataType keyType, int n
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
 * History:
 *      Date            Name                        Content
 *      04/05/16        Xiaoliang Wu                Complete.
 *      04/09/16        Xincheng Yang               Modified.
 *
***************************************************************/

RC createBtree (char *idxId, DataType keyType, int n){
    SM_FileHandle *fHandle = (SM_FileHandle *)calloc(1, sizeof(SM_FileHandle));
    char *metadata = (char *)calloc(1, PAGE_SIZE);

    int rootPage;
    int nodeNum;
    int entryNum;

    createPageFile(idxId);
    openPageFile(idxId, fHandle);
    ensureCapacity(1, fHandle);

    rootPage = -1;
    nodeNum = 0;
    entryNum = 0;

    /*
     * root page number
     * the number of node
     * the number of entry
     * n
     * key type
     */
    memcpy(metadata, &rootPage, sizeof(int));
    memcpy(metadata + sizeof(int), &nodeNum, sizeof(int));
    memcpy(metadata + 2*sizeof(int), &entryNum, sizeof(int));
    memcpy(metadata + 3*sizeof(int), &n, sizeof(int));
    memcpy(metadata + 4*sizeof(int), &keyType, sizeof(DataType));

    writeBlock(0, fHandle, metadata);

    free(fHandle);
    free(metadata);

    return RC_OK;
}

/***************************************************************
 * Function Name: openBtree 
 * 
 * Description: open btree file
 *
 * Parameters: BTreeHandle **tree, char *idxId
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
 * History:
 *      Date            Name                        Content
 *      04/06/16        Xiaoliang Wu                Complete.
 *
***************************************************************/

RC openBtree (BTreeHandle **tree, char *idxId){

    DataType keyType;
    int rootPage;
    int nodeNum;
    int entryNum;
    int n;
    BM_BufferPool  *bufferPool = MAKE_POOL();
    SM_FileHandle *fHandle = (SM_FileHandle *)calloc(1, sizeof(SM_FileHandle));
 
    char * metadata = (char *)calloc(PAGE_SIZE, sizeof(char));

    openPageFile(idxId, fHandle);
    initBufferPool(bufferPool, idxId, 10, RS_LRU, NULL);

    readBlock(0, fHandle, metadata);

    memcpy(&rootPage, metadata,sizeof(int));
    memcpy(&nodeNum, metadata+sizeof(int),sizeof(int));
    memcpy(&entryNum, metadata+2*sizeof(int),sizeof(int));
    memcpy(&n, metadata+3*sizeof(int),sizeof(int));
    memcpy(&keyType, metadata + 4*sizeof(int),sizeof(DataType));

    *tree = (BTreeHandle *)calloc(1, sizeof(BTreeHandle));
    
    (*tree)->keyType = keyType;
    (*tree)->rootPage = rootPage;
    (*tree)->nodeNum = nodeNum;
    (*tree)->entryNum = entryNum;
    (*tree)->n = n;
    (*tree)->bufferPool = bufferPool;
    (*tree)->fileHandle = fHandle;
    (*tree)->idxId = idxId;

    free(metadata);

    return RC_OK;
}

/***************************************************************
 * Function Name: closeBtree 
 * 
 * Description: close btree
 *
 * Parameters: BTreeHandle *tree)
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
 * History:
 *      Date            Name                        Content
 *      04/06/16        Xiaoliang Wu                Complete.
 *
***************************************************************/

RC closeBtree (BTreeHandle *tree){

    shutdownBufferPool(tree->bufferPool);
    free(tree->bufferPool);
    free(tree->fileHandle);
    free(tree);
    return RC_OK;
}

/***************************************************************
 * Function Name: deleteBtree 
 * 
 * Description: delte btree file
 *
 * Parameters: char *idxId
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
 * History:
 *      Date            Name                        Content
 *      04/06/16        Xiaoliang Wu                Complete.
 *
***************************************************************/

RC deleteBtree (char *idxId){
    return destroyPageFile(idxId);
}

// access information about a b-tree

/***************************************************************
 * Function Name: getNumNodes
 * 
 * Description:get this number of nodes in the tree and return it in *result
 *
 * Parameters:BTreeHandle *tree, int *result
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/05/2016	liuzhipeng	complete this function
***************************************************************/

RC getNumNodes (BTreeHandle *tree, int *result)
{
	*result=tree->nodeNum;
    return RC_OK;
}

/***************************************************************
 * Function Name: getNumEntries
 * 
 * Description:get the number of entries of the tree and return it in *reslut
 *
 * Parameters:BTreeHandle *tree, int *result
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/05/2016	liuzhipeng	complete this function
***************************************************************/

RC getNumEntries (BTreeHandle *tree, int *result)
{
	*result=tree->entryNum;
    return RC_OK;
}

/***************************************************************
 * Function Name: getKeyType
 * 
 * Description:get the key type of tree
 *
 * Parameters:BTreeHandle *tree, DataType *result
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/05/2016	liuzhipeng	complete this function
***************************************************************/

RC getKeyType (BTreeHandle *tree, DataType *result)
{
	*result=tree->keyType;
    return RC_OK;
}

// index access

/***************************************************************
 * Function Name: findKey
 * 
 * Description:find a specific value from the tree and return it in *result
 *
 * Parameters:BTreeHandle *tree, Value *key, RID *result
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/05/2016	liuzhipeng	complete this function
***************************************************************/
RC findKey (BTreeHandle *tree, Value *key, RID *result)
{
	int n;
	n=tree->rootPage;
	BT_Node *node=NULL;
	while(1)
	{
		getNode (tree,n, &node);
		if(node->nodeType)
		{
			int i;
			for(i=1 ; i < node->size ; i=i+2)
			{
				if((node->element+i)->node == key->v.intV)
				{
					result->page=(node->element+i-1)->id.page;
					result->slot=(node->element+i-1)->id.slot;
					freeNode(node);
					return RC_OK;
				}
			}
			freeNode(node);
			return RC_IM_KEY_NOT_FOUND;
		}
		else
		{
			int i;
			if((node->element+1)->node > key->v.intV)
			{
				n=node->element->node;
			}
			else
			{
				for(i=1 ; i<node->size ; i=i+2)
				{
					if((node->element+i)->node<=key->v.intV)
					{
						n=(node->element+i+1)->node;
					}
				}
			}
		}
	}
}

/***************************************************************
 * Function Name: insertKey 
 * 
 * Description:insert key
 *
 * Parameters:BTreeHandle *tree, Value *key, RID rid
 *
 * Return:RC
 *
 * Author:Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *  04/10/2016          Xincheng Yang             finish this function
 *  04/14/2016          Xincheng Yang             modified
***************************************************************/

RC insertKey (BTreeHandle *tree, Value *key, RID rid){
    BT_Node *node = NULL, *newnode = NULL, *root = NULL, *point = NULL;
    int i, intKey, left, right, k=0, maxsize=2*(tree->n+2);
    BT_Element *element = (BT_Element *)malloc(maxsize * sizeof(BT_Element));
    
    // If root does not exist, create root.
    if(tree->entryNum==0 || tree->rootPage==-1 || getNode(tree, tree->rootPage, &node) != RC_OK){   
        node = (BT_Node *)calloc(1, sizeof(BT_Node));
        node->isValid = 1;
        node->parent = -1;
        node->current = tree->fileHandle->totalNumPages;
        appendEmptyBlock(tree->fileHandle);
        node->nodeType = 1;
        node->size = 2;
        node->element = (BT_Element *)malloc(sizeof(BT_Element) * maxsize);
        (*(node->element)).id = rid;
        (*(node->element+1)).node = key->v.intV;
        tree->nodeNum++;
        tree->rootPage = node->current;
        setNode(tree, node->current, node);
    } else {
        // Find the target node to insert
        while(node->nodeType == 0){     // While not leaf
            for(i=1; ; i+=2){
                if((*(node->element+i)).node > key->v.intV || i>=node->size){
                    k = (*(node->element+i-1)).node;
                    getNode(tree, k, &node);
                    break;
                }
            }
        }
        
        intKey = key->v.intV;
        do {
            // Insert rid and key into node
            for(i=1; i < node->size+4; i+=2){
                if((*(node->element+i)).node > intKey || i>=node->size){
                    if(i > 1 && (*(node->element+i-2)).node == intKey){
                        freeNode(node);
                        return RC_IM_KEY_ALREADY_EXISTS;
                    }
                    if(node->nodeType == 0){        // if not leaf.
                        printNode(node);
                        memcpy(element, node->element, sizeof(BT_Element) * (i-1));
                        (*(element+i-1)).node = left;
                        (*(element+i)).node = intKey;
                        (*(element+i+1)).node = right;
                        memcpy(element + i+2, node->element+ i, sizeof(BT_Element) * (node->size-i));
                    } else {                        // if leaf node
                        memcpy(element, node->element, sizeof(BT_Element) * (i-1));
                        (*(element+i-1)).id = rid;
                        (*(element+i)).node = intKey;
                        memcpy(element + i+1, node->element+ i-1, sizeof(BT_Element) * (node->size+1-i));
                    }
                    node->size+=2;
                    memcpy(node->element, element, sizeof(BT_Element) * node->size);
                    break;
                }
            }
            
            if(node->size >= 2*(tree->n+1)){       // If node overflow, then split into 2 nodes.
                if(newnode == NULL){
                    newnode = (BT_Node *)malloc(sizeof(BT_Node));
                    newnode->element = (BT_Element *)calloc(maxsize, sizeof(BT_Element));  
                }
                newnode->size = node->size/4*2 + node->size%2;
                newnode->isValid = 1;
                newnode->current = tree->fileHandle->totalNumPages;
                appendEmptyBlock(tree->fileHandle);
                newnode->nodeType = node->nodeType;
                newnode->parent = node->parent;
                memcpy(newnode->element, node->element + node->size - newnode->size, 
                    sizeof(BT_Element)*newnode->size);

                node->size = (node->size/2 - node->size/4)*2 + 1;
                (*(node->element+node->size-1)).node =  newnode->current;   //Point to new node.

                tree->nodeNum++; 
                if(node->parent == -1){     // Create new root if corrent node do not have parent
                    left = node->current;  
                    if(root == NULL){
                        root = (BT_Node *)malloc(sizeof(BT_Node));
                        root->size = 3;
                        root->isValid = 1;
                        root->parent = -1;
                        root->nodeType = 0;
                        root->element = (BT_Element *)malloc(sizeof(BT_Element) * 3);
                    }

                    root->current = tree->fileHandle->totalNumPages;
                    appendEmptyBlock(tree->fileHandle);
                    (*(root->element)).node = left;
                    (*(root->element+1)).node = (*(newnode->element+1)).node;
                    (*(root->element+2)).node = newnode->current;
                    
                    tree->rootPage = root->current;
                    tree->nodeNum++;
                    node->parent = root->current;
                    newnode->parent = root->current;
                    setNode(tree, node->current, node);
                    setNode(tree, newnode->current, newnode);
                    setNode(tree, root->current, root);
                    break;
                } else {                    // Insert into root if corrent node have parent
                    setNode(tree, node->current, node);
                    setNode(tree, newnode->current, newnode);
                    intKey = (*(newnode->element+1)).node;
                    left = node->current;      
                    right = newnode->current;
                    getNode(tree, node->parent, &node);
                }
            } else {                        // If node not overflow, then save node and break loop
                setNode(tree, node->current, node);
                break;
            }
        } while (true);
    }
    
    tree->entryNum++;
    free(element);
    freeNode(node);
    freeNode(newnode);
    freeNode(root);
    return RC_OK;
}

/***************************************************************
 * Function Name: deleteKey 
 * 
 * Description:delete key
 *
 * Parameters:BTreeHandle *tree, Value *key
 *
 * Return:RC
 *
 * Author:Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *  04/17/2016          Xincheng Yang             finish this function
***************************************************************/

RC deleteKey (BTreeHandle *tree, Value *key){
    int i;
    BT_Node *node=NULL, *child=NULL, *sibling=NULL;
    
    if(tree->rootPage==-1 || getNode(tree, tree->rootPage, &node) != RC_OK){ 
        freeNode(node);
        return RC_IM_KEY_NOT_FOUND;
    }
    // Find the node
    while(node->nodeType == 0){     // While not leaf
        for(i=1; ; i+=2){
            if((*(node->element+i)).node > key->v.intV || i>=node->size){
                getNode(tree, (*(node->element+i-1)).node, &node);
                break;
            }
        }
    }
    // Find the key to delete
    for(i=1; i<node->size; i+=2){
        if((*(node->element+i)).node > key->v.intV){
            break;
        }
    }
    i-=2;
    if((*(node->element+i)).node != key->v.intV){       // If key do not exist
        freeNode(node);
        return RC_IM_KEY_NOT_FOUND;
    }

    // Delete key
    for(;i<node->size;i+=2){
        (*(node->element+i-1)).id = (*(node->element+i+1)).id;
        (*(node->element+i)).node = (*(node->element+i+2)).node;
    }
    node->size-=2;
    tree->entryNum--;
    while(node->size <2 && node->current!=0){       // Remove from parent
        getNode(tree, node->current, &child);
        child->isValid = 0;
        getNode(tree, node->parent, &node);

        for(i=0; i<node->size; i+=2){
            if((*(node->element+i)).node == child->current){
                break;
            }
        }
        if(i == 0){
            for(;i+2<node->size;i++){
                (*(node->element+i)).node = (*(node->element+i+2)).node;
            }
        } else {
            i--;
            if(child->size == 1 && child->nodeType == 1){       // Sibling point to next
                getNode(tree, (*(node->element+i-1)).node, &sibling);
                (*(node->element+sibling->size-1)).node = (*child->element).node;
                setNode(tree, sibling->current, sibling);
            }
            for(;i+2<node->size;i++){
                (*(node->element+i)).node = (*(node->element+i+2)).node;
            }
        }
        node->size-=2;
        setNode(tree, child->current, child); 
        tree->nodeNum--;
    } 
    
    setNode(tree, node->current, node);
    freeNode(sibling);
    freeNode(child);
    freeNode(node);
    return RC_OK;
}

/***************************************************************
 * Function Name: openTreeScan 
 * 
 * Description:initialize scan handle
 *
 * Parameters:BTreeHandle *tree, BT_ScanHandle **handle
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/06/2016	liuzhipeng	complete this function
***************************************************************/

RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle)
{
	*handle=(BT_ScanHandle*)calloc(1,sizeof(BT_ScanHandle));
	(*handle) ->tree=tree;
	int n;
	BT_Node *node=NULL;
	n=tree->rootPage;
	 getNode (tree, n, &node);
	 while(node->nodeType !=1)
	 {
	 	n=node->element->node;
	 	getNode(tree,n,&node);
	 }
	 (*handle)->currentNode=node->current;
	 (*handle)->currentPos=0;
	 getNumEntries (tree, &n);
	 (*handle)->count=n;
	 freeNode(node);

	 return RC_OK;
}

/***************************************************************
 * Function Name: nextEntry 
 * 
 * Description:return the next sorted RID in *result
 *
 * Parameters:BT_ScanHandle *handle, RID *result
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/06/2016	liuzhipeng	complete this function
***************************************************************/

RC nextEntry (BT_ScanHandle *handle, RID *result)
{
	if(handle->count <= 0)
		return RC_IM_NO_MORE_ENTRIES;
	BT_Node *node=NULL;
	getNode (handle->tree, handle->currentNode, &node);
	result->page=(node->element+handle->currentPos)->id.page;
	result->slot=(node->element+handle->currentPos)->id.slot;
	handle->count--;
	handle->currentPos=handle->currentPos+2;
	if(handle->currentPos == node->size -1 && handle->count!=0)
	{
		handle->currentNode=(node->element+handle->currentPos)->node;
		handle->currentPos=0;
	}
	freeNode(node);
	return RC_OK;	
}

/***************************************************************
 * Function Name: closeTreeScan 
 * 
 * Description:close a scan handle
 *
 * Parameters:BT_ScanHandle *handle
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/06/2016	liuzhipeng	complete this function
***************************************************************/

RC closeTreeScan (BT_ScanHandle *handle)
{
	free(handle);

	return RC_OK;
}

// debug and test functions

/***************************************************************
 * Function Name: printNode 
 * 
 * Description:print node
 *
 * Parameters:BT_Node *node
 *
 * Return: RC
 *
 * Author:Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *  04/10/2016          Xincheng Yang             finish this function
***************************************************************/
RC printNode(BT_Node *node){
    int j=0;
    printf("Node(%d):", node->current);
    if(node->nodeType == 0){
        for(j=0; j<node->size; j++){
            printf("%d ", (*(node->element+j)).node);
        }
        printf("\n");
    } else {
        for(j=0; j<node->size;j++){
            if(j%2 == 0 && j!=node->size-1){
                printf("%d.%d ", (*(node->element+j)).id.page, (*(node->element+j)).id.slot);
            } else {
                printf("%d ", (*(node->element+j)).node);
            }
        }
        printf("\n");
    }
    return RC_OK;
}

/***************************************************************
 * Function Name: printTree 
 * 
 * Description:print tree
 *
 * Parameters:BTreeHandle *tree
 *
 * Return: RC
 *
 * Author:Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *  04/9/2016          Xincheng Yang             finish this function
***************************************************************/
char *printTree (BTreeHandle *tree){
    BT_Node *node = NULL;
    int i=0, j, size=1, arr[100];
    
    if(tree->rootPage == -1){
        return "Root not exist";
    }
    printf("\nThe tree is\n");
    arr[0] = tree->rootPage;
    while(i!=size){
        getNode(tree, arr[i], &node);
        printf("(%d)[", arr[i]);
        if(node->nodeType == 0){
            for(j=0; j<node->size; j++){
                printf("%d ", (*(node->element+j)).node);
                if(j%2==0){
                    // if(node->current == tree->rootPage || node->size%2 == 0 || j!=node->size-1){
                    //     arr[size] = (*(node->element+j)).node;
                    //     size++;
                    // }
                    arr[size] = (*(node->element+j)).node;
                    size++;
                }
            }
        } else {
            for(j=0; j<node->size;j++){
                if(j%2 == 0 && j!=node->size-1){
                    printf("%d.%d ", (*(node->element+j)).id.page, (*(node->element+j)).id.slot);
                } else {
                    printf("%d ", (*(node->element+j)).node);
                }
            }
        }
        printf("]\n");
        i++;
    }
    freeNode(node);
}

// addition part

/***************************************************************
 * Function Name: getNode 
 * 
 * Description:get node from tree
 *
 * Parameters:BTreeHandle *tree, int nodeNum, BT_Node **node
 *
 * Return: RC
 *
 * Author:Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *  04/9/2016          Xincheng Yang             finish this function
***************************************************************/
RC getNode (BTreeHandle *tree, int nodeNum, BT_Node **node)
{
    BM_PageHandle *pg = (BM_PageHandle *)calloc(1, sizeof(BM_PageHandle));
    RC rv = RC_OK;
    bool isNull = false;
    int maxsize=2*(tree->n+2);
    
    if(*node == NULL){
        isNull = true;
        *node = (BT_Node *)calloc(1, sizeof(BT_Node));
    } 

    if((rv = pinPage(tree->bufferPool, pg, nodeNum)) == RC_OK){
        memcpy(&(*node)->isValid, pg->data, sizeof(int));
        if((*node)->isValid == 0){
            unpinPage(tree->bufferPool, pg);
            rv = RC_IM_NODE_NOT_EXIST;
        } else {
            memcpy(&(*node)->parent, pg->data + sizeof(int), sizeof(int));
            memcpy(&(*node)->current, pg->data + 2*sizeof(int) , sizeof(int));
            memcpy(&(*node)->size, pg->data + 3*sizeof(int), sizeof(int));
            memcpy(&(*node)->nodeType, pg->data + 4*sizeof(int), sizeof(int));
            if(isNull || (*node)->element == NULL){
                (*node)->element = (BT_Element *)malloc(sizeof(BT_Element) * maxsize);
            } 
            memcpy((*node)->element, pg->data + 5*sizeof(int), sizeof(BT_Element) * (*node)->size);
            rv = unpinPage(tree->bufferPool, pg);
        }
    }
    
    free(pg);
    return rv;
}

/***************************************************************
 * Function Name: setNode 
 * 
 * Description:set node into tree
 *
 * Parameters:BTreeHandle *tree, int nodeNum, BT_Node *node
 *
 * Return: RC
 *
 * Author:Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *  04/9/2016          Xincheng Yang             finish this function
***************************************************************/

RC setNode (BTreeHandle *tree, int nodeNum, BT_Node *node)
{
    BM_PageHandle *pg = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    RC rv = RC_OK;
    
    if( (rv = ensureCapacity(nodeNum+1, tree->fileHandle)) == RC_OK 
        && (rv = pinPage(tree->bufferPool, pg, nodeNum)) == RC_OK){
        printf("[SetNode]Set node 1\n"); 
        memcpy(pg->data, &node->isValid, sizeof(int));
        memcpy(pg->data + sizeof(int), &node->parent, sizeof(int));
        memcpy(pg->data + 2*sizeof(int), &node->current, sizeof(int));
        memcpy(pg->data + 3*sizeof(int), &node->size, sizeof(int));
        memcpy(pg->data + 4*sizeof(int), &node->nodeType, sizeof(int));
        memcpy(pg->data + 5*sizeof(int), node->element, sizeof(BT_Element) * node->size);
        
        markDirty(tree->bufferPool, pg);
        rv = unpinPage(tree->bufferPool, pg);
    }
    free(pg);
    return rv;
}

/***************************************************************
 * Function Name: freeNode
 * 
 * Description:free the memospace of Node struct
 *
 * Parameters:BT_Node *node
 *
 * Return:RC
 *
 * Author:lzp
 *
 * History:
 *      Date            Name                        Content
 *04/05/2016	liuzhipeng	complete this fuction
***************************************************************/
RC freeNode (BT_Node *node)
{
    if(node != NULL){
        if(node->element != NULL){
            free(node->element);
            node->element = NULL;
        }
    	free(node);
        node = NULL;
    }
	return RC_OK;
}
