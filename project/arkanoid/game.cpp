//
// Created by lsmg on 2/25/20.
//

#include <log.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <protocol.h>
#include "game.h"

Game::Game()
{
    room_map_ = new std::map<int, Gameroom*>;
    player_map = new std::map<int, Player*>;
}

void Game::Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data)
{
    Player *player = InitPlayer(fd, ip, port);

    if (!player->proto_operate.Decode(data, data_len))
    {
        return;
    }

    while(!player->proto_operate.IsEmpty())
    {
        Net::ProtoMsg *msg = player->proto_operate.Front();
        player->proto_operate.Pop();

        if (msg->header.msg_version != VERSION1)
        {
            LOG_WARN("fd:%d from %s invaild msgver: %d", fd, player->GetPortAndIpCharArray(), msg->header.msg_version);
            continue;
        }

        switch (static_cast<MsgType>(msg->header.msg_type))
        {
            case MsgType::ROOM_ENTER:
                RoomEnter(player, msg->body, msg->header.body_len);
                break;
            case MsgType::ROOM_EXIT:
                break;
            case MsgType::PLAYER_READY:
                break;
            case MsgType::PLAYER_CANCEL_READY:
                break;
            case MsgType::GAME_START:
                break;
            case MsgType::ROOM_PLAY_DATA:
                break;
            case MsgType::GAME_OVER:
                break;
            default:
                LOG_WARN("fd:%d from %s invaild msgtype: %d", fd, player->GetPortAndIpCharArray(), msg->header.msg_type);
        }
    }
}


Player *Game::InitPlayer(const int &fd, const char *ip, const int &port)
{
    auto ret = player_map->find(fd);
    if (ret != player_map->end())
    {
        return ret->second;
    }

    auto player = new Player(fd, ip, port);
    player_map->insert(std::pair<int, Player*>(fd, player));
    return player;
}

void Game::RoomEnter(Player *player, const uint8_t *data, const int &data_len)
{
    uint32_t roomid = *(uint32_t*)data;

    LOG_INFO("fd:%d from %s EnterRoom id:%d", player->id, player->GetPortAndIpCharArray(), roomid);
}
