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
    explicit Gameroom();
    bool EnterRoom(Player *player);

    bool ExitRoom(Player *player);

    bool CanStartGame();
private:
    Player **players;

};
#endif //NETLIB_GAMEROOM_H
