#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "smalloc.h" 
#include <sys/mman.h>
#include <string.h>

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
		smheader_ptr worst = NULL;
		size_t worst_chunk = 0;

		while (current) {
			if (current->used == 0 && current->size >= s) {
				if (current->size > worst_chunk) {
					worst = current;
					worst_chunk = current->size;
				}
			}
			current = current->next;
		}
		find_chunk = worst;
	}
	else {
		// first fit
		smheader_ptr first = NULL;
		size_t first_chunk = 0;

		while (current) {
			if (current->used == 0 && current->size >= s) {
				first = current;
				break;
			}
			current = current->next;
		}
		find_chunk = first;
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
		smheader_ptr current = smlist;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_header;
		// re-find (recurcive)
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
	// if p is null
	if (p == NULL) return;

	smheader_ptr header = (smheader_ptr)((char *)p - sizeof(smheader));
	smheader_ptr current = smlist;

	while (current) {
		if (current == header) {
			header->used = 0;
			// 해제 후 병합하기
			smcoalesce();
			return;
		}
		current = current->next;
	}
	// p is not in list
	abort();
}

void * srealloc (void * p, size_t s) 
{
    // if p is NULL
    if (p == NULL) return smalloc(s);
    // if s is 0
    if (s == 0) {
        sfree(p);
        return NULL;
    }
    
    smheader_ptr origin_header = (smheader_ptr)((char *)p - sizeof(smheader));
    size_t origin_size = origin_header->size;
    
    // s가 줄어드는 경우
    if (s < origin_size) {
        if (origin_size > s + sizeof(smheader)) {
            smheader_ptr split_header = (smheader_ptr)((char *)origin_header + sizeof(smheader) + s);
            split_header->size = origin_size - s - sizeof(smheader);
            split_header->used = 0;
            split_header->next = origin_header->next; 
            origin_header->next = split_header;
            origin_header->size = s;
            smcoalesce();
        } else {
            origin_header->size = s;
        }
        return p;
    }
    // 같은 경우
    else if (s == origin_size) {
        return p;
    }
    // s > origin_size
    else {
        // 인접한 블록과 병합 시도
        if (origin_header->next != NULL && 
            origin_header->next->used == 0 && 
            origin_size + origin_header->next->size + sizeof(smheader) >= s) {
            
            // 병합 가능!
            origin_header->size = origin_size + sizeof(smheader) + origin_header->next->size;
            smheader_ptr next_block = origin_header->next;
            origin_header->next = next_block->next;

            // 확장 후 남는 공간이 충분하면 분할
            if (origin_header->size > s + sizeof(smheader)) {
                smheader_ptr split_header = (smheader_ptr)((char *)origin_header + sizeof(smheader) + s);
                split_header->size = origin_header->size - s - sizeof(smheader);
                split_header->used = 0;
                split_header->next = origin_header->next;
                origin_header->next = split_header;
                origin_header->size = s;
                smcoalesce();
            }
            return p;
        }
        
        // 1. 인접 블록이 없거나
        // 2. 인접 블록이 사용중이거나  
        // 3. 병합해도 크기가 부족한 경우
        void* new_ptr = smalloc(s);
        if (new_ptr == NULL) return NULL;
        
        memcpy(new_ptr, p, origin_size);
        sfree(p);
        return new_ptr;
    }
}

void smcoalesce ()
{
	smheader_ptr current = smlist;
	while (current)
	{
		if (current->used == 0 && current->next != NULL && current->next->used == 0) {
			current->size = current->size + current->next->size + sizeof(smheader);
			current->next = current->next->next;
		}
		else {
			current = current->next;
		}
	}
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
