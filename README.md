# System Programming (ELEC462)

경북대학교 컴퓨터학부 **시스템 프로그래밍(ELEC462)** 수업의 Lab / 과제 / 시험 제출물 모음.
교재는 Bruce Molay, *Understanding Unix/Linux Programming* 을 따르며, 대부분의 실습은
교재 예제(`who`, `cp`, `ls`, `pwd`, `smsh`, `webserv` 등)를 직접 구현하거나 변형·확장한 것이다.

- 학번: `s2023012486`
- 개발/실행 환경: Linux (Ubuntu), `gcc`
- 각 항목은 제출 시점의 `*_s2023012486.zip` 로 보관되어 있고, 주요 소스는 저장소 루트에 `.c` 로도 함께 올려두었다.

## 빌드 & 실행

특별한 빌드 시스템은 없다. 대부분 단일 파일이라 아래처럼 직접 컴파일한다.

```bash
unzip lab7_s2023012486.zip
cd lab7_s2023012486
gcc -o ticker_demo ticker_demo.c          # 일반 예제
gcc -o hello5 hello5.c -lcurses           # curses 사용 예제 (lab7 hello5, hw6 등)
gcc -o webserv webserv.c socklib.c        # 여러 파일로 나뉜 예제 (lab12)
```

`Makefile` 이 포함된 항목(lab11, lab12)은 해당 디렉터리에서 `make` 로 빌드한다.
curses/pthread/소켓을 쓰는 예제는 각각 `-lcurses`, `-lpthread` 링크 옵션이나 시스템 헤더가 필요하다.

## Labs

| Lab | 파일 | 내용 |
|---|---|---|
| lab1 | `helloworld.c` | 개발 환경(Ubuntu) 세팅 및 첫 컴파일 |
| lab2 | `sum200.c`, `test_v2.c`, `who2.c` | gdb 디버깅 실습 — 배열 범위 초과 추적, `utmp` 를 직접 읽는 `who` 구현 |
| lab3 | `who3.c`, `utmplib.c`, `cp1.c` | 버퍼링 라이브러리(`utmplib`)를 쓰는 `who`, 저수준 I/O(`read`/`write`/`creat`) 파일 복사 |
| lab4 | `ls2_v2.c`, `spwd.c` | `ls -l` 구현(stat, 모드 문자열, uid/gid 변환), inode 를 거슬러 올라가는 `pwd` 구현 |
| lab5 | `write0.c` | 다른 터미널 장치(`/dev/tty*`)에 직접 쓰기 |
| lab6 | `play_again3.c`, `sigdemo2_timing.c` | termios 로 raw 모드 + 타임아웃 입력받기, `SIGINT` 핸들러로 경과 시간 측정 |
| lab7 | `hello5.c`, `sigactiondemo2.c`, `ticker_demo.c` | curses 로 문자열 움직이기, `sigaction` 과 `siginfo_t`, `setitimer` 인터벌 타이머 |
| lab8 | `psh2.c` | 인자를 하나씩 입력받아 `fork`/`execvp` 로 실행하는 아주 단순한 쉘 |
| lab9 | `smsh4.c` 외 | 소형 쉘(smsh) — 변수 저장/`export`, `if`/`then`/`fi` 제어 흐름, 내장 명령 |
| lab10 | `pstofile.c`, `stdindir2.c` | `close`+`creat` 로 출력 리다이렉션, `dup`/`dup2` 로 표준입력 재지정 |
| lab11 | `rls.c`, `rlsd.c` | TCP 소켓 기반 원격 `ls` 클라이언트/서버 (포트 15000) |
| lab12 | `webserv.c`, `socklib.c`, `hello_multi.c` | 미니 HTTP 서버(정적 파일·디렉터리 목록·CGI), pthread 로 메시지 교차 출력 |

## 과제 (Homework)

| HW | 파일 | 내용 |
|---|---|---|
| hw1 | `triangle_classification_fixed.c` | gdb 로 삼각형 분류 프로그램의 버그 찾아 수정 (`gdb.hw1.log` 포함) |
| hw2 | `hw2_s2023012486.c` | 명시적 스택으로 구현한 `ls -R` (재귀 디렉터리 나열 + 파일 상세 정보) |
| hw3 | `hw3_s2023012486.c` | `-d <디렉터리> -s <문자열>` — 이름에 특정 부분 문자열이 들어간 파일 검색 |
| hw4 | `hw4_s2023012486.c` | `stty` 축소판 — termios input/output/local 플래그 조회 및 토글 |
| hw5 | `hw5_s2023012486.c` | `alarm`/`pause` 기반 로켓 발사 카운트다운, `SIGINT`/`SIGQUIT` 로 단 분리 처리 |
| hw6 | `hw6_s2023012486.c` | curses + `setitimer` 로 벽에 튕기는 공 애니메이션 |
| hw7 | `hw7_s2023012486.c` | `array.txt` 를 여러 자식 프로세스로 분할, 각자 부분 최댓값을 exit 코드로 반환 후 부모가 취합 |

## 시험

- `midterm.zip`, `midterm 2.zip`
  - **Q1** — 디렉터리를 파티션, 파일을 메시지로 보는 Kafka 스타일 토픽 관리 CLI
    (`--partition-info`, `--all-msg`, `--produce <message>`)
  - **Q2** — 시그널 처리: `sigaction` 으로 `SIGINT`/`SIGQUIT` 무시, `sleep` 잔여 시간을 이용한 `safe_sleep`

## 참고 자료

- `시프.pptx` — 수업 관련 발표 자료
