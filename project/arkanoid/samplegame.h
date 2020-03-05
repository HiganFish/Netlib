//
// Created by lsmg on 2/25/20.
//

#ifndef SIMPLE_GAME_H
#define SIMPLE_GAME_H

#include <game/game.h>

class SimpleGame :public LGame::Game
{
public:
    explicit SimpleGame(int RoomMaxPlayer);

    void Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data) override;

private:
    LGame::PlayerInfo *InitPlayerInfo(int fd, const char *ip, int port) override;

    void EnterRoom(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    bool CheckPlayerInfo(LGame::PlayerInfo *player_info) override;

    void ExitRoom(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void CheckRoomPlayerNum(LGame::PlayerInfo *player_info) override;

    void Ready(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void CancelReady(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len) override;

    void GameStart(LGame::PlayerInfo *playerInfo, const uint8_t *data, const int &data_len) override;

    void GamePlay(LGame::PlayerInfo *playerInfo, const uint8_t *data, const int &data_len) override;

    void SampleRespone(LGame::PlayerInfo *playerinfo, LGame::MsgType type, bool result, bool broadcast) override;

    void
    Response(LGame::PlayerInfo *playerinfo, LGame::MsgType msg_type, LGame::MsgVersion msg_ver, uint8_t *body, uint32_t length) override;
};

#endif //NETLIB_GAME_H
