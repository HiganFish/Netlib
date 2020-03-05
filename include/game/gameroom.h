//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_GAMEROOM_H
#define NETLIB_GAMEROOM_H

#include <vector>
#include "game/player.h"
namespace LGame
{
class Gameroom
{
public:
    explicit Gameroom(int roomid);

    static void Init(int max_players);

    bool EnterRoom(Player *player);

    bool ExitRoom(Player *player);

    bool CanStartGame();

    int GetRoomid();

    int GetPlayerNum();

    std::vector<Player*> *GetPlayerVector() const;


private:
    static int MAX_PLAYERS;
    std::vector<Player*> *player_vector_;

    int player_num_;
    int roomid_;

    Gameroom& operator=(const Gameroom&);
};
}
#endif //NETLIB_GAMEROOM_H
