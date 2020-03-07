//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_REACTOR_H
#define NETLIB_REACTOR_H

#include <array>
#include <queue>


namespace Net
{

// 可读 可写 新连接
enum io_evnet_type : uint8_t
{
    IO_READ = 1,
    IO_WRITE = 2,
    IO_CLOSE = 3
};
struct io_event
{
    int fd;
    io_evnet_type event_type;
};

class OpBase;
class EventHandler;
class Reactor
{
public:

    std::queue<io_event> *io_queue;

    Reactor();

    void AddEventHandler(EventHandler* event_handler);

    int ReactorDispatch();

    void SetLisenerFd(int fd);
private:

    OpBase* op_base_;

    // 本次循环完成后退出
    int reactor_break_later_;

    // 立即退出
    bool reactor_break_;

    bool reactor_continue_;

    // 是否正在运行一个循环
    bool running_loop_;

    // 循环
    int ReactorLoop(int flags);

    // 处理事件
    int EventProcess();

    // 建立fd 和EentHandler的对应关系
    std::array<EventHandler*, 5000> *event_handler_array;

    int lisenfd_;
};
}
#endif //NETLIB_REACTOR_H
