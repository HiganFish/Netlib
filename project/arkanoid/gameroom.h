//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_GAMEROOM_H
#define NETLIB_GAMEROOM_H

#define MAX_PLAYERS 2

#include "player.h"

class Gameroom
{
public:
    explicit Gameroom(int roomid);
    bool EnterRoom(Player *player);

    bool ExitRoom(Player *player);

    bool CanStartGame();

    int GetRoomid();

    int GetPlayerNum();
private:
    Player **players;

    int player_num_ = 0;
    int roomid_ = -1;
};
#endif //NETLIB_GAMEROOM_H
