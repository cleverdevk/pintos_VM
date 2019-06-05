#include "bitmap.h"
#include <debug.h>
#include <limits.h>
#include <round.h>
#include <stdio.h>
#include "threads/malloc.h"
#ifdef FILESYS
#include "filesys/file.h"
#endif


#define NO_VALUE 9999;

//typedef struct Node* Node_pointer;

size_t buddy1[256] = {0, };
size_t buddy2[256] = {256,0 };
/*
BuddyListPointer head;

Node_pointer init_list() //return HEAD NODE
{
	Node_pointer HEAD = malloc(sizeof(Node));
	HEAD->start = NO_VALUE; //initialize HEAD to 9999(NOVALUE)
	HEAD->next = NULL;
	HEAD->before = NULL;
	return HEAD;
}

Node_pointer getScaleHEAD(BuddyListPointer buddypointer, size_t scale)
{
	BuddyListPointer Next = buddypointer;
	printf("getScaleHEAD\n");
	while (Next->scale != scale)
	{
		printf("getScaleHEAD___while\n");
		Next = Next->upper;
	}
	printf("getScaleHEAD_END\n");
	return Next->HEAD;
}

void add_node(size_t start, Node_pointer HEAD, size_t scale)
{
	Node_pointer Next = HEAD;
	Node_pointer node = (Node_pointer)malloc(sizeof(Node));

	node->start = start;

	if (HEAD->next == NULL) {
		HEAD->next = node;
		node->before = HEAD;
		node->next = NULL;
		return;
	}

	Next = Next->next;

	while (Next->start< start) {
		if (Next->next != NULL) {
			Next = Next->next;
		}
		else {
			break;
		}
	}
	
	if (Next->next == NULL && Next->start < start) {
		node->before = Next;
		node->next = NULL;
		Next->next = node;

		// merge buddies
		Node_pointer b = node->before;

		if (b != NULL && b != HEAD && b->start + scale == node->start && (b->start / scale) % 2 == 0) {
			//merge
			add_node(b->start, getScaleHEAD(head, scale * 2), scale * 2);
			remove_node(HEAD, b->start);
			remove_node(HEAD, node->start);

		}
		return;
	}
	else {
		node->next = Next;
		Next->before->next = node;
		node->before = Next->before;

		// merge buddies
		Node_pointer b = node->before;
		Node_pointer n = node->next;

		if (b != NULL && b != HEAD && b->start + scale == node->start && (b->start / scale) % 2 == 0) {
			//merge
			add_node(b->start, getScaleHEAD(head, scale * 2), scale * 2);
			remove_node(HEAD, b->start);
			remove_node(HEAD, node->start);

		}
		if (n != NULL && node->start + scale == n->start && ((n->start + scale) / scale) % 2 == 1) {
			//merge
			add_node(node->start, getScaleHEAD(head, scale * 2), scale * 2);
			remove_node(HEAD, node->start);
			remove_node(HEAD, n->start);
		}
	}

	return;
}

void remove_node(Node_pointer HEAD, size_t start) {
	Node_pointer Next = HEAD->next;
	if (Next == NULL) {
		printf("[ERROR] There is no node(EMPTY LIST ONLY HEADER).\n");
		return;
	}
	while (Next->start != start) {
		Next = Next->next;
		if (Next == NULL) {
			printf("[ERROR] There is no node that has start=%d\n", start);
			return;
		}
	}
	if (Next->next == NULL) { // if there is a only one node.
		HEAD->next = NULL;
		free(Next);
		return;
	}
	Next->next->before = Next->before;
	Next->before->next = Next->next;
	free(Next);
	
	return;
}

size_t pop_node(Node_pointer HEAD)
{
	printf("pop?\n");
	size_t start;
	Node_pointer Next = HEAD->next;
	printf("POP_NODE START!\n");
	if (Next == NULL) {
		printf("[ERROR] There is no node(EMPTY LIST ONLY HEADER).\n");
		return NO_VALUE;
	}
	if (Next->next == NULL) { // if there is a only one node.
		HEAD->next = NULL;
		start = Next->start;
		free(Next);
		return start;
	}
	start = Next->start;
	HEAD->next = Next->next;
	Next->next->before = HEAD;
	free(Next);

	return start;
}




void init_buddy_list() {
	BuddyListPointer BuddyHEAD = malloc(sizeof(BuddyList));
	BuddyListPointer current = BuddyHEAD;
	BuddyListPointer blp = malloc(sizeof(BuddyList));
	int ScaleArray[9] = { 1,2,4,8,16,32,64,128,256 };
	int i;

	current->scale = NO_VALUE;
	current->lower = NULL;
	current->upper = blp;
	blp->lower = current;
	blp->HEAD = init_list();

	current = current->upper;

	for (i = 0; i < 9; i++) {
		BuddyListPointer blp2 = malloc(sizeof(BuddyList));
		current->scale = ScaleArray[i];
		current->HEAD = init_list();
		if (i == 8) {
			current->upper = NULL;
			break;
		}
		current->upper = blp2;
		blp2->lower = current;

		current = current->upper;
	}
	head = BuddyHEAD;
	add_node(0, getScaleHEAD(head, 256), 256);
}

void print_status() {
	BuddyListPointer NextChannel = head->upper;
	while (NextChannel->upper != NULL) {
		printf("SCALE : %d : ", NextChannel->scale);
		Node_pointer NextNode = NextChannel->HEAD->next;
		while (NextNode != NULL) {
			printf("%d ", NextNode->start);
			NextNode = NextNode->next;
		}
		printf("\n");
		NextChannel = NextChannel->upper;
	}
}

*/

