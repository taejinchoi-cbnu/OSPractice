#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

/*TODO: add global variables and synchronization variables */

char * keyword = NULL ;

int is_text_file (char * filename)
{
	if (strlen(filename) < 4)
		return 0 ;

	if (strcmp(".txt", filename + strlen(filename) - 4) != 0)
		return 0 ;
	return 1 ;
}

void rtrim (char * s)
{
	size_t l = strlen(s) ;
	if (l == 0)
		return ;
	char * p = s + strlen(s) - 1 ;
	while (s != p && isspace(*p)) {
		*p = '\0' ;
		p-- ;
	}
}

void * search_one_file (void * a)
/* change this function into a multithreaded code */
{
	char * filename = (char *) a ;
	FILE * fp = NULL ;

	if (!(fp = fopen(filename, "r")))
		return NULL ;

	int linenum = 1 ;
	char * buf = malloc(sizeof(char) * 2048) ;
	size_t buf_size = 2048 ;
	while (!feof(fp)) {
		ssize_t l = getline(&buf, &buf_size, fp) ;
		rtrim(buf) ;

		if (l < 0) 
			break ;
		if (strstr(buf, keyword)) {
			printf("%s:%d  %s\n", filename, linenum, buf) ;
		}

		linenum++ ;
	}
	if (buf != NULL)
		free(buf) ;

	if (fp != NULL) 
		fclose(fp) ;

	return NULL ;
}

int main (int argc, char ** args) 
{
	if (argc != 2)
		exit(1) ;

	keyword = args[1] ;

	DIR * dir;
	if (dir = opendir(".")) {
		struct dirent *dent;
		while ((dent = readdir(dir))!=NULL) {
			if (is_text_file(dent->d_name)) {
				search_one_file((void *) dent->d_name) ;
			}
		}
		closedir(dir);
	}

	/*TODO*/

	exit(0);
}


