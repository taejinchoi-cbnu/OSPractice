#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h> // assert() 함수 사용을 위해 [5, 8]
#include <stdio.h>
#include <stdlib.h> // malloc() 함수 사용을 위해

// malloc 함수에 대한 기본적인 래퍼. 메모리 할당 실패 시 프로그램을 종료합니다. [4]
void *Malloc(size_t size) {
    void *p = malloc(size);
    assert(p != NULL); // 할당된 포인터가 NULL이 아닌지 확인 [5, 6]
    return p;
}

#endif // __COMMON_H__