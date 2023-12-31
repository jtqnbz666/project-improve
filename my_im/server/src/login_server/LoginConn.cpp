#include "LoginConn.h"
#include "IM.Server.pb.h"
#include "IM.Other.pb.h"
#include "IM.Login.pb.h"
#include "public_define.h"

using namespace IM::BaseDefine;

//不管是client 还是 msg 的连接， 都是用的LoginConn对象， 没有单独划分为MsgConn, 和 ClientConn
static ConnMap_t g_client_conn_map;     //handle 与 conn 映射
static ConnMap_t g_msg_serv_conn_map;
static uint32_t g_total_online_user_cnt = 0;
map<uint32_t, msg_serv_info_t*> g_msg_serv_info;//保存msgserver信息, fd 与 info 映射

//这个函数主要就是维护心跳连接
void login_conn_timer_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    uint64_t cur_time = get_tick_count();
    for(ConnMap_t::iterator it = g_client_conn_map.begin(); it != g_client_conn_map.end(); ) {
        ConnMap_t::iterator it_old = it;
        it ++;  //因为可能删除it_old,以防迭代器失效
        CLoginConn*pConn = (CLoginConn*)it_old->second;
        pConn->OnTimer(cur_time);   //心跳包维护
    }
    for(ConnMap_t::iterator it = g_msg_serv_conn_map.begin(); it != g_msg_serv_conn_map.end();) {
        ConnMap_t::iterator it_old = it;
        it ++;
        CLoginConn* pConn = (CLoginConn*)it_old->second;
        pConn->OnTimer(cur_time);   //心跳包。
    }
}

void init_login_conn() {
    netlib_register_timer(login_conn_timer_callback, NULL, 1000);
    //每秒触发一次
}

CLoginConn::CLoginConn() {

}

CLoginConn::~CLoginConn() {

}

void CLoginConn::Close() {
    if(m_handle != NETLIB_INVALID_HANDLE) {
        netlib_close(m_handle);
        if(m_conn_type == LOGIN_CONN_TYPE_CLIENT) {
            g_client_conn_map.erase(m_handle); 
        } else {
            g_msg_serv_conn_map.erase(m_handle);

            //remove all user count from this mssage server
            map<uint32_t, msg_serv_info_t*>::iterator it = g_msg_serv_info.find(m_handle);
            if(it != g_msg_serv_info.end()) {
                msg_serv_info_t* pMsgServInfo = it->second;
                //更新在线人数
                g_total_online_user_cnt -= pMsgServInfo->cur_conn_cnt;
                log("onclose from MsgServer: %s:%u", pMsgServInfo->hostname.c_str(), pMsgServInfo->port);
                delete pMsgServInfo;
                g_msg_serv_info.erase(it);
            }
        }
    }
    ReleaseRef(); //每次用了之后都会减一， 在findbasesocket的时候加一
}

void CLoginConn::OnConnect2(net_handle_t handle, int conn_type) {
    m_handle = handle;
    m_conn_type = conn_type;    //看看是client 还是 msgserver
    ConnMap_t* conn_map = &g_msg_serv_conn_map;
    if(conn_type == LOGIN_CONN_TYPE_CLIENT) {
        conn_map = &g_client_conn_map;
    } else {
        conn_map->insert(make_pair(handle, this));
    }
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)imconn_callback);	//刚开始分配了BaseSocket后，设置的回调函数是listenfd自己的，这里设置的回调函数才是真正需要的
    //imconn 的回调函数就是需要判断一个hash表， callback_data 都是传递的哈希表
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, (void*)conn_map);
}

void CLoginConn::OnClose() {
    Close();
}

void CLoginConn::OnTimer(uint64_t curr_tick) {
    if(m_conn_type == LOGIN_CONN_TYPE_CLIENT) {
        if(curr_tick > m_last_recv_tick + CLIENT_TIMEOUT) {
            Close(); //如果超过2分钟没有收到来自客户端的数据，则关闭连接。
        }
    } else {
        //这里是对于msgserver的心跳处理， 每5秒发送一次心跳包。
        if(curr_tick > m_last_send_tick + SERVER_HEARTBEAT_INTERVAL) {
            IM::Other::IMHeartBeat msg;
            CImPdu pdu;
            pdu.SetPBMsg(&msg);
            pdu.SetServiceId(SID_OTHER);
            pdu.SetCommandId(CID_OTHER_HEARTBEAT);
            SendPdu(&pdu);
        }
        if(curr_tick > m_last_recv_tick + SERVER_TIMEOUT) { //30秒
            log("connection to MsgServer timeout");
            Close(); 
        }
    }
}

//HandlePdu在imconn.cpp中的OnRead函数第哦啊用
void CLoginConn::HandlePdu(CImPdu* pPdu) {
    log("HandlerPdu = %u", pPdu->GetCommandId());
    switch(pPdu->GetCommandId()) {
        case CID_OTHER_HEARTBEAT:
            break;
        case CID_OTHER_MSG_SERV_INFO:
            _HandleMsgServInfo(pPdu);
            break;
        case CID_OTHER_USER_CNT_UPDATE:
            _HandleUserCntUpdate(pPdu);//msg_server收到client登录后通知loginserver登录加1，或者退出就减一

            break;
        case CID_LOGIN_REQ_MSGSERVER:
            _HandleMsgServRequest(pPdu); //找一个负载最小的msgserver返回给客户端
            break;
        default:
            log("wrong msg, cmd id = %d", pPdu->GetCommandId());
            break;
    }
}

