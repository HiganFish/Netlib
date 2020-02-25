//
// Created by lsmg on 2/25/20.
//

#include "player.h"

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
