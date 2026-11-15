# Lab 1 — IPC (client/server, ping-pong)

Два приложения на C: клиент шлёт ping, сервер отвечает pong по TCP.

## Статус
- [x] Окружение: WSL Ubuntu, gcc - [x] TCP server (socket, bind, listen, accept) - [x] TCP client (socket, connect) - [x] 
Обмен ping / pong - [x] Базовая обработка ошибок - [ ] Состояния по диаграмме - [ ] Чат + история (опционально) - [ ] 
CMake (опционально)

## Сборка
gcc server.c -o server
gcc client.c -o client

## Запуск
Сначала сервер, потом клиент (два терминала):
./server
./client

Сервер слушает порт 8080 на localhost (127.0.0.1).

## Ожидаемый вывод
Сервер:
Server listening on port 8080
Client connected
Got: ping
Sent: pong
Done

Клиент:
Connected to server
Sent: ping
Got: pong
Done
