//
// Created by lsmg on 2/25/20.
//

#include "game/gameroom.h"

int LGame::Gameroom::MAX_PLAYERS;

LGame::Gameroom::Gameroom(int roomid)
{
    roomid_ = roomid;
    player_num_ = 0;
    player_vector_ = new std::vector<Player*>;

}

bool LGame::Gameroom::EnterRoom(Player *player)
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
        if (player_temp->GetId() == player->GetId())
        {
            return false;
        }
    }
    player_num_++;
    player_vector_->push_back(player);
    return true;
}

bool LGame::Gameroom::ExitRoom(Player *player)
{
    if (player->ChangePlayerStatus(PlayerStatus::HALL))
    {
        int i = 0;
        for(auto player_temp : *player_vector_)
        {
            if (player_temp->GetFd() == player->GetFd())
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

bool LGame::Gameroom::CanStartGame()
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


int LGame::Gameroom::GetRoomid()
{
    return roomid_;
}

int LGame::Gameroom::GetPlayerNum()
{
    return player_num_;
}

std::vector<LGame::Player *> *LGame::Gameroom::GetPlayerVector() const
{
    return player_vector_;
}

void LGame::Gameroom::Init(int max_players)
{
    MAX_PLAYERS = max_players;
}
