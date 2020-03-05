//
// Created by lsmg on 2/25/20.
//

#include <network/log.h>
#include <netinet/in.h>
#include <network/protocol.h>
#include <cstdarg>
#include <game/game.h>

#include "samplegame.h"


void SimpleGame::Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data)
{
    LGame::PlayerInfo *playerinfo = InitPlayerInfo(fd, ip, port);

    if (!playerinfo->proto.Decode(data, data_len))
    {
        return;
    }

    while(!playerinfo->proto.IsEmpty())
    {
        Net::ProtoMsg *msg = playerinfo->proto.Front();
        playerinfo->proto.Pop();

        if (msg->header.msg_version != (uint8_t)LGame::MsgVersion::VERSION1)
        {
            continue;
        }
        if (msg->header.msg_type != (uint8_t)LGame::MsgType::ROOM_ENTER)
        {
            if (playerinfo->room == nullptr)
            {

            }
        }

        playerinfo->player->SetId(*(uint32_t*)(msg->body + 1));

        switch (static_cast<LGame::MsgType>(msg->header.msg_type))
        {
            case LGame::MsgType::ROOM_ENTER:
                EnterRoom(playerinfo, msg->body, msg->header.body_len);
                break;
            case LGame::MsgType::ROOM_EXIT:
                ExitRoom(playerinfo, msg->body, msg->header.body_len);
                break;
            case LGame::MsgType::PLAYER_READY:
                Ready(playerinfo, msg->body, msg->header.body_len);
                break;
            case LGame::MsgType::PLAYER_CANCEL_READY:
                CancelReady(playerinfo, msg->body, msg->header.body_len);
                break;
            case LGame::MsgType::GAME_START:
                GameStart(playerinfo, msg->body, msg->header.body_len);
                break;
            case LGame::MsgType::ROOM_PLAY_DATA:
                GamePlay(playerinfo, msg->body, msg->header.body_len);
                break;
            case LGame::MsgType::GAME_OVER:
                break;
            default:
                break;
        }
    }
}

LGame::PlayerInfo * SimpleGame::InitPlayerInfo(int fd, const char *ip, int port)
{
    auto playerinfo = GetPlayerInfoByFd(fd);
    if (playerinfo == nullptr)
    {
        playerinfo = AddPlayerInfo(fd, ip, port);
    }
    return playerinfo;
}

void SimpleGame::EnterRoom(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    data++;
    uint32_t playerid = *(uint32_t*)data;
    data+=4;
    uint32_t roomid = *(uint32_t*)data;

    LGame::Gameroom *room = GetGameroom(roomid);
    if (room != nullptr)
    {
        // 房间已经存在
        if (room->EnterRoom(player_info->player))
        {
            player_info->room = room;
            SampleRespone(player_info, LGame::MsgType::ROOM_ENTER, true, true);
        }
        else
        {
            // 进入失败
            SampleRespone(player_info, LGame::MsgType::ROOM_ENTER, false, false);
        }
        return;
    }

    // 房间不存在 创建房间并加入

    room = CreateNewGameroom(roomid);
    if (room->EnterRoom(player_info->player))
    {
        player_info->room = room;
        SampleRespone(player_info, LGame::MsgType::ROOM_ENTER, true, true);
        return;
    }
}

bool SimpleGame::CheckPlayerInfo(LGame::PlayerInfo *player_info)
{

}

void SimpleGame::ExitRoom(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    if (player_info->room->ExitRoom(player_info->player))
    {
        // 退出成功
        SampleRespone(player_info, LGame::MsgType::ROOM_EXIT, true, true);
        CheckRoomPlayerNum(player_info);
    }
    else
    {
        // 退出失败
        SampleRespone(player_info, LGame::MsgType::ROOM_EXIT, false, false);
    }
}

