//
// Created by lsmg on 3/5/20.
//

#include "game/game.h"

LGame::Game::Game(int GameroomMaxPlayer)
{
    LGame::Gameroom::Init(GameroomMaxPlayer);
    room_map_ = new std::map<int, Gameroom*>;
    player_info_map_ = new std::map<int, PlayerInfo*>;
}

LGame::Game::~Game()
{

}

void LGame::Game::Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data)
{

}

LGame::PlayerInfo * LGame::Game::InitPlayerInfo(int fd, const char *ip, int port)
{
    return nullptr;
}

void LGame::Game::EnterRoom(PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{

}

bool LGame::Game::CheckPlayerInfo(PlayerInfo *player_info)
{
    return false;
}

void LGame::Game::ExitRoom(PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{

}

void LGame::Game::CheckRoomPlayerNum(PlayerInfo *player_info)
{

}

void LGame::Game::Ready(PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{

}

void LGame::Game::CancelReady(PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{

}

void LGame::Game::GameStart(PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{

}

void LGame::Game::GamePlay(PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{

}

void LGame::Game::SampleRespone(PlayerInfo *player_info, MsgType type, bool result, bool broadcast)
{

}

void LGame::Game::Response(PlayerInfo *player_info, MsgType msg_type, MsgVersion msg_ver, uint8_t *body, uint32_t length)
{

}

LGame::PlayerInfo *LGame::Game::GetPlayerInfoByFd(int fd)
{
    auto playerinfo = player_info_map_->find(fd);
    if (playerinfo == player_info_map_->end())
    {
        return nullptr;
    }

    return playerinfo->second;
}

void LGame::Game::DeleteRoom(LGame::PlayerInfo *player_info)
{
    room_map_->erase(player_info->room->GetRoomid());
    delete player_info->room;
    player_info->room = nullptr;
}

LGame::PlayerInfo * LGame::Game::AddPlayerInfo(int fd, const char *ip, int port)
{
    auto info = new PlayerInfo;
    info->player = new Player(fd, ip, port);
    info->room = nullptr;
    player_info_map_->insert(std::pair<int, LGame::PlayerInfo*>(fd, info));
    return info;
}

LGame::Gameroom *LGame::Game::GetGameroom(int roomid)
{
    auto gameroom = room_map_->find(roomid);
    if (gameroom == room_map_->end())
    {
        return nullptr;
    }
    return gameroom->second;
}

LGame::Gameroom * LGame::Game::CreateNewGameroom(int roomid)
{
    auto room = new LGame::Gameroom(roomid);
    room_map_->insert(std::pair<int, LGame::Gameroom*>(roomid, room));
    return room;
}
