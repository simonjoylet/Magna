/* simu_client.h

 Generated by phxrpc_pb2client from simu.proto

*/

#pragma once

#include "simu.pb.h"
#include "phxrpc/rpc.h"


class SimuClient {
  public:
    static bool Init(const char *config_file);

    static const char *GetPackageName();

    SimuClient();
    virtual ~SimuClient();

    int PhxMqttConnect(const phxrpc::MqttConnectPb &req, phxrpc::MqttConnackPb *resp);
    int PhxMqttPublish(const phxrpc::MqttPublishPb &req, phxrpc::MqttPubackPb *resp);
    int PhxMqttDisconnect(const phxrpc::MqttDisconnectPb &req);
    int PhxEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp);
    int PhxBatchEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp);
    int GetRet(const magna::RetRequest &req, magna::RetResponse *resp);
    int ReportLoad(const magna::ReportLoadRequest &req, magna::ReportLoadResponse *resp);

};

