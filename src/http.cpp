//
// Created by lsmg on 2/23/20.
//

#include <http.h>
#include <util.h>
#include <sys/socket.h>

std::string Net::Http::header = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n";

void Net::Http::SendHttpResponse(int fd, std::string body)
{

    // 不同系统下换行不同
    Net::ReplaceAll(body, "\r\n", "<br>");
    Net::ReplaceAll(body, "\n", "<br>");
    std::string pack = header + "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
    send(fd, pack.c_str(), pack.length(), 0);
}

