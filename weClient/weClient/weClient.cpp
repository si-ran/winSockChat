﻿#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#pragma comment(lib,"ws2_32.lib")

#define WINSOCKVERSION MAKEWORD(2, 2)

using std::string;
using std::cout;
using std::endl;
using std::thread;

//用于异步消息接收
void recvFunc(SOCKET& clientSocket) {
    while (1) {
        char receiveBuf[100];
        recv(clientSocket, receiveBuf, 100, 0);
        printf("Recv:%s\n", receiveBuf);
    }
}


int main() {

    //链接ws2_32.lib，创建套接字，这是接下来绑定socket的必要步骤
    WSADATA wsaData;
    if (WSAStartup(WINSOCKVERSION, &wsaData)) {
        cout << "ERROR: WSAStartup can not open" << endl;
        return 1;
    }

    //进一步绑定套接字，初始化参数
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN clientsock_in;
    string ip = "127.0.0.1";
    inet_pton(AF_INET, ip.data(), (void*)&clientsock_in.sin_addr.s_addr);   //ip设定
    clientsock_in.sin_family = AF_INET;                                     //Ipv4
    clientsock_in.sin_port = htons(38888);                                  //端口号

    //客户端连接server
    if (connect(clientSocket, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "connect error" << endl;
        return 1;
    };

    //连接成功后先启动接收消息的线程
    thread handler(recvFunc, std::ref(clientSocket));
    handler.detach();

    //不断给server发送消息
    for (int i = 0; i < 10; i++) {
        char sendBuf[100] = "hello,this is client";
        cout << "Send: " << sendBuf << endl;
        send(clientSocket, sendBuf, strlen(sendBuf) + 1, 0);
        Sleep(5000);
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}