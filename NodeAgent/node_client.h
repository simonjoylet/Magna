/* node_client.h

 Generated by phxrpc_pb2client from node.proto

*/

#pragma once

#include "node.pb.h"
#include "phxrpc/rpc.h"


class NodeClient {
  public:
    static bool Init(const char *config_file);

    static const char *GetPackageName();

    NodeClient();
    virtual ~NodeClient();

    int PhxMqttConnect(const phxrpc::MqttConnectPb &req, phxrpc::MqttConnackPb *resp);
    int PhxMqttPublish(const phxrpc::MqttPublishPb &req, phxrpc::MqttPubackPb *resp);
    int PhxMqttDisconnect(const phxrpc::MqttDisconnectPb &req);
    int PhxEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp);
    int PhxBatchEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp);

	int StartComponent(const magna::StartComponentRequest &req, magna::StartComponentResponse *resp);
	int StartComponent(const phxrpc::Endpoint_t & ep, const magna::StartComponentRequest &req, magna::StartComponentResponse *resp);
    int StopComponent(const magna::StopComponentRequest &req, magna::StopComponentResponse *resp);

};