/* Element type.

   This must be an unsigned integer type at least as wide as int.

   Each bit represents one bit in the bitmap.
   If bit 0 in an element represents bit K in the bitmap,
   then bit 1 in the element represents bit K+1 in the bitmap,
   and so on. */
typedef unsigned long elem_type;

size_t new_size = 0;
size_t new_adr = 0;

/* Number of bits in an element. */
#define ELEM_BITS (sizeof (elem_type) * CHAR_BIT)

/* From the outside, a bitmap is an array of bits.  From the
   inside, it's an array of elem_type (defined above) that
   simulates an array of bits. */
struct bitmap
  {
    size_t bit_cnt;     /* Number of bits. */
    elem_type *bits;    /* Elements that represent bits. */
  };

/* Returns the index of the element that contains the bit
   numbered BIT_IDX. */
static inline size_t
elem_idx (size_t bit_idx) 
{
  return bit_idx / ELEM_BITS;
}

/* Returns an elem_type where only the bit corresponding to
   BIT_IDX is turned on. */
static inline elem_type
bit_mask (size_t bit_idx) 
{
  return (elem_type) 1 << (bit_idx % ELEM_BITS);
}

/* Returns the number of elements required for BIT_CNT bits. */
static inline size_t
elem_cnt (size_t bit_cnt)
{
  return DIV_ROUND_UP (bit_cnt, ELEM_BITS);
}

/* Returns the number of bytes required for BIT_CNT bits. */
static inline size_t
byte_cnt (size_t bit_cnt)
{
  return sizeof (elem_type) * elem_cnt (bit_cnt);
}

/* Returns a bit mask in which the bits actually used in the last
   element of B's bits are set to 1 and the rest are set to 0. */
static inline elem_type
last_mask (const struct bitmap *b) 
{
  int last_bits = b->bit_cnt % ELEM_BITS;
  return last_bits ? ((elem_type) 1 << last_bits) - 1 : (elem_type) -1;
}

/* Creation and destruction. */

/* Creates and returns a pointer to a newly allocated bitmap with room for
   BIT_CNT (or more) bits.  Returns a null pointer if memory allocation fails.
   The caller is responsible for freeing the bitmap, with bitmap_destroy(),
   when it is no longer needed. */
struct bitmap *
bitmap_create (size_t bit_cnt) 
{
  struct bitmap *b = malloc (sizeof *b);
  if (b != NULL)
    {
      b->bit_cnt = bit_cnt;
      b->bits = malloc (byte_cnt (bit_cnt));
      if (b->bits != NULL || bit_cnt == 0)
        {
          bitmap_set_all (b, false);
          return b;
        }
      free (b);
    }
  return NULL;
}

