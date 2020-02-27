//
// Created by lsmg on 2/25/20.
//

#include <cstring>
#include <sys/socket.h>
#include "protocol.h"

uint8_t* Net::ProtoOperate::Encode(Net::ProtoMsg *msg, uint32_t *len)
{
    *len = msg->header.body_len + HEADER_SIZE;

    auto *data = new uint8_t[*len];
    memset(data, 0, *len);

    *data = msg->header.msg_type;
    data++;

    *data = msg->header.msg_version;
    data++;

    *(uint32_t*)data = msg->header.body_len;
    data += 4;

    memcpy(data, msg->body, msg->header.body_len);

    // 回退增加的部分
    return data - HEADER_SIZE;
}

bool Net::ProtoOperate::Decode(uint8_t *data, uint32_t len)
{
    // 防止沾包 直接将受到的数据全部按顺序存储
    for (int i = 0; i < len; ++i)
    {
        reserved_data.push_back(data[i]);
    }

    uint32_t r_len = reserved_data.size();
    uint8_t *r_data = reserved_data.data();
    uint32_t parse_len = 0;

    while (r_len > 0)
    {
        switch (parse_status)
        {
            case ParseStatus::PARSE_INIT:
                current_msg = new ProtoMsg();
                parse_status = ParseStatus::PARSE_HEAD;

            case ParseStatus::PARSE_HEAD:
                if (!ParseHeader(&r_data, &r_len, &parse_len))
                {
                    return false;
                }
            case ParseStatus::PARSE_BODY:
                if (!ParseBody(&r_data, &r_len, &parse_len))
                {
                    return false;
                }
                msg_queue.push(current_msg);
                current_msg = nullptr;
        }
    }

    reserved_data.erase(reserved_data.begin(), reserved_data.begin() + parse_len);
    return true;
}

bool Net::ProtoOperate::ParseHeader(uint8_t **r_data, uint32_t *reserved_len, uint32_t *parse_len)
{
    if (*reserved_len < HEADER_SIZE)
    {
        return false;
    }

    uint8_t *data = *r_data;

    current_msg->header.msg_type = *data;
    data++;

    current_msg->header.msg_version = *data;
    data++;

    current_msg->header.body_len = *(uint32_t*)data;

    *reserved_len -= HEADER_SIZE;
    *parse_len += HEADER_SIZE;
    *r_data += HEADER_SIZE;

    parse_status = ParseStatus::PARSE_BODY;
    return true;
}

bool Net::ProtoOperate::ParseBody(uint8_t **r_data, uint32_t *reserved_len, uint32_t *parse_len)
{
    if (*reserved_len < current_msg->header.body_len)
    {
        return false;
    }

    uint8_t *data = *r_data;
    current_msg->body = data;

    *reserved_len -= current_msg->header.body_len;
    *parse_len += current_msg->header.body_len;
    // 这里不能写 *data 调试了半天。。。。。
    *r_data += current_msg->header.body_len;

    parse_status = ParseStatus::PARSE_INIT;
    return true;
}

Net::ProtoMsg *Net::ProtoOperate::Front()
{
    return msg_queue.front();
}

void Net::ProtoOperate::Pop()
{
    msg_queue.pop();
}

bool Net::ProtoOperate::IsEmpty()
{
    return msg_queue.empty();
}

Net::ProtoOperate::ProtoOperate()
{

}

Net::ProtoOperate::~ProtoOperate()
{

}
