//
// Created by lsmg on 2/25/20.
//

#include <log.h>
#include <netinet/in.h>
#include <protocol.h>
#include <cstdarg>
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
                EnterRoom(player, msg->body, msg->header.body_len);
                break;
            case MsgType::ROOM_EXIT:
                ExitRoom(player, msg->body, msg->header.body_len);
                break;
            case MsgType::PLAYER_READY:
                Ready(player, msg->body, msg->header.body_len);
                break;
            case MsgType::PLAYER_CANCEL_READY:
                CancelReady(player, msg->body, msg->header.body_len);
                break;
            case MsgType::GAME_START:
                GameStart(player, msg->body, msg->header.body_len);
                break;
            case MsgType::ROOM_PLAY_DATA:
                GamePlay(player, msg->body, msg->header.body_len);
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

void Game::EnterRoom(Player *player, const uint8_t *data, const int &data_len)
{
    uint32_t roomid = *(uint32_t*)data;
    data+=4;
    uint32_t playerid = *(uint32_t*)data;

    auto ret = room_map_->find(roomid);
    if (ret != room_map_->end())
    {
        // 房间已经存在
        if (ret->second->EnterRoom(player))
        {
            // 进入成功
            player->id_ = playerid;
            player->room = ret->second;
            SampleRespone(player, MsgType::ROOM_ENTER, true, true);
            LOG_INFO("fd:%d from %s EnterRoom fd:%d success", player->fd, player->GetPortAndIpCharArray(), roomid);
        }
        else
        {
            // 进入失败
            SampleRespone(player, MsgType::ROOM_ENTER, false, false);
            LOG_INFO("fd:%d from %s EnterRoom fd:%d enter failed", player->fd, player->GetPortAndIpCharArray(), roomid);
        }
        return;
    }

    // 房间不存在 创建房间并加入
    auto room = new Gameroom(roomid);
    room_map_->insert(std::pair<int, Gameroom*>(roomid, room));
    if (room->EnterRoom(player))
    {
        // 创建后进入成功
        player->id_ = playerid;
        player->room = room;
        SampleRespone(player, MsgType::ROOM_ENTER, true, true);
        LOG_INFO("fd:%d from %s EnterRoom fd:%d create and enter success", player->fd, player->GetPortAndIpCharArray(), roomid);
        return;
    }

    // TODO 进入失败
}

