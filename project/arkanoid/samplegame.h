//
// Created by lsmg on 2/25/20.
//

#ifndef SIMPLE_GAME_H
#define SIMPLE_GAME_H

#include <game/game.h>
#include <network/protocol.h>

class SimpleGame :public LGame::Game
{
public:
    explicit SimpleGame(int RoomMaxPlayer);

    void Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data) override;

    inline void GameInfo(LGame::PlayerInfo *info, const char *ctl, bool result)
    {
        LOG_INFO("[id: %d][roomid: %d] %s %s", info->player->GetId(), info->room->GetRoomid(), ctl, result?"successfully":"failed");
    }

    inline void GameDebug(LGame::Player *player, const char *ctl)
    {
        LOG_DEBUG("[id: %d] %s", player->GetId(), ctl);
    }

    inline void GameDebug(LGame::PlayerInfo *info, const char *ctl)
    {
        LOG_DEBUG("[id: %d] %s", info->player->GetId(), ctl);
    }
private:

    Net::Protocol *protos;


    LGame::PlayerInfo *InitPlayerInfo(int id, int fd, const char *ip, int port) override;

    void EnterRoom(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    bool CheckPlayerInfo(LGame::PlayerInfo *player_info) override;

    void ExitRoom(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void CheckRoomPlayerNum(LGame::PlayerInfo *player_info) override;

    void Ready(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void CancelReady(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void GameStart(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void GamePlay(LGame::PlayerInfo *playerInfo, const uint8_t *data, const int &data_len) override;

    void SampleRespone(LGame::PlayerInfo *playerinfo, LGame::MsgType type, bool result, bool broadcast) override;

    void Response(LGame::PlayerInfo *playerinfo, LGame::MsgType msg_type, LGame::MsgVersion msg_ver, uint8_t *body, uint32_t length) override;
};

#endif //NETLIB_GAME_H
