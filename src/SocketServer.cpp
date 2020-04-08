#include <stdio.h>

#include <iostream>

#include "InitSock.h"
CInitSock initSock;  // ��ʼ��Winsock��

namespace myserver {
int startServer() {
  // �����׽���
  SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sListen == INVALID_SOCKET) {
    printf("Failed socket() \n");
    return 0;
  }

  // ���sockaddr_in�ṹ
  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(12345);
  sin.sin_addr.S_un.S_addr = INADDR_ANY;

  // ������׽��ֵ�һ�����ص�ַ
  if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
    printf("Failed bind() \n");
    return 0;
  }

  // �������ģʽ
  if (listen(sListen, 2) == SOCKET_ERROR) {
    printf("Failed listen() \n");
    return 0;
  }

  // ѭ�����ܿͻ�����������
  sockaddr_in remoteAddr;
  int nAddrLen = sizeof(remoteAddr);
  SOCKET sClient1 = 0;
  SOCKET sClient2 = 0;
  char szText[] = " ���ӳɹ������Կ�ʼ�������� \r\n";
  while (sClient1 == 0) {
    // ����һ��������
    sClient1 = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
    if (sClient1 == INVALID_SOCKET) {
      printf("Failed accept()");
    }

    printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    continue;
  }
  while (sClient2 == 0) {
    // ����һ��������
    sClient2 = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
    if (sClient2 == INVALID_SOCKET) {
      printf("Failed accept()");
    }

    printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    continue;
  }
  bool first = true;
  while (TRUE) {
    if (first) {
      send(sClient1, szText, strlen(szText), 0);
      send(sClient2, szText, strlen(szText), 0);
      first = false;
    }
    // �ӿͻ��˽�������
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

  // �ر�ͬ�ͻ��˵�����
  closesocket(sClient1);
  closesocket(sClient2);

  // �رռ����׽���
  closesocket(sListen);

  return 0;
}
}  // namespace myserver
