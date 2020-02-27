//
// Created by lsmg on 2/25/20.
//

#include "gameroom.h"

Gameroom::Gameroom(int roomid)
{
    roomid_ = roomid;
    player_vector_ = new std::vector<Player*>;

}

bool Gameroom::EnterRoom(Player *player)
{
    if (player_num_ >= MAX_PLAYERS)
    {
        return false;
    }
    if (!player->ChangePlayerStatus(PlayerStatus::ROOM_NOT_READY))
    {
        return false;
    }
    for(Player *player_temp : *player_vector_)
    {
        if (player_temp->fd == player->fd)
        {
            return false;
        }
    }
    player_num_++;
    player_vector_->push_back(player);
    return true;
}

bool Gameroom::ExitRoom(Player *player)
{
    if (player->ChangePlayerStatus(PlayerStatus::HALL))
    {
        int i = 0;
        for(auto player_temp : *player_vector_)
        {
            if (player_temp->fd == player->fd)
            {
                player_num_--;
                delete player_temp;
                player_vector_->erase(player_vector_->begin() + i);
                return true;
            }
            i++;
        }
    }
    return false;
}

bool Gameroom::CanStartGame()
{
    if (player_num_ != MAX_PLAYERS)
    {
        return false;
    }
    for(auto player_temp : *player_vector_)
    {
        if (player_temp->GetPlayerStatus() != PlayerStatus::ROOM_READY)
        {
            return false;
        }
    }
    return true;
}


int Gameroom::GetRoomid()
{
    return roomid_;
}

int Gameroom::GetPlayerNum()
{
    return player_num_;
}

std::vector<Player *> *Gameroom::GetPlayerVector() const
{
    return player_vector_;
}