/* Creates and returns a bitmap with BIT_CNT bits in the
   BLOCK_SIZE bytes of storage preallocated at BLOCK.
   BLOCK_SIZE must be at least bitmap_needed_bytes(BIT_CNT). */
struct bitmap *
bitmap_create_in_buf (size_t bit_cnt, void *block, size_t block_size UNUSED)
{
  struct bitmap *b = block;
  
  ASSERT (block_size >= bitmap_buf_size (bit_cnt));

  b->bit_cnt = bit_cnt;
  b->bits = (elem_type *) (b + 1);
  bitmap_set_all (b, false);
  return b;
}

/* Returns the number of bytes required to accomodate a bitmap
   with BIT_CNT bits (for use with bitmap_create_in_buf()). */
size_t
bitmap_buf_size (size_t bit_cnt) 
{
  return sizeof (struct bitmap) + byte_cnt (bit_cnt);
}

/* Destroys bitmap B, freeing its storage.
   Not for use on bitmaps created by bitmap_create_in_buf(). */
void
bitmap_destroy (struct bitmap *b) 
{
  if (b != NULL) 
    {
      free (b->bits);
      free (b);
    }
}

/* Bitmap size. */

/* Returns the number of bits in B. */
size_t
bitmap_size (const struct bitmap *b)
{
  return b->bit_cnt;
}

/* Setting and testing single bits. */

/* Atomically sets the bit numbered IDX in B to VALUE. */
void
bitmap_set (struct bitmap *b, size_t idx, bool value) 
{
  ASSERT (b != NULL);
  ASSERT (idx < b->bit_cnt);
  if (value)
    bitmap_mark (b, idx);
  else
    bitmap_reset (b, idx);
}

/* Atomically sets the bit numbered BIT_IDX in B to true. */
void
bitmap_mark (struct bitmap *b, size_t bit_idx) 
{
  size_t idx = elem_idx (bit_idx);
  elem_type mask = bit_mask (bit_idx);

  /* This is equivalent to `b->bits[idx] |= mask' except that it
     is guaranteed to be atomic on a uniprocessor machine.  See
     the description of the OR instruction in [IA32-v2b]. */
  asm ("orl %1, %0" : "=m" (b->bits[idx]) : "r" (mask) : "cc");
}

/* Atomically sets the bit numbered BIT_IDX in B to false. */
void
bitmap_reset (struct bitmap *b, size_t bit_idx) 
{
  size_t idx = elem_idx (bit_idx);
  elem_type mask = bit_mask (bit_idx);

  /* This is equivalent to `b->bits[idx] &= ~mask' except that it
     is guaranteed to be atomic on a uniprocessor machine.  See
     the description of the AND instruction in [IA32-v2a]. */
  asm ("andl %1, %0" : "=m" (b->bits[idx]) : "r" (~mask) : "cc");
}

/* Atomically toggles the bit numbered IDX in B;
   that is, if it is true, makes it false,
   and if it is false, makes it true. */
void
bitmap_flip (struct bitmap *b, size_t bit_idx) 
{
  size_t idx = elem_idx (bit_idx);
  elem_type mask = bit_mask (bit_idx);

  /* This is equivalent to `b->bits[idx] ^= mask' except that it
     is guaranteed to be atomic on a uniprocessor machine.  See
     the description of the XOR instruction in [IA32-v2b]. */
  asm ("xorl %1, %0" : "=m" (b->bits[idx]) : "r" (mask) : "cc");
}

/* Returns the value of the bit numbered IDX in B. */
bool
bitmap_test (const struct bitmap *b, size_t idx) 
{
  ASSERT (b != NULL);
  ASSERT (idx < b->bit_cnt);
  return (b->bits[elem_idx (idx)] & bit_mask (idx)) != 0;
}

/* Setting and testing multiple bits. */

/* Sets all bits in B to VALUE. */
void
bitmap_set_all (struct bitmap *b, bool value) 
{
  ASSERT (b != NULL);

  bitmap_set_multiple (b, 0, bitmap_size (b), value);
}