void Game::ExitRoom(Player *player, const uint8_t *data, const int &data_len)
{
    if (!IsPlayerValid(player, "fd:%d from %s ExitRoom invalid", player->fd, player->GetPortAndIpCharArray()))
    {
        return;
    }

    if (player->room->ExitRoom(player))
    {
        // 退出成功
        SampleRespone(player, MsgType::ROOM_EXIT, true, true);
        LOG_INFO("fd:%d from %s ExitRoom fd:%d success", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());

        CheckRoomPlayerNum(player);
    }
    else
    {
        // 退出失败
        SampleRespone(player, MsgType::ROOM_EXIT, false, false);
        LOG_INFO("fd:%d from %s ExitRoom fd:%d failed", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
}

// 有人推出后检查房间人数， 人数为0 则删除房间
void Game::CheckRoomPlayerNum(Player *player)
{
    if (player->room->GetPlayerNum()== 0)
    {
        LOG_INFO("fd:%d from %s Delete room fd:%d", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
        room_map_->erase(player->room->GetRoomid());
        delete player->room;
        player->room = nullptr;
    }
}

void Game::Ready(Player *player, const uint8_t *data, const int &data_len)
{
    if (!IsPlayerValid(player, "fd:%d from %s Ready invalid", player->fd, player->GetPortAndIpCharArray()))
    {
        return;
    }

    if (player->ChangePlayerStatus(PlayerStatus::ROOM_READY))
    {
        // 准备成功
        SampleRespone(player, MsgType::PLAYER_READY, true, true);
        LOG_INFO("fd:%d from %s Ready for fd:%d success", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
    else
    {
        // 准备失败
        SampleRespone(player, MsgType::PLAYER_READY, false, false);
        LOG_INFO("fd:%d from %s Ready for fd:%d failed", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
}

void Game::CancelReady(Player *player, const uint8_t *data, const int &data_len)
{
    if (!IsPlayerValid(player, "fd:%d from %s CancelReady invalid", player->fd, player->GetPortAndIpCharArray()))
    {
        return;
    }

    if (player->ChangePlayerStatus(PlayerStatus::ROOM_NOT_READY))
    {
        // 取消准备成功
        SampleRespone(player, MsgType::PLAYER_CANCEL_READY, true, true);
        LOG_INFO("fd:%d from %s Cancel Ready for roomid:%d success", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
    else
    {
        // 取消准备失败
        SampleRespone(player, MsgType::PLAYER_CANCEL_READY, false, false);
        LOG_INFO("fd:%d from %s Cancel Ready for roomid:%d failed", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
}

void Game::GameStart(Player *player, const uint8_t *data, const int &data_len)
{

    if (!IsPlayerValid(player, "fd:%d from %s GameStart invalid", player->fd, player->GetPortAndIpCharArray()))
    {
        return;
    }

    if (player->room->CanStartGame())
    {
        // 可以开始游戏
        SampleRespone(player, MsgType::GAME_START, true, true);
        LOG_INFO("fd:%d from %s Start for roomid:%d success", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
    else
    {
        // 游戏人数不足
        SampleRespone(player, MsgType::GAME_START, false, false);
        LOG_INFO("fd:%d from %s Start for roomid:%d failed", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }

}

void Game::SampleRespone(Player *player, MsgType type, bool result, bool broadcast)
{
    if (broadcast)
    {
        auto player_vector = player->room->GetPlayerVector();
        const int BUFFER_SIZE = 5;

        auto res_data = new uint8_t[BUFFER_SIZE]{};
        auto res_temp = res_data;
        *res_temp = result ? 1 : 0;
        res_temp++;
        *(uint32_t *) res_temp =player->id_;

        for (auto player_temp : *player_vector)
        {
            if (player_temp == nullptr)
            {
                continue;
            }
            Response(player_temp, (uint8_t)type, VERSION1, res_data, BUFFER_SIZE);
        }
        delete []res_data;
    }
    else
    {
        uint8_t res_data = result ? 1:0;
        Response(player, (uint8_t)type, VERSION1, &res_data, 1);
    }
}

void Game::Response(Player *player, uint8_t msg_type, uint8_t msg_ver, uint8_t *body, uint32_t length)
{
    Net::ProtoMsg msg{};
    msg.header.msg_type = msg_type;
    msg.header.msg_version = msg_ver;
    msg.header.body_len = length;
    msg.body = body;

    uint8_t *data = player->proto_operate.Encode(&msg, &length);

    send(player->fd, (char*)data, length, 0);
}

bool Game::IsPlayerValid(Player *player, const char *format, ...)
{
    if (player->room == nullptr)
    {
        va_list args;
        va_start(args, format);
        LOG_WARN(format, args);
        va_end(args);

        return false;
    }

    return true;
}

void Game::GamePlay(Player *player, const uint8_t *data, const int &data_len)
{
    auto res = new uint8_t[6 + 5 + data_len]{};
    res[0] = (int8_t)MsgType::ROOM_PLAY_DATA;
    res[1] = 1;
    *(uint32_t*)(res+2) = data_len + 5;
    res[6] = 1;
    *(uint32_t*)(res+7) = player->id_;
    memcpy(res + 11, data, data_len);

    LOG_INFO("fd:%d from %s PlayGame for roomid:%d", player->fd, player->GetPortAndIpCharArray(), player->room->GetRoomid());

    for (auto temp : *player->room->GetPlayerVector())
    {
        if (temp == nullptr)
        {
            continue;
        }
        send(temp->fd, res, data_len + 6 + 5, 0);
    }
}
