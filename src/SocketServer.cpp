#include <stdio.h>

#include <iostream>

#include "InitSock.h"
CInitSock initSock;  // 初始化Winsock库

namespace myserver {
int startServer() {
  // 创建套节字
  SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sListen == INVALID_SOCKET) {
    printf("Failed socket() \n");
    return 0;
  }

  // 填充sockaddr_in结构
  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(12345);
  sin.sin_addr.S_un.S_addr = INADDR_ANY;

  // 绑定这个套节字到一个本地地址
  if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
    printf("Failed bind() \n");
    return 0;
  }

  // 进入监听模式
  if (listen(sListen, 2) == SOCKET_ERROR) {
    printf("Failed listen() \n");
    return 0;
  }

  // 循环接受客户的连接请求
  sockaddr_in remoteAddr;
  int nAddrLen = sizeof(remoteAddr);
  SOCKET sClient1 = 0;
  SOCKET sClient2 = 0;
  char szText[] = " 连接成功，可以开始聊天啦！ \r\n";
  while (sClient1 == 0) {
    // 接受一个新连接
    sClient1 = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
    if (sClient1 == INVALID_SOCKET) {
      printf("Failed accept()");
    }

    printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    continue;
  }
  while (sClient2 == 0) {
    // 接受一个新连接
    sClient2 = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
    if (sClient2 == INVALID_SOCKET) {
      printf("Failed accept()");
    }

    printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    continue;
  }
  bool first = true;
  while (TRUE) {
    if (first) {
      send(sClient1, szText, strlen(szText), 0);
      send(sClient2, szText, strlen(szText), 0);
      first = false;
    }
    // 从客户端接收数据
    char buff1[256];
    char buff2[256];
    int nRecv1 = recv(sClient1, buff1, 256, 0);
    if (nRecv1 > 0) {
      buff1[nRecv1] = '\0';
      char name[] = "client1:";
      auto buf = strcat(name, buff1);
      printf("%s\n", buf);
      send(sClient2, buf, strlen(buf), 0);
    }
    int nRecv2 = recv(sClient2, buff2, 256, 0);
    if (nRecv2 > 0) {
      buff2[nRecv2] = '\0';
      char name[]= "client2:";
      auto buf = strcat(name, buff2);       
      printf("%s\n", buf);
      send(sClient1, buf, strlen(buf), 0);
    }
    
    
  }

  // 关闭同客户端的连接
  closesocket(sClient1);
  closesocket(sClient2);

  // 关闭监听套节字
  closesocket(sListen);

  return 0;
}
}  // namespace myserver
