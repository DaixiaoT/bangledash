#ifndef _LIST_H_
#define _LIST_H_

// 插入一个obj到双向链表头部
#define list_add_head(head, obj)               \
	if((head)==0) {                            \
		(head) = (obj);                        \
		(obj)->prev = (obj)->next = (obj);    \
	}                                          \
	else {                                     \
		(obj)->next = (head);                  \
		(obj)->prev = (head)->prev;            \
		(head)->prev = (obj);                  \
		(obj)->prev->next = (obj);             \
		(head) = (obj);                        \
	}

// 插入一个obj到双向链表尾部
#define list_add_tail(head, obj)               \
	if((head)==0) {                            \
		(head) = (obj);                        \
		(obj)->prev = (obj)->next = (obj);     \
	}                                          \
	else {                                     \
		(head)->prev->next = (obj);            \
		(obj)->prev = (head)->prev;            \
		(obj)->next = (head);                  \
		(head)->prev = (obj);                  \
	}

// 插入一个newobj到双向链表中obj之前
/* obj, newobj必须不为空 */
#define list_insert_before(head, obj, newobj)  \
		(newobj)->next = (obj);                \
		(newobj)->prev = (obj)->prev;          \
		(obj)->prev    = (newobj);             \
		(newobj)->prev->next = (newobj);       \
		if( (head) == (obj) )                  \
			(head) = (newobj);

// 插入一个newobj到双向链表中obj之后
#define list_insert_after(head, obj, newobj)   \
		(newobj)->prev = (obj);                \
		(newobj)->next = (obj)->next;          \
		(obj)->next    = (newobj);             \
		(newobj)->next->prev = (newobj);

// 从双向链表中删除obj对象
#define list_remove(head, obj)                 \
	if((obj)->next==(obj)) {                   \
		(obj)->prev = (obj)->next = 0;         \
		(head) = 0;                            \
	}                                          \
	else {                                     \
		if((head)==(obj))                      \
			(head) = (obj)->next;              \
		(obj)->next->prev = (obj)->prev;       \
		(obj)->prev->next = (obj)->next;       \
		(obj)->prev = (obj)->next = 0;         \
	}

// 从双向链表头部移出一个对象，放到obj中
#define list_remove_head(head, obj)            \
	if((head)==0) {                            \
		(obj) = 0;                             \
	}                                          \
	else                                       \
	if((head)->next == (head)) {               \
		(obj) = (head);                        \
		(obj)->prev = (obj)->next = 0;         \
		(head) = 0;                            \
	}                                          \
	else {                                     \
		(obj) = (head);                        \
		(head)->next->prev = (head)->prev;     \
		(head)->prev->next = (head)->next;     \
		(head) = (obj)->next;                  \
		(obj)->prev = (obj)->next = 0;         \
	}

// 从在双向链表中，用newobj替代oldobj
#define list_replace(head, oldobj, newobj)     \
	if((oldobj)->prev == (oldobj)) {           \
		(newobj)->prev = (newobj);             \
		(newobj)->next = (newobj);             \
		(head) = (newobj);                     \
	}                                          \
	else {                                     \
		(newobj)->prev = (oldobj)->prev;       \
		(newobj)->next = (oldobj)->next;       \
		(newobj)->prev->next = (newobj);       \
		(newobj)->next->prev = (newobj);       \
		if((head)==(oldobj))                   \
			(head) = (newobj);                 \
	}

// 插入一个obj到单向链表尾部
#define single_list_add_tail(head, tail, obj)                             		\
	(obj)->next = 0;                           		\
	if (!(head)) 							              \
	{                                       		      	  \
		(head) = (obj);                                \
	}                                                         	\
	else 								                     \
	{                                                    		\
		(tail)->next = (obj);                      	\
	}                                                         	\
	(tail) = (obj);

// 从单向链表头部移出一个对象，放到obj中
#define single_list_remove_head(head, tail, obj)         				\
	(obj) = (head);                         				\
	if (head) 							    \
	{                             					\
		(head) = (head)->next; 	\
		if(!(head))                             \
            (tail) = 0;                         \
	}

#endif //_LIST_H_