void SimpleGame::CheckRoomPlayerNum(LGame::PlayerInfo *player_info)
{
    if (player_info->room->GetPlayerNum() == 0)
    {
        DeleteRoom(player_info);
    }
}

void SimpleGame::Ready(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    if (player_info->player->ChangePlayerStatus(LGame::PlayerStatus::ROOM_READY))
    {
        // 准备成功
        SampleRespone(player_info, LGame::MsgType::PLAYER_READY, true, true);
    }
    else
    {
        // 准备失败
        SampleRespone(player_info, LGame::MsgType::PLAYER_READY, false, false);
    }
}

void SimpleGame::CancelReady(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    if (player_info->player->ChangePlayerStatus(LGame::PlayerStatus::ROOM_NOT_READY))
    {
        // 取消准备成功
        SampleRespone(player_info, LGame::MsgType::PLAYER_CANCEL_READY, true, true);
    }
    else
    {
        // 取消准备失败
        SampleRespone(player_info, LGame::MsgType::PLAYER_CANCEL_READY, false, false);
    }
}

void SimpleGame::GameStart(LGame::PlayerInfo *playerInfo, const uint8_t *data, const int &data_len)
{
    if (playerInfo->room->CanStartGame())
    {
        // 可以开始游戏
        SampleRespone(playerInfo, LGame::MsgType::GAME_START, true, true);
    }
    else
    {
        // 游戏人数不足
        SampleRespone(playerInfo, LGame::MsgType::GAME_START, false, false);
    }
}

void SimpleGame::GamePlay(LGame::PlayerInfo *playerInfo, const uint8_t *data, const int &data_len)
{
    for (auto player : *playerInfo->room->GetPlayerVector())
    {
        if (player == nullptr)
        {
            continue;
        }
        auto playerinfo_temp = GetPlayerInfoByFd(player->GetId());
        if (playerinfo_temp != nullptr)
        {
            Response(playerinfo_temp, LGame::MsgType::ROOM_PLAY_DATA, LGame::MsgVersion::VERSION1,
                     const_cast<uint8_t *>(data), data_len);
        }
    }
}

void SimpleGame::SampleRespone(LGame::PlayerInfo *playerinfo, LGame::MsgType type, bool result, bool broadcast)
{
    if (broadcast)
    {
        const int BUFFER_SIZE = 5;
        // [00 00 00 00 00] uint8 result uint32 id
        auto res_data = new uint8_t[BUFFER_SIZE]{};
        auto res_temp = res_data;
        *res_temp = result ? 1 : 0;
        res_temp++;
        *(uint32_t *)res_temp =playerinfo->player->GetId();

        // 获取所有玩家 并广播
        auto player_vector = playerinfo->room->GetPlayerVector();
        for (auto player_temp : *player_vector)
        {
            if (player_temp == nullptr)
            {
                continue;
            }
            LGame::PlayerInfo* info = GetPlayerInfoByFd(player_temp->GetFd());
            if (info != nullptr)
            {
                Response(info, type, LGame::MsgVersion::VERSION1, res_data, BUFFER_SIZE);
            }
        }
        delete []res_data;
    }
    else
    {
        uint8_t res_data = result ? 1:0;
        Response(playerinfo, type, LGame::MsgVersion::VERSION1, &res_data, 1);
    }
}

void
SimpleGame::Response(LGame::PlayerInfo *playerinfo, LGame::MsgType msg_type, LGame::MsgVersion msg_ver, uint8_t *body, uint32_t length)
{
    Net::ProtoMsg msg{};
    msg.header.msg_type = (uint8_t)msg_type;
    msg.header.msg_version = (uint8_t)msg_ver;
    msg.header.body_len = length;
    msg.body = body;

    uint8_t *data = playerinfo->proto.Encode(&msg, &length);

    send(playerinfo->player->GetFd(), (char*)data, length, 0);
}

SimpleGame::SimpleGame(int RoomMaxPlayer) : Game(RoomMaxPlayer)
{

}
