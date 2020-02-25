//
// Created by lsmg on 2/23/20.
//

#include <reactor.h>
#include <util.h>
#include <listener.h>
#include <unistd.h>
#include <fcntl.h>
#include <log.h>
#include <http.h>
#include <cstring>
#include <eventhandler.h>

void LisenerCb(Net::EventHandler *handler, void* arg)
{
    char buffer[1024]{};
    getcwd(buffer, sizeof(buffer));
    strcat(buffer, "/out.txt");

    int file = open(buffer, O_RDONLY);
    ERROR_IF(file == -1, "open %s", buffer);

    char outbuffer[1024];

    std::string out;
    while(read(file, outbuffer, sizeof(outbuffer)) != 0)
    {
        out.append(outbuffer);
        memset(outbuffer, 0, sizeof(outbuffer));
    }

    Net::Http::SendHttpResponse(handler->fd_, out);
    close(handler->fd_);
}
int main(int argc, char* argv[])
{
    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);
    int port = Net::GetPortFromArg(argc, argv);

    auto *reactor = new Net::Reactor();
    auto *listener = new Net::Listener(reactor, LisenerCb, port);

    reactor->ReactorDispatch();

}