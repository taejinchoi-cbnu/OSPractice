#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// 메가바이트를 바이트로 변환하는 매크로
#define MB (1024*1024)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("사용법: %s <메모리_크기_MB> [실행_시간_초]\n", argv[0]);
        return 1;
    }
    
    // 명령줄 인자 파싱
    int mem_mb = atoi(argv[1]);
    int run_seconds = 0;  // 0은 무한히 실행
    
    if (argc >= 3) {
        run_seconds = atoi(argv[2]);
    }
    
    printf("메모리 %d MB 할당 중...\n", mem_mb);
    
    // 메모리 할당 (메가바이트 단위를 바이트로 변환)
    int array_size = mem_mb * MB;
    char *array = (char *) malloc(array_size);
    
    if (array == NULL) {
        printf("메모리 할당 실패!\n");
        return 1;
    }
    
    printf("메모리 할당 완료. PID: %d\n", getpid());
    
    // 초기화 (각 바이트를 1로 설정)
    memset(array, 1, array_size);
    
    // 시작 시간 기록
    time_t start_time = time(NULL);
    time_t end_time = start_time + run_seconds;
    
    // 메모리에 지속적으로 접근
    long long access_count = 0;
    int index = 0;
    
    while (1) {
        // 배열의 각 요소에 접근하고 값을 변경
        for (int i = 0; i < array_size; i += 4096) {  // 4KB 단위로 접근
            array[i] = (char)(i % 256);
        }
        
        access_count++;
        
        // 1초마다 상태 출력
        time_t current = time(NULL);
        if (current != start_time && current % 10 == 0) {
            printf("메모리 접근 중... %lld회 완료\n", access_count);
            start_time = current;
        }
        
        // 지정된 시간만큼 실행했는지 확인
        if (run_seconds > 0 && time(NULL) >= end_time) {
            break;
        }
    }
    
    // 메모리 해제
    free(array);
    printf("프로그램 종료. 총 %lld회 메모리 접근 완료\n", access_count);
    
    return 0;
}