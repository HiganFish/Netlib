//
// Created by lsmg on 2/25/20.
//

#include "gameroom.h"

Gameroom::Gameroom(int roomid)
{
    roomid_ = roomid;
    players = new Player*[MAX_PLAYERS];
}

bool Gameroom::EnterRoom(Player *player)
{
    if (player->GetPlayerStatus() != PlayerStatus::HALL)
    {
        return false;
    }
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        if (players[i] == nullptr)
        {
            players[i] = player;
            player->SetStatus(PlayerStatus::ROOM_NOT_READY);

            player_num_++;

            player->room = this;
            return true;
        }
    }
    return false;
}

bool Gameroom::ExitRoom(Player *player)
{
    if (player->ChangePlayerStatus(PlayerStatus::HALL))
    {
        for (int i = 0; i < MAX_PLAYERS; ++i)
        {
            if (players[i]->id == player->id)
            {
                player_num_--;
                delete players[i];
                return true;
            }
        }
    }
    return false;
}

bool Gameroom::CanStartGame()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        if (players[i]->GetPlayerStatus() != PlayerStatus::ROOM_READY)
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
