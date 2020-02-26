//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_PLAYER_H
#define NETLIB_PLAYER_H

#include <string>
#include "protocol.h"

enum class PlayerStatus
{
    HALL,
    ROOM_NOT_READY,
    ROOM_READY,
    ROOM_PLAYING,
    ROOM_PLAY_OVER
};

class Gameroom;
class Player
{
public:
    int id = -1;
    Gameroom *room;

    Player(int fd, const char *ip, const int &port);

    bool ChangePlayerStatus(PlayerStatus status);

    PlayerStatus GetPlayerStatus();

    void SetStatus(PlayerStatus status);

    char *GetPortAndIpCharArray();
    // 对应的二进制协议解析
    Net::ProtoOperate proto_operate;

private:
    const char *ip_;
    int port_;
    PlayerStatus status_ = PlayerStatus::HALL;

};
#endif //NETLIB_PLAYER_H