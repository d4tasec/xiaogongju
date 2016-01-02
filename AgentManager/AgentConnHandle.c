/**
* File Name :AgentManager/AgentConnHandle.c
* Author: rootkiter
* E-mail: rootkiter@rootkiter.com
* Created Time : 一 11/16 00:13:14 2015
*/
#include "AgentConnHandle.h"
#include "PCNodeManager.h"
#include "PCNodeInfo.h"
#include "AgentIDCtrl.h"
#include "AgentProNet.h"
#include "AgentCMDParse.h"


//====================================
#define M_INFO_SEND_ERROR     -1
#define M_INFO_SEND_OK         1
int m_Info_send(int sock,pPCNodeInfo info);
#define M_INFO_RECV_ERROR   NULL
//pPCNodeInfo m_agentInfo_Recv(int sock);
#define M_SEND_NOTHING_ERROR  -1
#define M_SEND_NOTHING_OK      1
int m_send_nothing(int sock);
#define M_SENDID_ERROR        -1
#define M_SENDID_OK            1
int m_sendID(int sock,int id);
#define M_RECVID_ERROR        -1
int m_recvID(int sock);
//====================================

int m_AddNeighborProxy(pPCNodeInfo info){
    int res = PCMANAGER_ADDNeighbor(info);
    if(PCMANAGER_ADDNEIGHBOR_ID_CLASH == 
        res ){
        int newid = AGENT_ID_ASK();
        if(PCMANAGER_REPLACEID_ERROR == 
            PCMANAGER_ReplaceID(info->id,newid)){
            return 0;
        }
        if(PCMANAGER_ADDNEIGHBOR_OK ==
              PCMANAGER_ADDNeighbor(info)){
            return 1;
        }
    }
    else if(PCMANAGER_ADDNEIGHBOR_OK == res ){
        return 1;
    }
    Printf_Error("add error ????");
    return 0;
}


//int when_IAM_ADMIN(int sock,char *ip,int port){
//    pPCNodeInfo serverinfo;
//    int hisid = AGENT_ID_ASK();
//    if(M_SENDID_ERROR != m_sendID(sock,hisid)){
//        serverinfo = m_agentInfo_Recv(sock);
//        if(serverinfo != M_INFO_RECV_ERROR){
//            serverinfo->conn.ConnType = CONNTYPE_DIRECT_CONNECT;
//            memcpy(serverinfo->conn.IPaddr,ip,MAX_IP_ADDR_LEN);
//            serverinfo->conn.port = port;
//            if(1 == m_AddNeighborProxy(serverinfo)){
//                return 1;
//            }
//        }
//    }
//    return 0;
//}
//
//int when_IAM_NORMALNODE(int sock,char *ip,int port){
//    int myid = m_recvID(sock);
////    PCMANAGER_ReplaceID(PCMANAGER_Get_RootID(),myid);
//    PCMANAGER_Set_RootID(myid);
//    pPCNodeInfo serverinfo = m_agentInfo_Recv(sock);
//    if(serverinfo->NodeType == MYSELF_NODE){
//        serverinfo->conn.ConnType = CONNTYPE_DIRECT_CONNECT;
//        memcpy(serverinfo->conn.IPaddr,ip,MAX_IP_ADDR_LEN);
//        serverinfo->conn.port = port;
//        if(1 == m_AddNeighborProxy(serverinfo)){
//            return 1;
//        }
//    }
//    return 0;
//}

// be called By client
int AgentInfo_Interactive(int sock,char *ip,int port){
Printf_Error("444444444444444444444444");
    pPCNodeInfo myself = PCMANAGER_Get_RootNode();
    if(myself == NULL){return AGENTCONN_INTERACTIVE_ERROR;}
Printf_Error("555555555555555555555555");
    if(M_INFO_SEND_ERROR == m_Info_send(sock,myself)){
        return AGENTCONN_INTERACTIVE_ERROR;
    }
    /////// Add code From here
//    m_Info_send(sock,myself);
Printf_Error("111111111111111111111111");
    switch(myself->NodeType ){
    case  MYSELF_NODE:
Printf_Error("222222222222222222222222");
//        when_IAM_NORMALNODE(sock,ip,port);
        break;
    case IAM_ADMIN_NODE:
Printf_Error("333333333333333333333333");
//        when_IAM_ADMIN(sock,ip,port);
        break;
    default:
        Printf_Error("NodeType is error");
        break;
    }
    return AGENTCONN_INTERACTIVE_OK;
}

