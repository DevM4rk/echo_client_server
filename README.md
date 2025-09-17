### 과제
echo-client, echo-server 프로그램.

### 실행

```
echo-client:
syntax : echo-client <ip> <port>
sample : echo-client 192.168.10.2 1234

echo-server:
syntax : echo-server <port> [-e[-b]]
sample : echo-server 1234 -e -b
```


### 상세


- socket 관련 함수를 사용(socket, connect, send, recv, bind, listen, accept 등).


- echo-client(이하 client)는 echo-server(server)에 TCP 접속.


- client는 사용자로부터 메세지를 입력받아 server에 메세지를 전달.


- server는 받은 메세지를 화면에 출력하고 "-e"(echo) 옵션이 주어진 경우 client에게 그대로 보냄.


- server는 "-b"(broadcast) 옵션이 주어진 경우 접속되어 있는 모든 client에게 메세지를 보냄.


- client는 server로부터 메세지를 받으면 화면에 출력.


- server는 여러개 client의 접속 요청 및 데이터 처리가 가능(- thread).
