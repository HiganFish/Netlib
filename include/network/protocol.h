//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_PROTOCOL_H
#define NETLIB_PROTOCOL_H


#include <cstdint>
#include <vector>
#include <queue>

namespace Net
{
struct ProtoHead
{
    uint8_t msg_type;
    uint8_t msg_version;
    uint32_t body_len;
};
struct ProtoMsg
{
    ProtoHead header;
    uint8_t *body;
};

enum class ParseStatus
{
    PARSE_INIT,
    PARSE_HEAD,
    PARSE_BODY
};
class ProtoOperate
{
public:
    ProtoOperate();
    ~ProtoOperate();

    bool Decode(uint8_t *data, uint32_t len);
    uint8_t* Encode(ProtoMsg *msg, uint32_t *len);

    bool IsEmpty();
    ProtoMsg* Front();
    void Pop();

private:
    const static int HEADER_SIZE = 6;

    std::vector<uint8_t> reserved_data;

    ProtoMsg *current_msg;

    ParseStatus parse_status = ParseStatus::PARSE_INIT;

    std::queue<ProtoMsg*> msg_queue;

    bool ParseHeader(uint8_t **r_data, uint32_t *reserved_len, uint32_t *parse_len);

    bool ParseBody(uint8_t **r_data, uint32_t *reserved_len, uint32_t *parse_len);

    ProtoOperate(const ProtoOperate&);
    ProtoOperate& operator=(const ProtoOperate&);
};

}
#endif //NETLIB_PROTOCOL_H
