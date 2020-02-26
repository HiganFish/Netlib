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

#define VERSION1 1

class Game
{
public:
    Game();

    void Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data);

private:
    std::map<int, Gameroom*> *room_map_;
    std::map<int, Player*> *player_map;

    Player* InitPlayer(const int &fd, const char *ip, const int &port);

    void RoomEnter(Player *player, const uint8_t *data, const int &data_len);
};
#endif //NETLIB_GAME_H
