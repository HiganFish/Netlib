//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_GAME_H
#define NETLIB_GAME_H

#include <map>
#include "gameroom.h"

enum class MsgType : uint8_t
{
ROOM_ENTER,
ROOM_EXIT,
PLAYER_READY,
PLAYER_CANCEL_READY,
GAME_START,
ROOM_PLAY_DATA,
GAME_OVER
};

class Game
{
public:
    Game();

    void Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data);

private:
    std::map<int, Gameroom*> *room_map_;
    std::map<int, Player*> *player_map;

    Player* InitPlayer(const int &fd, const char *ip, const int &port);

    void EnterRoom(Player *player, const uint8_t *data, const int &data_len);

    bool IsPlayerValid(Player *player, const char *format, ...);

    void ExitRoom(Player *player, const uint8_t *data, const int &data_len);
    void CheckRoomPlayerNum(Player *player);

    void Ready(Player *player, const uint8_t *data, const int &data_len);
    void CancelReady(Player *player, const uint8_t *data, const int &data_len);

    void GameStart(Player *player, const uint8_t *data, const int &data_len);

    void SampleRespone(Player *player, MsgType type, bool result, bool broadcast);
    void Response(Player *player, uint8_t msg_type, uint8_t msg_ver, uint8_t *body, uint32_t length);


    Game(const Game&);
    Game& operator=(const Game&);

    const static int VERSION1 = 1;
};
#endif //NETLIB_GAME_H
