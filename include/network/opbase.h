//
// Created by lsmg on 2/20/20.
//

#ifndef NETLIB_OPBASE_H
#define NETLIB_OPBASE_H

#include <map>

namespace Net {

class OpBase
{
public:
    OpBase();
    virtual ~OpBase();

    // 添加事件
    virtual bool Add(int fd, int option, int event_type);

    // 删除事件
    virtual bool Del(int fd, int option, int event_type);

    // 循环
    virtual bool Dispatch(int time);

    // 清除使用空间
    virtual void Dealloc();

    std::map<int, int> *io_map_;
};
}
#endif //NETLIB_OPBASE_H
