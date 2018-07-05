#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct header_t
{
	size_t size;
	unsigned is_free;
	struct header_t *next;
};

struct header_t
{
  *head = NULL;
  *tail = NULL;
}

pthread_mutex_t global_malloc_lock;

struct header_t *get_free_block(size_t size)
{
	struct header_t *curr = head;
	while(curr)
  {
		if (curr->is_free && curr->size >= size)
			return curr;
		curr = curr->next;
	}
	return NULL;
}

void free(void *block)
{
	struct header_t *header, *tmp;
	void *programbreak;

	if (!block)
		return;
	pthread_mutex_lock(&global_malloc_lock);
	header = (struct header_t*)block - 1;
	programbreak = sbrk(0);
	if ((char*)block + header->size == programbreak)
  {
		if (head == tail)
			head = tail = NULL;
		else
    {
			tmp = head;
			while (tmp)
      {
				if(tmp->next == tail)
        {
					tmp->next = NULL;
					tail = tmp;
				}
				tmp = tmp->next;
			}
		}
		sbrk(0 - header->size - sizeof(struct header_t));
		pthread_mutex_unlock(&global_malloc_lock);
		return;
	}
	header->is_free = 1;
	pthread_mutex_unlock(&global_malloc_lock);
}

void *halloc(size_t size)
{
	size_t total_size;
	void *block;
	struct header_t *header;
	if (!size)
		return NULL;
	pthread_mutex_lock(&global_malloc_lock);
	header = get_free_block(size);
	if (header)
  {
		header->is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void*)(header + 1);
	}
	total_size = sizeof(struct header_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1)
  {
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->size = size;
	header->is_free = 0;
	header->next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->next = header;
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void*)(header + 1);
}
