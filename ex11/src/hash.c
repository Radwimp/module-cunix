#include "../include/hash.h"
#include "../include/linked_list.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

hashtable_t     *hash_create(unsigned int size)
{
  hashtable_t *tbl;
  int i;

  if (size <= 0)
    return NULL;
  tbl = (hashtable_t *) malloc(sizeof(hashtable_t));
  tbl->size = size;
  tbl->table = (void **) malloc(sizeof(void *) * size);
  for (i = 0; i < size; i++)
    tbl->table[i] = NULL;
  return tbl;
}

void 			hash_destroy(hashtable_t *ht, void (*fp)(char *data))
{
  int i;
  node_t *tmp;
	
  for(i = 0; i < ht->size; i++)
	  if(ht->table[i] != NULL)
		{
			tmp = ht->table[i];
			list_destroy(&tmp, fp);
		}
	free(ht->table);
	ht->table = NULL;
	free(ht);
}

unsigned int 		hash_func(char *key)
{
  int i, res;

	if(key == NULL)
		return 0;
	res = 0;
	for(i = 0; *key != '\0'; i++, key++)
		res += *(key);
	return res;
}

void 			hash_set(hashtable_t *ht, char *key, void *ptr)
{
	unsigned int i;
  
  i = hash_func(key) % ht->size;
	if(ht->table[i] == NULL)
		ht->table[i] = list_create(ptr);	
	else 
		if(hash_get(ht, key) == NULL)
			list_push(ht->table[i], ptr);
		else 
		{

			list_rem(ht->table[i], hash_get(ht, key));
			list_push(ht->table[i], ptr);
		}
	
}


char 			*hash_get(hashtable_t *ht, char *key)
{
	int i;
  node_t *tmp;

  i = hash_func(key) % ht->size;
	if(ht->table[i] == NULL)
		return NULL;
	tmp = ht->table[i];
	return tmp->data;
}



