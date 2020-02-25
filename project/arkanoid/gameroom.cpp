//
// Created by lsmg on 2/25/20.
//

#include "gameroom.h"

Gameroom::Gameroom()
{
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
            return true;
        }
    }
    return false;
}

bool Gameroom::ExitRoom(Player *player)
{
    if (player->ChangePlayerStatus(PlayerStatus::ROOM_NOT_READY))
    {
        for (int i = 0; i < MAX_PLAYERS; ++i)
        {
            if (players[i]->id == player->id)
            {
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
