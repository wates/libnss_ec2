
#include <iostream>
#include "instances.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int len=0;
    namemap *names = ListInstances(&len);
    for (int i = 0; i < len; i++){
        unsigned int hs = htonl(names[i].address);
        std::cout << inet_ntoa(*reinterpret_cast<in_addr*>(&hs)) << " " << names[i].name << std::endl;
    }
    FreeMap(names);

    return 0;
}