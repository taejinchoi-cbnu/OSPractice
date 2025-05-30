#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "smalloc.h" 
#include <sys/mman.h>

smheader_ptr smlist = 0x0 ;
smmode smalloc_mode = bestfit ;

void * smalloc (size_t s) 
{
	if (s == 0) return NULL;
	
    if (smlist == NULL) {
		void* request_memory = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

        if (request_memory == MAP_FAILED) return NULL;
        
        smlist = (smheader_ptr)request_memory;
        smlist->size = getpagesize() - sizeof(smheader);
        smlist->used = 0;
        smlist->next = NULL;
    }

	smheader_ptr current = smlist;
	smheader_ptr find_chunk = NULL;

	if (smalloc_mode == bestfit) {
		smheader_ptr best = NULL;
		size_t best_chunk = SIZE_MAX;

		while (current) {
			if (current->used == 0 && current->size >= s) {
				if (current->size < best_chunk) {
					best = current;
					best_chunk = current->size;
                }
            }
            current = current->next;
        }
        find_chunk = best;
    }
	else if (smalloc_mode == worstfit) {
		// worst fit
	}
	else {
		// first fit
	}

	// find_chunk without request more page
	if (find_chunk != NULL) {
		find_chunk->used = 1;

		// if find_chunk size enough -> split for memory efficient
		if (find_chunk->size > s + sizeof(smheader)) {
			size_t remain = find_chunk->size - s - sizeof(smheader);
			
			// make a split header
			smheader_ptr split_header = (smheader_ptr)((char*)find_chunk + sizeof(smheader) + s);
			split_header->size = remain;
			split_header->used = 0;
			split_header->next = find_chunk->next;
				
			// change current header
			find_chunk->size = s;
			find_chunk->next = split_header;
		}
		// return allocated ptr address
		return (char *)find_chunk + sizeof(smheader);
	}

	// if size is bigger than free-list size
	// request another memory page
	void * more_memory = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (more_memory == MAP_FAILED) {
		fprintf(stderr, "can't mmap more memory");
		return NULL;
	}
	else {
		// link new memory to free-list

		// make a new memory page header
		smheader_ptr new_header = (smheader_ptr)more_memory;
		new_header->size = getpagesize() - sizeof(smheader);
		new_header->used = 0;
		new_header->next = NULL;

		// connect new memory to free-list
		if (smlist == NULL) {
			smlist = new_header;
		}
		else {
			smheader_ptr current = smlist;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = new_header;
		}

		// re-find
		return smalloc(s);
	}
}

// DONE (EZPZ)
void * sset_mode (smmode m)
{
	smalloc_mode = m;
	return 0x0 ;
}

void sfree (void * p) 
{
	// TODO 
}

void * srealloc (void * p, size_t s) 
{
	// TODO
	return 0x0 ; // erase this 
}

void smcoalesce ()
{
	//TODO
}

void smdump () 
{
	smheader_ptr itr ;

	printf("==================== used memory slots ====================\n") ;
	int i = 0 ;
	for (itr = smlist ; itr != 0x0 ; itr = itr->next) {
		if (itr->used == 0)
			continue ;

		printf("%3d:%p:%8d:", i, ((void *) itr) + sizeof(smheader), (int) itr->size) ;

		int j ;
		char * s = ((char *) itr) + sizeof(smheader) ;
		for (j = 0 ; j < (itr->size >= 8 ? 8 : itr->size) ; j++)  {
			printf("%02x ", s[j]) ;
		}
		printf("\n") ;
		i++ ;
	}
	printf("\n") ;

	printf("==================== unused memory slots ====================\n") ;
	i = 0 ;
	for (itr = smlist ; itr != 0x0 ; itr = itr->next, i++) {
		if (itr->used == 1)
			continue ;

		printf("%3d:%p:%8d:", i, ((void *) itr) + sizeof(smheader), (int) itr->size) ;

		int j ;
		char * s = ((char *) itr) + sizeof(smheader) ;
		for (j = 0 ; j < (itr->size >= 8 ? 8 : itr->size) ; j++) {
			printf("%02x ", s[j]) ;
		}
		printf("\n") ;
		i++ ;
	}
	printf("\n") ;
}
