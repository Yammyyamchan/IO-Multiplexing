# 간단한 쉘 명령어를 실행하는 TCP기반의 Server와 Client
## select()함수를 이용한 멀티플렉싱
**1. Client에서 명령어 개수를 입력하고, 명령어를 입력한 후, 메시지를 Server로 전송**   
**2. Server는 Client로부터 받은 메시지를 system()호출해 간단한 쉘 명령어를 실행**  
**3. 하나의 프로세스가 동시에 여러개의 클라이언트의 요청을 처리함**  

---

메시지 구조  
|1byte|1byte|Nbyte|1byte|Nbyte|...|...|
|-----|-----|-----|-----|-----|---|---|
|명령어개수|명령어길이|명령어|명령어길이|명령어|...|...|

---
[Client]  
![스크린샷 2024-01-01 23-13-50](https://github.com/Yammyyamchan/IO-Multiplexing-/assets/131407815/0ca78583-6516-4a09-b5ba-f9ccc6bf274b)

명령어 개수만큼 반복을 해서 명령어를 입력받는다.

---  
[Server]  

![스크린샷 2024-01-01 23-22-30](https://github.com/Yammyyamchan/IO-Multiplexing-/assets/131407815/b1dbbf49-4d3a-4b8a-a938-96a2293dc172)


![스크린샷 2024-01-01 23-23-00](https://github.com/Yammyyamchan/IO-Multiplexing-/assets/131407815/bf7568af-788c-4465-b9c9-77e9e6c566f6) 

---  
[실행 결과]  
