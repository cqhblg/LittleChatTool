#include <iostream>
using namespace std;
namespace myclient {
int extern startClient();
}
namespace myserver {
int extern startServer();
}
int main() {
  string str;
  cin >> str;
  if (str == "client") {
    if (myclient::startClient() == 0) {
      cout << "start server success!" << endl;
    }
  } else if (str == "server") {
    if (myserver::startServer() == 0) {
      cout << "start client success!" << endl;
    }
  }
}