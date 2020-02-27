//
// Created by lsmg on 2/25/20.
//

#include <log.h>
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

void Game::EnterRoom(Player *player, const uint8_t *data, const int &data_len)
{
    uint32_t roomid = *(uint32_t*)data;

    auto ret = room_map_->find(roomid);
    if (ret != room_map_->end())
    {
        // 房间已经存在
        if (ret->second->EnterRoom(player))
        {
            // 进入成功

            uint8_t res_data = 1;
            player->proto_operate.EncodeAndSendBack(player->id, (uint8_t)MsgType::ROOM_ENTER, VERSION1, &res_data, 1);
            LOG_INFO("fd:%d from %s EnterRoom id:%d success", player->id, player->GetPortAndIpCharArray(), roomid);
        }
        else
        {
            // 进入失败
            uint8_t res_data = 0;
            player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::ROOM_ENTER, VERSION1, &res_data, 1);
            LOG_INFO("fd:%d from %s EnterRoom id:%d enter failed", player->id, player->GetPortAndIpCharArray(), roomid);
        }
        return;
    }

    // 房间不存在 创建房间并加入
    auto room = new Gameroom(roomid);
    room_map_->insert(std::pair<int, Gameroom*>(roomid, room));
    if (room->EnterRoom(player))
    {
        // 创建后进入成功
        uint8_t res_data = 1;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t)MsgType::ROOM_ENTER, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s EnterRoom id:%d create and enter success", player->id, player->GetPortAndIpCharArray(), roomid);
        return;
    }

    // TODO 进入失败
}

void Game::ExitRoom(Player *player, const uint8_t *data, const int &data_len)
{
    if (player->room == nullptr)
    {
        LOG_WARN("fd:%d from %s ExitRoom invalid", player->id, player->GetPortAndIpCharArray());
        return;
    }

    if (player->room->ExitRoom(player))
    {
        // 退出成功
        uint8_t res_data = 1;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::ROOM_EXIT, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s ExitRoom id:%d success", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());

        CheckRoomNum(player);
    }
    else
    {
        // 退出失败
        uint8_t res_data = 0;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::ROOM_EXIT, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s ExitRoom id:%d failed", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
}

// 有人推出后检查房间人数， 人数为0 则删除房间
void Game::CheckRoomNum(Player *player)
{
    if (player->room->GetPlayerNum()== 0)
    {
        LOG_INFO("fd:%d from %s Delete room id:%d", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());
        room_map_->erase(player->room->GetRoomid());
        delete player->room;
        player->room = nullptr;
    }
}

void Game::Ready(Player *player, const uint8_t *data, const int &data_len)
{
    if (player->room == nullptr)
    {
        LOG_WARN("fd:%d from %s Ready invalid", player->id, player->GetPortAndIpCharArray());
        return;
    }

    if (player->ChangePlayerStatus(PlayerStatus::ROOM_READY))
    {
        // 准备成功
        uint8_t res_data = 1;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::PLAYER_READY, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s Ready for id:%d success", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
    else
    {
        // 准备失败
        uint8_t res_data = 0;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::PLAYER_READY, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s Ready for id:%d failed", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
}

void Game::CancelReady(Player *player, const uint8_t *data, const int &data_len)
{
    if (player->room == nullptr)
    {
        LOG_WARN("fd:%d from %s CancelReady invalid", player->id, player->GetPortAndIpCharArray());
        return;
    }

    if (player->ChangePlayerStatus(PlayerStatus::ROOM_NOT_READY))
    {
        // 取消准备成功
        uint8_t res_data = 1;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::PLAYER_CANCEL_READY, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s Cancel Ready for id:%d success", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
    else
    {
        // 取消准备失败
        uint8_t res_data = 0;
        player->proto_operate.EncodeAndSendBack(player->id, (uint8_t) MsgType::PLAYER_CANCEL_READY, VERSION1, &res_data, 1);
        LOG_INFO("fd:%d from %s Cancel Ready for id:%d failed", player->id, player->GetPortAndIpCharArray(), player->room->GetRoomid());
    }
}
