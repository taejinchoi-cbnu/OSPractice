struct _smheader {	
	size_t size ;
	uint8_t used ;
	struct _smheader * next ;
} ;

typedef struct _smheader 	smheader ;
typedef struct _smheader *	smheader_ptr ;

typedef 
	enum {
		bestfit, worstfit, firstfit
	} 
	smmode ;

void * smalloc (size_t s) ;
void * sset_mode (smmode m) ;

void sfree (void * p) ;

void * srealloc (void * p, size_t s) ;

void smcoalesce () ;

void smdump () ;
