#include <stdio.h>

#include <iostream>
#include <thread>

#include "InitSock.h"
using namespace std;

namespace myclient {

//�����첽��Ϣ����
void recvFunc(SOCKET& clientSocket) {
  while (true) {
    char receiveBuf[200] = "\0";
    if (recv(clientSocket, receiveBuf, sizeof(receiveBuf), 0) < 0) {
      std::cout << "����������" << std::endl;
      return;
    }
    std::cout << receiveBuf << '\n' << std::endl;
  }
}
int startClient() {
  // �����׽���
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) {
    printf(" Failed socket() \n");
    return 0;
  }

  // Ҳ�������������bind������һ�����ص�ַ
  // ����ϵͳ�����Զ�����

  // ��дԶ�̵�ַ��Ϣ
  sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(12345);
  // ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ
  // �����ļ����û��������ֱ��ʹ��127.0.0.1����
  servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  if (connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
    printf(" Failed connect() \n");
    return 0;
  }
  
  char buff[256];
  char szText[256];

  
  //���ӳɹ���������������Ϣ���߳�
  thread handler(recvFunc, std::ref(s));
  handler.detach();
  
  while (TRUE) {
 

    // ��������˷�������

    std::cin >> szText;
    szText[255] = '\0';
    send(s, szText, strlen(szText), 0);
  }

  // �ر��׽���
  closesocket(s);
  return 0;
}
}  // namespace myclient
