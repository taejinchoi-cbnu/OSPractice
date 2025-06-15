#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

typedef 
	struct {
		int order ;
		char * message ;
	}
	work_args_t ;


pthread_mutex_t 	m = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t  	c = PTHREAD_COND_INITIALIZER ;

int order = 1 ;

void * work (void * a)
{
	/* TODO
	1. 뮤텍스 잠금
	2. 순서가 올 때까지 wait
	3. 메시지 3번 출력
	4. order 증가
	5. 모든 대기 스레드 깨우기
	6. 뮤텍스 해제 */

	// parameter로 받은 값 work_args_t 타입으로 캐스팅
	work_args_t* args = (work_args_t* ) a;
	pthread_mutex_lock(&m);

	while(order != args->order)
		pthread_cond_wait(&c, &m);
	

	for (int i = 0; i < 3; i++) 
		printf("%s\n", args->message);
	
	// 메세지 출력 후 order 증가
	order++;
	
	pthread_cond_broadcast(&c);

	pthread_mutex_unlock(&m);

	return NULL;
	
}

int main ()
{
	// Thread 2 -> 3 -> 1 -> 4 -> main 순서대로 출력되어야함
	work_args_t args[5] = {{3, "Thread 1"}, {1, "Thread 2"}, {2, "Thread 3"}, {4, "Thread 4"}, {5, "Main thread"}};

	// 스레드 생성
	for (int i = 0 ; i < 4 ; i++) {
		pthread_t t ;
		pthread_create(&t, NULL, work, (void*) &(args[i]));
	}

	// 스레드 실행
	work((void *) &(args[4])) ;

	exit(0);
}


