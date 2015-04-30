/**************************************************\
 * Filename:     linkedList.c                     *
 * Author:       Sue Bogar                        *
 * Edited By:    Katherine Gibson (gibsonk@seas)  *
 * Date Written: 11/17/98                         *
 * EMail: bogar@cs.umbc.edu                       *
 *                                                *
 * Description:  This file contains the functions *
 * necessary to work with a linked list.          *
\**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

/********************************************
 ** Function: CreateNode
 ** Input:    none
 ** Output:  memory for a node is malloced
 **           and a pointer (nodePtr) to the 
 **           memory is returned to the user
 ********************************************/ 
NODEPTR CreateNode (void)
{
  NODEPTR newNode;

  /* get the space needed for the node */
  newNode = (NODEPTR) malloc(sizeof(NODE));

  if (newNode == NULL)
  {
    fprintf(stderr, "Out of memory in CreateNode function\n");
    exit(-1);
  }

  /* Initialize the members */
  newNode->next = NULL;

  return newNode;
}


/********************************************
 ** Function: SetData
 ** Input:    a pointer to a node (NODEPTR), and
 **           the value to be placed into the node
 ** Output:   none
 *********************************************/
void SetData (NODEPTR temp, int value)
{
  temp->data = value;
} 


/********************************************
 * Function: Insert
 * Input:    a pointer to a pointer to the head of the 
 *             linked list (headPtr)
 *           a pointer to the node (NODEPTR) to be 
 *             inserted
 * Output:   none
 ********************************************/ 
void Insert (NODEPTR* headPtr, NODEPTR temp)
{
  NODEPTR prev, curr;

  /* if list is empty, temp becomes first node */
  if ( *headPtr == NULL )
  {
    *headPtr = temp;
  }
  else
  {
    prev = NULL;
    curr = *headPtr;

    /* traverse the list until the end */
    while (curr != NULL)
    {
      prev = curr;
      curr = curr -> next;
    }
  
    /* insert temp at the end */
    prev -> next = temp;
  }
}


/********************************************
 * Function: Delete
 * Input:    a pointer to a pointer to the head 
 *             (headPtr) of the linked list
 *           an integer (target) containing the value 
 *             of the data in the node to be deleted
 * Output:   integer expressing operation success
 *            - if found, value found in DELETED node is returned
 *            - if not found, error is printed and -1 is returned
 ********************************************/ 
int Delete (NODEPTR* headPtr, int target)
{
  NODEPTR prev, curr;

  if (*headPtr == NULL)
  {
    printf ("Can't delete from an empty list\n");
    return (-1);
  }
  /* if node to delete is first in list, move head */
  else if (target == (*headPtr)->data)
  {
    curr = *headPtr;
    *headPtr = (*headPtr)->next;
    free (curr);
    return MATCH;
  }
  /* otherwise, traverse the list until 
     the end or the target value is found */
  else
  {
    prev = *headPtr;
    curr = (*headPtr)->next;

    while (curr != NULL && curr->data != target)
    {
      prev = curr;
      curr = curr -> next;
    }
    /* found the target, not the end of the list */
    if(curr != NULL)
    {
      /* delete the node that contains target value */
      prev->next = curr->next;
      free(curr);
      return MATCH;
    }
    else
    {
      printf("%d was not in the list\n", target);
      return (-1);
    }
  }      
}

/********************************************
 * Function: PrintList
 * Input:    a pointer to the head of the list
 * Ouput:    none
 ********************************************/ 
void PrintList (NODEPTR head)
{
  NODEPTR curr;

  if (head == NULL)
  {
    printf ("The list is empty\n");
  }
  else
  {
      
    curr = head;

    /* traverse the list until the end */
    while (curr != NULL)
    {
      /* print the current data item */
      printf("%d ", curr -> data);

      /* move to the next node */
      curr = curr -> next;
    }
    printf ("\n");
  }   
}

/********************************************
 * Function: Move
 * Input: 
 * Output:
 ********************************************/
int Move (NODEPTR* headPtrA, int target, NODEPTR* headPtrB)
{
  NODEPTR prev, curr;

  if (*headPtrA == NULL)
  {
    printf ("Can't move from an empty list\n");
    return (-1);
  }
  /* if node to move is first in list, move head */
  else if (target == (*headPtrA)->data)
  {
    curr = *headPtrA;
    *headPtrA = (*headPtrA)->next;
    curr->next = NULL;
    Insert(headPtrB, curr);
    return MATCH;
  }
  /* otherwise, traverse the list until 
     the end or the target value is found */
  else
  {
    prev = *headPtrA;
    curr = (*headPtrA)->next;

    while (curr != NULL && curr->data != target)
    {
      prev = curr;
      curr = curr -> next;
    }
    /* found the target, not the end of the list */
    if(curr != NULL)
    {
      /* delete the node that contains target value */
      prev->next = curr->next;
      curr->next = NULL;
      Insert(headPtrB, curr);
      return MATCH;
    }
    else
    {
      printf("The data was not in the list\n");
      return (-1);
    }
  }      
}

/********************************************
 * Function: DestroyList
 * Input:    a pointer to the head of the list
 * Output:   none
 * This frees the data and nodes of the list
 ********************************************/
void DestroyList (NODEPTR *headPtr)
{
  NODEPTR prev, curr, temp;

  if (*headPtr == NULL)
  {
    return;
  }
  /* traverse the list until 
     the end is found */
  else
  {
    prev = *headPtr;
    curr = (*headPtr)->next;

    while (curr != NULL)
    {
      prev->next = curr->next;
      temp = curr;
      curr = curr -> next;
      temp -> next = NULL;
      free(temp);
      temp = NULL;
    }
  }
}
