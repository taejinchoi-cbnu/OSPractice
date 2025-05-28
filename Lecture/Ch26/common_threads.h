#ifndef __COMMON_THREADS_H__
#define __COMMON_THREADS_H__

#include <pthread.h> // POSIX 스레드 함수 사용을 위해 [9, 10]
#include <assert.h> // assert() 함수 사용을 위해 [5, 6]
#include <stdio.h>
#include <stdlib.h> // exit() 함수 사용을 위해 (assert 사용 시 필요 없을 수도 있음)

// pthread_create 함수에 대한 래퍼. 생성 실패 시 프로그램을 종료합니다. [3, 4]
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine) (void *), void *arg) {
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0); // 반환 코드가 0인지 확인 (성공) [3, 5, 6]
}

// pthread_join 함수에 대한 래퍼. 조인 실패 시 프로그램을 종료합니다. [1, 2, 4]
void Pthread_join(pthread_t thread, void **value_ptr) {
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0); // 반환 코드가 0인지 확인 (성공) [4, 6]
}

#endif // __COMMON_THREADS_H__