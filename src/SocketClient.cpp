#include <stdio.h>

#include <iostream>
#include <thread>

#include "InitSock.h"
using namespace std;

namespace myclient {

//用于异步消息接收
void recvFunc(SOCKET& clientSocket) {
  while (true) {
    char receiveBuf[200] = "\0";
    if (recv(clientSocket, receiveBuf, sizeof(receiveBuf), 0) < 0) {
      std::cout << "服务器断线" << std::endl;
      return;
    }
    std::cout << receiveBuf << '\n' << std::endl;
  }
}
int startClient() {
  // 创建套节字
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) {
    printf(" Failed socket() \n");
    return 0;
  }

  // 也可以在这里调用bind函数绑定一个本地地址
  // 否则系统将会自动安排

  // 填写远程地址信息
  sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(12345);
  // 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
  // 如果你的计算机没有联网，直接使用127.0.0.1即可
  servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  if (connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
    printf(" Failed connect() \n");
    return 0;
  }
  
  char buff[256];
  char szText[256];

  
  //连接成功后先启动接收消息的线程
  thread handler(recvFunc, std::ref(s));
  handler.detach();
  
  while (TRUE) {
 

    // 向服务器端发送数据

    std::cin >> szText;
    szText[255] = '\0';
    send(s, szText, strlen(szText), 0);
  }

  // 关闭套节字
  closesocket(s);
  return 0;
}
}  // namespace myclient