void CLoginConn::_HandleMsgServInfo(CImPdu* pPdu) {
    msg_serv_info_t* pMsgServInfo = new msg_serv_info_t;//保存msgserv信息
    IM::Server::IMMsgServInfo msg;
    msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength());
    pMsgServInfo->ip_addr1 = msg.ip1();
    pMsgServInfo->ip_addr2 = msg.ip2();
    pMsgServInfo->port = msg.port();
    pMsgServInfo->max_conn_cnt = msg.max_conn_cnt();
    pMsgServInfo->cur_conn_cnt = msg.cur_conn_cnt();
    pMsgServInfo->hostname = msg.host_name();
    g_msg_serv_info.insert(make_pair(m_handle, pMsgServInfo));

    g_total_online_user_cnt += pMsgServInfo->cur_conn_cnt;
    //这里有一个使用 \ 的细节
    log("MsgServInfo, ip_addr1=%s, ip_addr2 = %s, port = %d, max_conn_cnt = %d, cur_conn_cnt = %d, \
    hostname:%s.",pMsgServInfo->ip_addr1.c_str(), pMsgServInfo->ip_addr2.c_str(), pMsgServInfo->port,pMsgServInfo->max_conn_cnt,
	pMsgServInfo->cur_conn_cnt, pMsgServInfo->hostname.c_str());
}

void CLoginConn::_HandleUserCntUpdate(CImPdu* pPdu) {
    map<uint32_t, msg_serv_info_t*>::iterator it = g_msg_serv_info.find(m_handle);
    if(it != g_msg_serv_info.end()) {
        msg_serv_info_t* pMsgServInfo = it->second;
        IM::Server::IMUserCntUpdate msg;
        msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength());
        uint32_t action = msg.user_action();
        if(action == USER_CNT_INC) { //上线加一
            pMsgServInfo->cur_conn_cnt++;
            g_total_online_user_cnt++;
        } else {        //下线减一
            pMsgServInfo->cur_conn_cnt --;
            g_total_online_user_cnt --;
        }
        log("%s:%d, cur_cnt=%u, total_cnt = %u", pMsgServInfo->hostname.c_str(), 
        pMsgServInfo->port, pMsgServInfo->cur_conn_cnt, g_total_online_user_cnt);
    }
}

void CLoginConn::_HandleMsgServRequest(CImPdu* pPdu) {
    IM::Login::IMMsgServReq msg;
    msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength());
    log("HandleMsgServReq.");

    //如果没有可用的msgserv
    if(g_msg_serv_info.size() == 0) {
        IM::Login::IMMsgServRsp msg;
        msg.set_result_code(::IM::BaseDefine::REFUSE_REASON_NO_MSG_SERVER);
        CImPdu pdu;
        pdu.SetPBMsg(&msg);
        pdu.SetServiceId(SID_LOGIN);
        pdu.SetCommandId(CID_LOGIN_RES_MSGSERVER);
        pdu.SetSeqNum(pPdu->GetSeqNum());
        SendPdu(&pdu);
        Close();
        return ;
    }
    //有用的消息
    msg_serv_info_t* pMsgServInfo;
    uint32_t min_user_cnt = (uint32_t)-1;   //无符号就对应全1
    map<uint32_t, msg_serv_info_t*>::iterator it_min_conn = g_msg_serv_info.end(), it;

    for(it = g_msg_serv_info.begin(); it != g_msg_serv_info.end(); it ++) {
        pMsgServInfo = it->second;
        if( (pMsgServInfo->cur_conn_cnt < pMsgServInfo->max_conn_cnt) && 
            (pMsgServInfo->cur_conn_cnt < min_user_cnt)) {
                it_min_conn = it;
                min_user_cnt = pMsgServInfo->cur_conn_cnt;
            }
    }
    if(it_min_conn == g_msg_serv_info.end()) {
        log("All TCP MsgServer are full");
        IM::Login::IMMsgServRsp msg;
        msg.set_result_code(::IM::BaseDefine::REFUSE_REASON_MSG_SERVER_FULL);
        CImPdu pdu;
        pdu.SetPBMsg(&msg);
        pdu.SetServiceId(SID_LOGIN);
        pdu.SetCommandId(CID_LOGIN_RES_MSGSERVER);
        pdu.SetSeqNum(pPdu->GetSeqNum());
        SendPdu(&pdu);
    } else {
        IM::Login::IMMsgServRsp msg;
        msg.set_result_code(::IM::BaseDefine::REFUSE_REASON_NONE);
        msg.set_prior_ip(it_min_conn->second->ip_addr1);
        msg.set_port(it_min_conn->second->port);
        CImPdu pdu;
        pdu.SetPBMsg(&msg);     
        pdu.SetServiceId(SID_LOGIN);
        pdu.SetCommandId(CID_LOGIN_RES_MSGSERVER);
        pdu.SetSeqNum(pPdu->GetSeqNum());
        SendPdu(&pdu);
    }
    Close(); // 把msgserv的信息发过去，这条连接就可以关闭了

}