#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <atlstr.h>

#define WINSOCKVERSION MAKEWORD(2, 2)

using std::string;
using std::cout;
using std::wcout;
using std::cin;
using std::endl;
using std::thread;

char utf8[100];
//utf8转换函数
char *fl_str(wchar_t* wstr) {
    const size_t MAX = 1024;
    char* str = (char*)malloc(1024);
    fl_utf8fromwc(str, MAX, wstr, wcslen(wstr));
    return str;
}

SOCKET clientSocket;
//用于异步消息接收
void recvFunc(SOCKET& clientSocket, Fl_Output* output) {
    while (true) {
        char receiveBuf[200] = "\0";
        if (recv(clientSocket, receiveBuf, sizeof(receiveBuf), 0) < 0) {
            cout << "服务器断线" << endl;
            return;
        }
        cout << receiveBuf << '\n' << endl;
        char text[1024] = "";
        strcpy(text, output->value());
        strcat(text, "\n");
        strcat(text, receiveBuf);
        Fl::lock();
        output->value(text);
        Fl::unlock();
        Fl::awake();
    }
}

//用于消息发送
void sendFunc(Fl_Button* btn, void* data) {
    Fl_Input* input = (Fl_Input*)data;
    wchar_t* af = (wchar_t*)malloc(1024);
    fl_utf8towc(input->value(), 256, af, 1024);
    CString ttt = TEXT(af);
    send(clientSocket, ttt, strlen(ttt) + 1, 0);
}

int main11(int argc, char** argv) {

    //链接ws2_32.lib，创建套接字，这是接下来绑定socket的必要步骤
    WSADATA wsaData;
    if (WSAStartup(WINSOCKVERSION, &wsaData)) {
        cout << "ERROR: WSAStartup can not open" << endl;
        return 1;
    }

    //进一步绑定套接字，初始化参数
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    cout << "欢迎来到匿名聊天系统, 输入想说的话，按下回车就可以发送消息了" << endl;
    Fl_Window* window = new Fl_Window(640, 400);
    Fl_Multiline_Output* output = nullptr;
    Fl_Button* button = nullptr;
    Fl_Input* input = nullptr;
    //连接成功后先启动接收消息的线程
    thread handler(recvFunc, std::ref(clientSocket), std::ref(output));
    handler.detach();

    /*
    closesocket(clientSocket);
    WSACleanup();
    */
    Fl::lock();
    output = new Fl_Multiline_Output(20, 50, 600, 260, "ouput");
    const char aaa[] = "发送";
    CString bbb = TEXT(aaa);
    cout << bbb << endl;
    button = new Fl_Button(540, 320, 80, 30, fl_str(bbb.AllocSysString()));
    input = new Fl_Input(20, 320, 500, 30);
    output->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    button->callback((Fl_Callback*)sendFunc, input);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}