int on_Agent_Connect(int sock){
//    pPCNodeInfo clientinfo = m_agentInfo_Recv(sock);
//Printf_DEBUG("the sock is -----> %d",sock);
//    clientinfo -> conn.ConnType = CONNTYPE_REVERSE_CONNECT;
//    if(clientinfo == NULL){return 0;}
//    // add clientnode 
//    pPCNodeInfo myself = PCMANAGER_Get_RootNode();
//    switch(clientinfo->NodeType){
//    case IAM_ADMIN_NODE:
//        Printf_OK("CLIENT is Admin");
//        if(0 == m_AddNeighborProxy(clientinfo)){
//            Printf_Error("Add NeighborProxy Error");
//            return 0;
//        }
//        int myid = m_recvID(sock);
//        // reset my id
//        PCMANAGER_Set_RootID(myid);
//   //     PCMANAGER_ReplaceID(PCMANAGER_Get_RootID(),myid);
//        // set client is upper
//        PCMANAGER_SETUpperAdmin(clientinfo->id);
//        // send myself
//        m_Info_send(sock,myself);
//        //ChildNodeInfoSyncTrigger();
//        break;
//    case MYSELF_NODE:
//        Printf_OK("Client is Myself_node");
//        int hisid = AGENT_ID_ASK();
//        m_sendID(sock,hisid);
//        Printf_OK("Client id is %d",hisid);
//        clientinfo->id = hisid;
//        if(0 == m_AddNeighborProxy(clientinfo)){
//            Printf_Error("Add NeighborProxy Error");
//            return 0;
//        }
//        Printf_DEBUG("Send Agent Info Here11111111111");
////        SendAgentInfo(PCMANAGER_Get_RootID(),clientinfo->id,
////            clientinfo->OSType,clientinfo->PCName);
////        m_Info_send(sock,myself);
//        break;
//    default:
//        return 0;
//    }
Printf_DEBUG("myid now is %d",PCMANAGER_Get_RootID());
    return 1;
}


//******************************************************************
#define SEND_INFO_MSG         1
#define SEND_NOTHING_MSG      2
#define RECVOK    1
#define RECVERROR 2

#define SEND_INFO_ERROR  -1
#define SEND_INFO_OK      1
int send_info(int sock,char *buffer,int len){
    int result,nrecv;
    char msg  = SEND_INFO_MSG;
    int nsend = API_socket_send(sock,&msg,1);
    if(nsend == SOCKET_SEND_ERROR){
        result =  SEND_INFO_ERROR;
    }
    else{
        nsend = API_socket_send(sock,buffer,len);
        if(nsend == SOCKET_SEND_ERROR){
            result =  SEND_INFO_ERROR;
        }
        else{
            nrecv = API_socket_recv(sock,&msg,1);
            if(nrecv == SOCKET_RECV_ERROR 
                || msg != RECVOK){
                result =  SEND_INFO_ERROR;
            }
            else{
                result = SEND_INFO_OK;
            }
        }
    }
    return result;
}

int m_send_nothing(int sock){
    int result,nsend,nrecv;
    char msg = SEND_NOTHING_MSG;
    nsend= API_socket_send(sock,&msg,1);
    if(nsend == SOCKET_SEND_ERROR){
        result = SEND_INFO_ERROR;
    }
    else{
        nrecv = API_socket_recv(sock,&msg,1);
        if(nrecv == SOCKET_RECV_ERROR
            || msg != RECVOK){
            result = SEND_INFO_ERROR;
        }
        else{
            result = SEND_INFO_OK;
        }
    }
    return result;
}

#define RECV_INFO_ERROR    -1
#define RECV_INFO_OK        1
int recv_info(int sock,char *buffer,int len){
    int result , nsend,nrecv;
    char msg ,*buf;
    nrecv  =API_socket_recv(sock,&msg,1);
    if(nrecv == SOCKET_RECV_ERROR
        || msg == SEND_NOTHING_MSG){
        result = RECV_INFO_ERROR;
    }
    else{
        buf = (char *)malloc(sizeof(char)*len+10);
        nrecv = API_socket_recv(sock,buf,len);
        if(nrecv == SOCKET_RECV_ERROR){
            result = RECV_INFO_ERROR;
        }
        else{
            msg = RECVOK;
            API_socket_send(sock,&msg,1);
            result = RECV_INFO_OK;
        }
        memcpy(buffer,buf,len);
        free(buf);
    }
    buf = NULL;
    return result;
}

