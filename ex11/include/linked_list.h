#ifndef   LINKED_LIST_H_
# define  LINKED_LIST_H_

typedef struct  node {
    char        *data;
    struct node *next;
}              node_t;

node_t  *list_create(char *data);

void    list_destroy(node_t **head, void (*fp)(char *data));

void    list_push(node_t *head, char *data);
void    list_unshift(node_t **head, char *data);

void    *list_pop(node_t **head);
void    *list_shift(node_t **head);
void    *list_remove(node_t **head, int pos);
void	  *list_rem(node_t *head, char *data);
void    list_print(node_t *head);
void    list_visitor(node_t *head, void (*fp)(char *data));
#endif /* LINKED_LIST_H_ */