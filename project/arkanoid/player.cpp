//
// Created by lsmg on 2/25/20.
//

#include "player.h"

#include <utility>
#include <cstring>

bool Player::ChangePlayerStatus(PlayerStatus status)
{
    switch (status_)
    {
        case PlayerStatus::HALL:
            if (status == PlayerStatus::ROOM_NOT_READY)
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_NOT_READY:
            if (status == PlayerStatus::ROOM_READY)
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_READY:
            if (status == PlayerStatus::ROOM_PLAYING)
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_PLAYING:
            if (status == PlayerStatus::HALL)
            {
                status_ = status;
                return true;
            }
            return false;
    }
}

PlayerStatus Player::GetPlayerStatus()
{
    return status_;
}

void Player::SetStatus(PlayerStatus status)
{
    status_ = status;
}

Player::Player(int fd, const char *ip, const int &port)
{
    id = fd;
    ip_ = ip;
    port_ = port;
}

char *Player::GetPortAndIpCharArray()
{
    char *buffer = new char[strlen(ip_) + 8]{};

    strcpy(buffer, ip_);
    buffer[strlen(ip_)] = ':';
    sprintf(buffer + strlen(ip_) + 1, "%d", port_);

    return buffer;
}
