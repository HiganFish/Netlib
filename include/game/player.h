//
// Created by lsmg on 2/25/20.
//

#ifndef NETLIB_PLAYER_H
#define NETLIB_PLAYER_H

#include <string>

namespace LGame
{
enum class PlayerStatus
{
    HALL,
    ROOM_NOT_READY,
    ROOM_READY,
    ROOM_PLAYING,
    ROOM_PLAY_OVER
};

class Gameroom;
class Player
{
public:
    Player(int id, int fd, const char *ip, int port);

    bool ChangePlayerStatus(PlayerStatus status);

    PlayerStatus GetPlayerStatus();

    void SetStatus(PlayerStatus status);

    void GetIpAndPort(const char *ip, const int *port);

    int GetId() const;

    void SetId(int id);

    int GetFd() const;

    void SetFd(int fd);

private:
    // 玩家个人id
    int id_ = 0;
    // 玩家连接的描述符
    int fd_ = -1;

    const char *ip_;
    int port_;
    PlayerStatus status_ = PlayerStatus::HALL;

    Player(const Player&);
    Player& operator=(const Player&);
};
}
#endif //NETLIB_PLAYER_H