/* Sets the CNT bits starting at START in B to VALUE. */
void
bitmap_set_multiple (struct bitmap *b, size_t start, size_t cnt, bool value) 
{
  size_t i;
  
  ASSERT (b != NULL);
  ASSERT (start <= b->bit_cnt);
  ASSERT (start + cnt <= b->bit_cnt);

  for (i = 0; i < cnt; i++)
    bitmap_set (b, start + i, value);
}

/* Returns the number of bits in B between START and START + CNT,
   exclusive, that are set to VALUE. */
size_t
bitmap_count (const struct bitmap *b, size_t start, size_t cnt, bool value) 
{
  size_t i, value_cnt;

  ASSERT (b != NULL);
  ASSERT (start <= b->bit_cnt);
  ASSERT (start + cnt <= b->bit_cnt);

  value_cnt = 0;
  for (i = 0; i < cnt; i++)
    if (bitmap_test (b, start + i) == value)
      value_cnt++;
  return value_cnt;
}

/* Returns true if any bits in B between START and START + CNT,
   exclusive, are set to VALUE, and false otherwise. */
bool
bitmap_contains (const struct bitmap *b, size_t start, size_t cnt, bool value) 
{
  size_t i;
  
  ASSERT (b != NULL);
  ASSERT (start <= b->bit_cnt);
  ASSERT (start + cnt <= b->bit_cnt);
//	printf("bit_cnt = %d\n", b->bit_cnt);
  for (i = 0; i < cnt; i++)
    if (bitmap_test (b, start + i) == value)
      return true;
  return false;
}

/* Returns true if any bits in B between START and START + CNT,
   exclusive, are set to true, and false otherwise.*/
bool
bitmap_any (const struct bitmap *b, size_t start, size_t cnt) 
{
  return bitmap_contains (b, start, cnt, true);
}

/* Returns true if no bits in B between START and START + CNT,
   exclusive, are set to true, and false otherwise.*/
bool
bitmap_none (const struct bitmap *b, size_t start, size_t cnt) 
{
  return !bitmap_contains (b, start, cnt, true);
}

/* Returns true if every bit in B between START and START + CNT,
   exclusive, is set to true, and false otherwise. */
bool
bitmap_all (const struct bitmap *b, size_t start, size_t cnt) 
{
  return !bitmap_contains (b, start, cnt, false);
}

/* Finding set or unset bits. */

/* Finds and returns the starting index of the first group of CNT
   consecutive bits in B at or after START that are all set to
   VALUE.
   If there is no such group, returns BITMAP_ERROR. */
size_t
bitmap_scan (const struct bitmap *b, size_t start, size_t cnt, bool value) 
{
  ASSERT (b != NULL);
  ASSERT (start <= b->bit_cnt);

  if (cnt <= b->bit_cnt) 
    {
      size_t last = b->bit_cnt - cnt;
      size_t i;
      for (i = start; i <= last; i++)
        if (!bitmap_contains (b, i, cnt, !value))
          return i; 
    }
  return BITMAP_ERROR;

}

