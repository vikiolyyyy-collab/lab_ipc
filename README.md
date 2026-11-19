# Lab 1 — IPC (client/server, chat)

Два приложения на C. Обмен по TCP (localhost, порт 8080).
Клиент присылает строки, сервер отвечает эхом. Выход: exit.

## Статус
- [x] Окружение: WSL Ubuntu, gcc
- [x] TCP server / client
- [x] Простой чат (эхо)
- [x] Выход по exit
- [x] Базовая обработка ошибок
- [x] CMake
- [ ] История переписки 

## Сборка (CMake)
mkdir -p build
cd build
cmake ..
make

## Запуск
Два терминала, сначала сервер:

./server

./client

Пишите сообщения в клиенте. Для выхода: exit

## Сборка вручную 
gcc server.c -o server
gcc client.c -o client
