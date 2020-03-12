//
// Created by lsmg on 2/25/20.
//

#include <netinet/in.h>
#include <network/protocol.h>
#include <game/game.h>
#include <network/log.h>
#include "samplegame.h"


void SimpleGame::Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data)
{
    LOG_DEBUG("-------------------------------------------------------");
    LOG_DEBUG("[fd: %d] parse start", fd);
    if (!protos[fd].Decode(data, data_len))
    {
        return;
    }
    LOG_DEBUG("[fd: %d] parse over", fd);

    Net::ProtoMsg *msg = protos[fd].Front();
    int id = *(uint32_t*)(msg->body+1);
    LGame::PlayerInfo *playerinfo = InitPlayerInfo(id, fd, ip, port);

    LOG_DEBUG("[id: %d]recv a new msg from %s:%d", id, ip, port);

    while(!protos[fd].IsEmpty())
    {
        msg = protos[fd].Front();
        protos[fd].Pop();

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

        switch (static_cast<LGame::MsgType>(msg->header.msg_type))
        {
            case LGame::MsgType::ROOM_ENTER:
            {
                GameDebug(playerinfo, "ROOM_ENTER");
                EnterRoom(playerinfo, msg->body, msg->header.body_len);
                break;
            }
            case LGame::MsgType::ROOM_EXIT:
            {
                GameDebug(playerinfo, "ROOM_EXIT");
                ExitRoom(playerinfo, msg->body, msg->header.body_len);
                break;
            }
            case LGame::MsgType::PLAYER_READY:
            {
                GameDebug(playerinfo, "PLAYER_READY");
                Ready(playerinfo, msg->body, msg->header.body_len);
                break;
            }
            case LGame::MsgType::PLAYER_CANCEL_READY:
            {
                GameDebug(playerinfo, "PLAYER_CANCEL_READY");
                CancelReady(playerinfo, msg->body, msg->header.body_len);
                break;
            }
            case LGame::MsgType::GAME_START:
            {
                GameDebug(playerinfo, "GAME_START");
                GameStart(playerinfo, msg->body, msg->header.body_len);
                break;
            }
            case LGame::MsgType::ROOM_PLAY_DATA:
            {
                GameDebug(playerinfo, "ROOM_PLAY_DATA");
                GamePlay(playerinfo, msg->body, msg->header.body_len);
                break;
            }
            case LGame::MsgType::GAME_OVER:
                break;
            default:
                break;
        }
    }
    LOG_DEBUG("-------------------------------------------------------");
}

LGame::PlayerInfo * SimpleGame::InitPlayerInfo(int id, int fd, const char *ip, int port)
{
    auto playerinfo = GetPlayerInfoById(id);
    if (playerinfo == nullptr)
    {
        playerinfo = AddPlayerInfo(id, fd, ip, port);
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

            GameInfo(player_info, "enterroom", true);
            SampleRespone(player_info, LGame::MsgType::ROOM_ENTER, true, true);
        }
        else
        {
            GameInfo(player_info, "enterroom", false);
            SampleRespone(player_info, LGame::MsgType::ROOM_ENTER, false, false);
        }
        return;
    }

    // 房间不存在 创建房间并加入

    room = CreateNewGameroom(roomid);
    if (room->EnterRoom(player_info->player))
    {
        player_info->room = room;
        GameInfo(player_info, "enter and create room", true);
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
        GameInfo(player_info, "exit room", true);
        SampleRespone(player_info, LGame::MsgType::ROOM_EXIT, true, true);
        CheckRoomPlayerNum(player_info);
    }
    else
    {
        GameInfo(player_info, "exit room", false);
        SampleRespone(player_info, LGame::MsgType::ROOM_EXIT, false, false);
    }
}

void SimpleGame::CheckRoomPlayerNum(LGame::PlayerInfo *player_info)
{
    if (player_info->room->GetPlayerNum() == 0)
    {
        GameInfo(player_info, "delete room", true);
        DeleteRoom(player_info);
    }
}

void SimpleGame::Ready(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    if (player_info->player->ChangePlayerStatus(LGame::PlayerStatus::ROOM_READY))
    {
        GameInfo(player_info, "ready", true);
        SampleRespone(player_info, LGame::MsgType::PLAYER_READY, true, true);
    }
    else
    {
        GameInfo(player_info, "ready", false);
        SampleRespone(player_info, LGame::MsgType::PLAYER_READY, false, false);
    }
}

void SimpleGame::CancelReady(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    if (player_info->player->ChangePlayerStatus(LGame::PlayerStatus::ROOM_NOT_READY))
    {
        GameInfo(player_info, "cancel ready", true);
        SampleRespone(player_info, LGame::MsgType::PLAYER_CANCEL_READY, true, true);
    }
    else
    {
        GameInfo(player_info, "cancel ready", false);
        SampleRespone(player_info, LGame::MsgType::PLAYER_CANCEL_READY, false, false);
    }
}

void SimpleGame::GameStart(LGame::PlayerInfo *player_info, const uint8_t *data, const int &data_len)
{
    if (player_info->room->CanStartGame())
    {
        GameInfo(player_info, "game start", true);
        SampleRespone(player_info, LGame::MsgType::GAME_START, true, true);
    }
    else
    {
        GameInfo(player_info, "game start", false);
        SampleRespone(player_info, LGame::MsgType::GAME_START, false, false);
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
        auto playerinfo_temp = GetPlayerInfoById(player->GetId());
        if (playerinfo_temp != nullptr)
        {
            Response(playerinfo_temp, LGame::MsgType::ROOM_PLAY_DATA, LGame::MsgVersion::VERSION1,
                     const_cast<uint8_t *>(data), data_len);
        }
    }
}

void SimpleGame::SampleRespone(LGame::PlayerInfo *playerinfo, LGame::MsgType type, bool result, bool broadcast)
{
    GameDebug(playerinfo, "Start Response");

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
            LGame::PlayerInfo* info = GetPlayerInfoById(player_temp->GetId());
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
    GameDebug(playerinfo, "Response over");
}

void
SimpleGame::Response(LGame::PlayerInfo *playerinfo, LGame::MsgType msg_type, LGame::MsgVersion msg_ver, uint8_t *body, uint32_t length)
{
    Net::ProtoMsg msg{};
    msg.header.msg_type = (uint8_t)msg_type;
    msg.header.msg_version = (uint8_t)msg_ver;
    msg.header.body_len = length;
    msg.body = body;

    uint8_t *data = protos[playerinfo->player->GetFd()].Encode(&msg, &length);

    send(playerinfo->player->GetFd(), (char*)data, length, 0);
}

SimpleGame::SimpleGame(int RoomMaxPlayer) : Game(RoomMaxPlayer)
{
    protos = new Net::Protocol[10000]();
}