size_t
cnt_to_buddy_size (size_t cnt)
{
	ASSERT (cnt <= 256);
	
	if(cnt > 128)
		return 256;
	else if(cnt > 64)
		return 128;
	else if(cnt > 32)
		return 64;
	else if(cnt > 16)
		return 32;
	else if(cnt > 8)
		return 16;
	else if(cnt > 4)
		return 8;
	else if(cnt > 2)
		return 4;
	else if(cnt > 1)
		return 2;
	else
		return 1;
}
/*
size_t
bitmap_scan_for_buddy (const struct bitmap *b, size_t start, size_t cnt, bool value)
{
	size_t buddy_size;	
	size_t adr;
	size_t count = 0;
	printf("here1\n");
	ASSERT (b != NULL);
	printf("here2\n");
	ASSERT (start <= b->bit_cnt);
	printf("here3\n");
	buddy_size = cnt_to_buddy_size(cnt);
	printf("here4\n");
	while(true)
	{
		printf("here5\n");
		adr = pop_node(getScaleHEAD(head, buddy_size));
		printf("adr = %d\n", adr);
		if(adr != 9999)
		{
			while(count > 0)
			{
				printf("[while] buddy_size = %d\n", buddy_size);
				buddy_size /= 2;
				add_node(adr + buddy_size, getScaleHEAD(head, buddy_size),buddy_size);
				count--;
			}
			return adr;
		}
		else
		{
			printf("[scan_while_else]\n");
			if(buddy_size != 256)
			{
				printf("[scan_while_else_if]\n");
				count++;
				buddy_size *= 2;
			}
			else
				return BITMAP_ERROR;
		}

	}
	
	return BITMAP_ERROR;
}

size_t
bitmap_scan_and_flip_for_buddy (struct bitmap *b, size_t start, size_t cnt, bool value)
{
	printf("[log] call bitmap_scan_for_buddy//start = %d,  cnt = %d,  b->bit_cnt = %d\n", start, cnt, b->bit_cnt);
	size_t idx = bitmap_scan_for_buddy(b, start, cnt, value);
	printf("bitmap_scan_for_buddy is run? // idx = %d\n", idx);
	if(idx != BITMAP_ERROR)
		bitmap_set_multiple(b, idx, cnt_to_buddy_size(cnt), !value);
	return idx;
}
*/
size_t bitmap_buddy_array (size_t start, size_t cnt);

size_t
bitmap_scan_and_flip_for_buddy1 (struct bitmap *b, size_t start, size_t cnt, bool value)
{
	if(start > b->bit_cnt || b == NULL)
		return BITMAP_ERROR;

	size_t idx = bitmap_buddy_array (start, cnt);
	size_t j;
//	printf("\nInput Address : %d\n", idx);
	if(idx != BITMAP_ERROR)
	{
		bitmap_set_multiple(b, idx, cnt_to_buddy_size(cnt), !value);
/*		for(j = start; j < 256; j++){
		if(buddy2[j] != 0 || buddy1[j] == 1)
		printf("|	b->bits = %lu	|	buddy1[%d] = %d		|	buddy2[%d] = %d		|\n", b->bits[elem_idx (j)], j, buddy1[j], j, buddy2[j]);
}*/
	}
	return idx;
	
	
}


size_t
bitmap_buddy_array (size_t start, size_t cnt)
{
	size_t i;
	size_t buddy_size = cnt_to_buddy_size(cnt);
	size_t save;
	size_t j;
//	printf("\nInput Size = %d | Buddy Size = %d\n", cnt, buddy_size);
	ASSERT (buddy_size <= 256);
	for(i = start; i < 256; )
	{
		if(buddy1[i] == 0 && buddy2[i] >= buddy_size)
		{
			if(buddy2[i] == buddy_size)
			{
				for(save = start; save < buddy_size; save++)
					buddy1[i] = 1;
			}
			else
			{
				save = buddy2[i] / 2;
				while(save >= buddy_size)
				{
					buddy2[i] = save;
					buddy2[save + i] = save;
					save /= 2;
				}
				j = i;
				for(save = start; save < buddy_size; save++)
				{
					buddy1[j++] = 1;
				}
			}
			
			return i;
		}
		if(buddy1[i] != 0 || buddy2[i] < buddy_size)
			i += buddy2[i];
	}

	return BITMAP_ERROR;
	
}
void
bitmap_buddy_free(size_t start, size_t cnt)
{
	size_t i;
	size_t end = start + cnt;
	size_t n;
	size_t j;

//	printf("\nFree index = %d, Size = %d \n", start, cnt);
	for(i = start; i < end; i++)
		buddy1[i] = 0;
	while(true)
	{
		if((start / buddy2[start]) % 2 == 0)
		{
			n = start + buddy2[start];
			if(buddy1[n] == 0)
			{
				buddy2[n] = 0;
				buddy2[start] *= 2;
			}
			else
				break;
		}
		else
		{
			n = start - buddy2[start];
			if(buddy1[n] == 0)
			{
				buddy2[start] = 0;
				buddy2[n] *= 2;
			}
			else
				break;
		}
	}
/*
	for(j = 0; j < 256; j++)
	{
		if(buddy2[j] != 0 || buddy1[j] == 1)
		printf("|	buddy1[%d] = %d		|	buddy2[%d] = %d		|\n", j, buddy1[j], j, buddy2[j]);
	}
*/
}



											//bk : contains of bestfit
