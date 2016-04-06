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
 *
***************************************************************/

RC createBtree (char *idxId, DataType keyType, int n){
    SM_FileHandle *fHandle = (SM_FileHandle *)calloc(1, sizeof(SM_FileHandle));
    char *metadata = (char *)calloc(1, PAGE_SIZE);
    char *read = (char *)malloc(PAGE_SIZE);

    int rootPage;
    int nodeNum;
    int entryNum;

    createPageFile(idxId);
    openPageFile(idxId, fHandle);
    ensureCapacity(2, fHandle);

    rootPage = 1;
    nodeNum = 0;
    entryNum = 0;

    memcpy(metadata, &rootPage, sizeof(int));
    memcpy(metadata + sizeof(int), &nodeNum, sizeof(int));
    memcpy(metadata + 2*sizeof(int), &entryNum, sizeof(int));
    memcpy(metadata + 3*sizeof(int), &entryNum, sizeof(int));

    writeBlock(0, fHandle, metadata);

    free(fHandle);
    free(metadata);

    return RC_OK;
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

RC openBtree (BTreeHandle **tree, char *idxId){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

RC closeBtree (BTreeHandle *tree){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

RC deleteBtree (char *idxId){
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
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

RC insertKey (BTreeHandle *tree, Value *key, RID rid){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

RC deleteKey (BTreeHandle *tree, Value *key){
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
	if(handle->currentPos == node->size && handle->count!=0)
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
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

char *printTree (BTreeHandle *tree){
}

// addition part

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/
RC getNode (BTreeHandle *tree, int nodeNum, BT_Node **node)
{

}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *
***************************************************************/

RC setNode (BTreeHandle *tree, int nodeNum, BT_Node *node)
{

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
	free(node->element);
	free(node);
	return RC_OK;
}
