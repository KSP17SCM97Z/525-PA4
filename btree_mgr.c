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

RC findKey (BTreeHandle *tree, Value *key, RID *result){
}

// create, destroy, open, and close an btree index

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

RC createBtree (char *idxId, DataType keyType, int n){
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
 *04/052016	liuzhipeng	complete this function
***************************************************************/

RC getKeyType (BTreeHandle *tree, DataType *result)
{
	*result=tree->keyType;
}

// index access

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
RC findKey (BTreeHandle *tree, Value *key, RID *result)
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

RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle){
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

RC nextEntry (BT_ScanHandle *handle, RID *result){
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

RC closeTreeScan (BT_ScanHandle *handle){
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
