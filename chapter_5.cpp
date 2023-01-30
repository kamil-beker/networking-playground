/*
 * usage: showip hostname
 *
 * $ showip google.com
 * IP Addresses for: google.com
 * --> [IPv4] 172.217.18.110
 *
 * $ showip 2607:f8b0:4005:802::1007
 * IP Addresses for: 2607:f8b0:4005:802::1007
 * --> [IPv6] 2607:f8b0:4005:802::1007
 */
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        std::cerr << "usage: showip hostname" << std::endl;
        return 1;
    }

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return 2;
    }

    std::cout << "IP Addresses for: " << argv[1] << std::endl;

    for (p = res; p != nullptr; p = p->ai_next)
    {
        void *addr = nullptr;
        std::string ipver = "";

        if (p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout << "--> [" << ipver << "] " << ipstr << std::endl;
    }

    freeaddrinfo(res);

    return 0;
}
