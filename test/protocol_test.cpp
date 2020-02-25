//
// Created by lsmg on 2/25/20.
//
#include <protocol.h>
#include <log.h>

int main()
{
    Net::Log::SetLogger(Net::OUT_CONSOLE, Net::LOG_LEVEL_INFO);

    Net::ProtoMsg msg{};

    uint8_t test[] = {1, 2, 3, 4};

    msg.header.msg_type = Net::MsgType::ENTERROOM;
    msg.header.msg_version = Net::MsgVersion::VERSION_1;
    msg.header.body_len = sizeof(test);

    msg.body = test;

    Net::ProtoOperate operate;

    uint32_t len = 0;
    uint8_t *serialization = operate.Encode(&msg, &len);

    Net::ProtoMsg *deserizlization;

    if (operate.Decode(serialization, len))
    {
        deserizlization = operate.Front();
        operate.Pop();
        LOG_INFO("type: %d, version: %d, len: %d", deserizlization->header.msg_type,
                 deserizlization->header.msg_version, deserizlization->header.body_len);
        for (int i = 0; i < deserizlization->header.body_len; ++i)
        {
            printf("%hhu", *deserizlization->body);
        }
    }
    else
    {
        LOG_INFO("decode failed");
    }
}