bool
bitmap_contains_for_bestfit (const struct bitmap *b, size_t start, size_t cnt, bool value)
{
	size_t i;
	
	ASSERT (b != NULL);
	ASSERT (start <= b->bit_cnt);
	ASSERT (start + cnt <= b->bit_cnt);

	for(i = 0; i < cnt; i++)
		if(bitmap_test(b, start + i) == value)
			return true;
//	printf("count\n");
	while(bitmap_test(b, start + i) != value){
//		printf("while(i = %d, bit_cnt = %d)		", i, b->bit_cnt);	
		i++;
		if(start + i >= b->bit_cnt)
			break;
	}
	new_adr = start;
	new_size = i;

	return false;
}

											//bk : best-fit
size_t
bitmap_scan_for_bestfit (const struct bitmap *b, size_t start, size_t cnt, bool value)
{
  size_t best_size = 9999;
  size_t best_adr = 9999;


  ASSERT (b != NULL);
  ASSERT (start <= b->bit_cnt);

  if(cnt <= b->bit_cnt)
  {
     size_t last = b->bit_cnt - cnt;
     size_t i;
     for(i = start; i <= last; )
     {
//	printf("go to contains \n");
//	printf("[bitmap_scan_for_bestfit] i = %d, cnt = %d\n", i, cnt);
        if(!bitmap_contains_for_bestfit (b, i, cnt, !value))
	{
//		printf("find address!\n");
//		printf("best_size = %d, best_adr = %d, new_size = %d, new_adr = %d\n", best_size, best_adr, new_size, new_adr);
		if(best_size > new_size)
		{
			best_size = new_size;
			best_adr = new_adr;
//			printf("[Log] now best-fit address : %d\n", best_adr);
//			printf("[Log] now best-fit size : %d\n", best_size);
		}
	i += new_size;
        }
	else	i++;
     }
  }
//  printf("[Log] final best-fit address : %d\n", best_adr);
//  printf("[Log] final best-fit size : %d\n", best_size);
  return best_adr;
}
/* Finds the first group of CNT consecutive bits in B at or after
   START that are all set to VALUE, flips them all to !VALUE,
   and returns the index of the first bit in the group.
   If there is no such group, returns BITMAP_ERROR.
   If CNT is zero, returns 0.
   Bits are set atomically, but testing bits is not atomic with
   setting them. */
size_t
bitmap_scan_and_flip (struct bitmap *b, size_t start, size_t cnt, bool value)
{
  size_t idx = bitmap_scan_for_bestfit (b, start, cnt, value);
  if (idx != BITMAP_ERROR) 
    bitmap_set_multiple (b, idx, cnt, !value);
//	printf("[new log] b->bit_cnt = %d,  cnt = %d,  address = %d\n", b->bit_cnt, cnt, idx);
  return idx;
}

/* File input and output. */

#ifdef FILESYS
/* Returns the number of bytes needed to store B in a file. */
size_t
bitmap_file_size (const struct bitmap *b) 
{
  return byte_cnt (b->bit_cnt);
}

/* Reads B from FILE.  Returns true if successful, false
   otherwise. */
bool
bitmap_read (struct bitmap *b, struct file *file) 
{
  bool success = true;
  if (b->bit_cnt > 0) 
    {
      off_t size = byte_cnt (b->bit_cnt);
      success = file_read_at (file, b->bits, size, 0) == size;
      b->bits[elem_cnt (b->bit_cnt) - 1] &= last_mask (b);
    }
  return success;
}

/* Writes B to FILE.  Return true if successful, false
   otherwise. */
bool
bitmap_write (const struct bitmap *b, struct file *file)
{
  off_t size = byte_cnt (b->bit_cnt);
  return file_write_at (file, b->bits, size, 0) == size;
}
#endif /* FILESYS */

/* Debugging. */

/* Dumps the contents of B to the console as hexadecimal. */
void
bitmap_dump (const struct bitmap *b) 
{
  hex_dump (0, b->bits, byte_cnt (b->bit_cnt), false);
}