int m_sendID(int sock,int id){
    char buffer[20];
    API_m_itochar(id,buffer,4);
    if(send_info(sock,buffer,4)
        ==SEND_INFO_ERROR){
        return M_SENDID_ERROR;
    }
    return M_SENDID_OK;
}

int m_recvID(int sock){
    char buffer[20];
    if(recv_info(sock,buffer,4) 
        == RECV_INFO_ERROR){
        return M_RECVID_ERROR;
    }
    return API_m_chartoi(buffer,4);
}

int m_Info_send(int sock,pPCNodeInfo info){
    char buffer[1000];
    char idbuf[4],ostype[4],nodetype[4];
    int  namelen;
    int result = M_INFO_SEND_OK;
    pPCConn conn = NULL;
    pAgent_proto proto = NULL;

    if(info == NULL){
        result = M_INFO_SEND_ERROR;
        goto exit;
    }
    else{
        conn = PCCONN_CreatePCConnFromSocket(sock);    
        namelen = strlen(info->PCName);
        if(PCCONN_CREATEPCCONNFROMSOCKET_ERROR == conn
           || namelen > MAX_PCNAME_LEN ){
            result = M_INFO_SEND_ERROR;
            goto exit;
        }
        
        API_m_itochar (info->id       ,&(buffer[ 0]) ,4);
        API_m_itochar (info->OSType   ,&(buffer[ 4]) ,4);
        API_m_itochar (info->NodeType ,&(buffer[ 8]) ,4);
        API_m_itochar (namelen        ,&(buffer[12]) ,4);
        strncpy(&(buffer[16]), info->PCName,namelen+1);
       // send it  
        proto = PROTO_CreateProto();
        if(proto == PROTO_CREATEPROTO_ERROR){
            result =  M_INFO_SEND_ERROR;
            goto exit;
        }
        PROTO_SetCMD(proto,CMDTYPE_NEWCONN,
            CMDID_NEWNODE_HERE,0);
        PROTO_SetAddress(proto,-1,-1);
        PROTO_SetArgs(proto,16+namelen+1,buffer);

        if(PROTO_SENDPROTO_ERROR ==
            PROTO_SendProto(conn,proto) ){
            // send error
            result =  M_INFO_SEND_ERROR;
            goto exit;
        }
    }
exit:
    if(result == M_INFO_SEND_ERROR){
        Printf_Error("Send info Error");
    }
    return  result;
}

#define BUF_ERROR   -1
#define BUF_OK       1
pPCNodeInfo m_agentInfo_Recv(int sock){
//    pPCNodeInfo info = PCNODE_Create();
//    pPCConn conn = PCCONN_CreatePCConnFromSocket(sock);
//    pAgent_proto proto = NULL;
//    char pcname[MAX_PCNAME_LEN+1];
//    char id[4],ostype[4],linktype[4];
//    int  mid,mostype,mlinktype;
//    int result = 1;
//    if(info == NULL || conn == NULL){
//        result = BUF_ERROR;
//        goto exit;
//    }
//    else{
//        // recv id
//        
//        if( PROTO_RECVSTATE_CANRECV 
//            != PROTO_RecvState(conn)){
//            result = BUF_ERROR;
//            goto exit;
//        }
//        else{
//            // recv agent info         
//            proto = PROTO_RecvProto(conn);
//            if(PROTO_RECVPROTO_ERROR 
//                == proto){
//                result = BUF_ERROR;
//                goto exit;
//            }
//            if(CMDPARSE_AND_DO_ERROR == 
//                CMDParse_And_Do(proto,conn)){
//                result = BUF_ERROR;
//                goto exit;
//            }
//        }
//    }
//exit:
//    if(result == BUF_ERROR){
//        return M_INFO_RECV_ERROR;
//    }
//Printf_OK("[recv ]id = %d,ostype = %d , nodetype = %d, pcname = %s",
//        info-> id,info->OSType,info->NodeType,info->PCName);
//    return info;
      return M_INFO_RECV_ERROR;
}

//******************************************************************
