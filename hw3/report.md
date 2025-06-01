# 고민한거

먼저 malloc부터 구현해봤다.
smlist가 NULL인 경우 (첫 번째 malloc) mmap을 통해 메모리를 요청하고
mmap 실패 핸들링 추가
그리고 smlist를 초기화 해준다.
mmap으로 가저온 data에 헤더 빼고 나머지 부분에서 요청한 size만큼 할당해주고 header 만들기
free 는 malloc의 역순으로 진행하면 됨.