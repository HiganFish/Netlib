//
// Created by lsmg on 2/25/20.
//

#include "game/player.h"
#include "network/log.h"
#include <cstring>

bool LGame::Player::ChangePlayerStatus(PlayerStatus status)
{
    switch (status)
    {
        case PlayerStatus::HALL:
            if ((status_ == PlayerStatus::ROOM_NOT_READY) || (status_ == PlayerStatus::ROOM_PLAY_OVER))
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_NOT_READY:
            if ((status_ == PlayerStatus::ROOM_READY) || (status_ == PlayerStatus::HALL))
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_READY:
            if (status_ == PlayerStatus::ROOM_NOT_READY)
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_PLAYING:
            if (status_ == PlayerStatus::ROOM_READY)
            {
                status_ = status;
                return true;
            }
            return false;
        case PlayerStatus::ROOM_PLAY_OVER:
            if (status_ == PlayerStatus::ROOM_PLAYING)
            {
                status_ = status;
                return true;
            }
            return false;
    }
}

LGame::PlayerStatus LGame::Player::GetPlayerStatus()
{
    return status_;
}

void LGame::Player::SetStatus(PlayerStatus status)
{
    status_ = status;
}

LGame::Player::Player(int fd, const char *ip, int port) :
        fd_(fd), ip_(ip), port_(port)
{
    status_ = PlayerStatus::HALL;
    LOG_INFO("create new player fd: %d ip_: %s port: %d", fd, ip, port);
}

void LGame::Player::GetIpAndPort(const char *ip, const int *port)
{
    ip = ip_;
    port = &port_;
}

int LGame::Player::GetId() const
{
    return id_;
}

void LGame::Player::SetId(int id)
{
    id_ = id;
}

int LGame::Player::GetFd() const
{
    return fd_;
}

void LGame::Player::SetFd(int fd)
{
    Player::fd_ = fd;
}
