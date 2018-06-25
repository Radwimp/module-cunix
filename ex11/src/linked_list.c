#include "../include/linked_list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

node_t  *list_create(char *data)
{
  node_t *head;
    
  head = (node_t *) malloc(sizeof(node_t));
  head->data = data;
  head->next = NULL;
  return head;
}

void    list_print(node_t *head)
{
  node_t *tmp;
    
  tmp = head;
  while (tmp->next != NULL)
  {
    printf(tmp->data);
    tmp = tmp->next;
  }
}

void    list_destroy(node_t **head, void (*fp)(char *data))
{
  if((*head) == NULL)
		return;
	fp((*head)->data);
	list_destroy(&(*head)->next, fp);
	free(*head);
}

void    list_push(node_t *head, char *data)
{
  node_t *cur_elem;

	if (head == NULL)
	{
		head = list_create(data);
		return;
	}
  cur_elem = head;
  while (cur_elem->next != NULL)
    cur_elem = cur_elem->next;
  cur_elem->next = (node_t *) malloc(sizeof(node_t));
  cur_elem->next->data = data;
  cur_elem->next->next = NULL;
}

void    list_unshift(node_t **head, char *data)
{
  node_t *tmp;
    
  tmp = (node_t *) malloc (sizeof(node_t));
  tmp->data = data;
  tmp->next = *head;
  *head = tmp;
}

void   *list_pop(node_t **head)
{
  node_t *cur_elem;
    
  cur_elem = *head;
  while (cur_elem->next->next != NULL)
    cur_elem = cur_elem->next;
  free(cur_elem->next);
  cur_elem->next = NULL;
}

void   *list_shift(node_t **head)
{
  node_t *tmp;

  tmp = *head;
  if (tmp->next == NULL)
	  *(head) = NULL;
  else *head = tmp->next;
  free(tmp);
}

void   *list_remove(node_t **head, int pos)
{
  node_t *cur_elem, *tmp;
	int i;

  if (pos == 1)
		return list_shift(head);
  cur_elem = *head;
  i = -1;
  while (++i < pos)
    cur_elem = cur_elem->next;
  tmp = cur_elem->next;
  cur_elem->next = tmp->next;
  free(tmp);
}

void	*list_rem(node_t *head, char *data) 
{
	node_t *cur_elem;
	int i = 1;
	
  cur_elem = head;
  while (strcmp(cur_elem->data,data) != 0)
  {
		cur_elem = cur_elem->next;
		i++;
	}
	list_remove(&(head), i);
}

void    list_visitor(node_t *head, void (*fp)(char *data))
{
 node_t *cur_elem;
 
 cur_elem = head;
 while (cur_elem->next != NULL)
 {
   fp(cur_elem->data);
   cur_elem = cur_elem->next;
 }
}

