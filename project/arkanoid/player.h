//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_PLAYER_H
#define NETLIB_PLAYER_H
enum class PlayerStatus
{
    HALL,
    ROOM_NOT_READY,
    ROOM_READY,
    ROOM_PLAYING
};

class Player
{
public:
    int id = -1;

    bool ChangePlayerStatus(PlayerStatus status);

    PlayerStatus GetPlayerStatus();

    void SetStatus(PlayerStatus status);
private:

    PlayerStatus status_ = PlayerStatus::HALL;

};
#endif //NETLIB_PLAYER_H
