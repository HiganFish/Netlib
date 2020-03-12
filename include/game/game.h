//
// Created by lsmg on 3/5/20.
//

#ifndef NETLIB_GAME_H
#define NETLIB_GAME_H

#include <map>

#include "player.h"
#include "gameroom.h"

namespace LGame
{

enum class MsgType : uint8_t
{
    ROOM_ENTER,
    ROOM_EXIT,
    PLAYER_READY,
    PLAYER_CANCEL_READY,
    GAME_START,
    ROOM_PLAY_DATA,
    GAME_OVER
};

enum class MsgVersion : uint8_t
{
    VERSION0,
    VERSION1
};

struct PlayerInfo
{
    Player *player;
    Gameroom* room;
};

class Game
{
public:
    Game(int GameroomMaxPlayer);
    virtual ~Game();

    /**
     * 处理并分发到来的原始数据
     * @param fd 连接的文件描述符
     * @param ip
     * @param port
     * @param data_len
     * @param data
     */
    virtual void Distribute(const int &fd, const char *ip, const int &port, const int &data_len, uint8_t *data);

    /**
     * @param id 玩家个人id
     * @return id对应的playerinfo结构体
     */
    PlayerInfo* GetPlayerInfoById(int id);

    /**
     * 从room_map_中删除 对应的房间
     * @param player_info 从中获取对应的房间
     */
    void DeleteRoom(PlayerInfo *player_info);

    /**
     * 获取id对应的playerinfo 如果无则创建
     * @param id 玩家个人id
     * @param fd 玩家连接的文件描述符
     * @param ip
     * @param port
     * @return id对应的结构体
     */
    LGame::PlayerInfo *AddPlayerInfo(int id, int fd, const char *ip, int port);


    Gameroom* GetGameroom(int roomid);

    /**
     * 创建新的gameroom 并添加到room_map_中
     * @param roomid
     */
    Gameroom * CreateNewGameroom(int roomid);
private:
    // roomid to gameroom
    std::map<int, Gameroom*> *room_map_;

    // player id to player info
    std::map<int, PlayerInfo*> *player_info_map_;

    // 如果存在PlayerInfo则返回， 如果不存在则创建并返回
    virtual LGame::PlayerInfo *InitPlayerInfo(int id, int fd, const char *ip, int port);

    // 进入房间 并返回数据
    virtual void EnterRoom(PlayerInfo *player_info, const uint8_t *data, const int &data_len);

    // 检查playerinfo合法性
    virtual bool CheckPlayerInfo(PlayerInfo *player_info);

    // 退出房间 并返回数据
    virtual void ExitRoom(PlayerInfo *player_info, const uint8_t *data, const int &data_len);
    // 检查房间人数 如果人数为0则自动解散房间
    virtual void CheckRoomPlayerNum(PlayerInfo *player_info);

    // 准备游戏 并返回数据
    virtual void Ready(PlayerInfo *player_info, const uint8_t *data, const int &data_len);
    // 取消准备状态 并返回数据
    virtual void CancelReady(PlayerInfo *player_info, const uint8_t *data, const int &data_len);

    // 开始游戏 并返回数据
    virtual void GameStart(PlayerInfo *player_info, const uint8_t *data, const int &data_len);

    // 游戏中状态数据分发
    virtual void GamePlay(PlayerInfo *player_info, const uint8_t *data, const int &data_len);

    /**
     * 发送数据包 body中仅有 result 和 id
     * @param player_info
     * @param type 数据包种类
     * @param result 当前操作结果
     * @param broadcast 是否发送到当前房间所有人
     */
    virtual void SampleRespone(PlayerInfo *player_info, MsgType type, bool result, bool broadcast);

    /**
     * 自定义body的发送
     * @param player_info
     * @param msg_type 数据包种类
     * @param msg_ver 协议等级
     * @param body 完成发送数据包的body
     * @param length body长度
     */
    virtual void Response(PlayerInfo *player_info, MsgType msg_type, MsgVersion msg_ver, uint8_t *body, uint32_t length);


    Game(const Game&);
    Game& operator=(const Game&);

    const static int VERSION1 = 1;
};
}
#endif //NETLIB_GAME_H
