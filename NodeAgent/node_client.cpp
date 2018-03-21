/* node_client.cpp

 Generated by phxrpc_pb2client from node.proto

*/

#include "node_client.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>

#include "phxrpc_node_stub.h"

#include "phxrpc/rpc.h"


phxrpc::ClientConfig global_nodeclient_config_;
phxrpc::ClientMonitorPtr global_nodeclient_monitor_;


bool NodeClient::Init(const char *config_file) {
    return global_nodeclient_config_.Read(config_file);
}

const char *NodeClient::GetPackageName() {
    const char *ret = global_nodeclient_config_.GetPackageName();
    if (strlen(ret) == 0) {
        ret = "magna";
    }
    return ret;
}

NodeClient::NodeClient() {
    static std::mutex monitor_mutex;
    if (!global_nodeclient_monitor_.get()) {
        monitor_mutex.lock();
        if (!global_nodeclient_monitor_.get()) {
            global_nodeclient_monitor_ = phxrpc::MonitorFactory::GetFactory()
                ->CreateClientMonitor(GetPackageName());
        }
        global_nodeclient_config_.SetClientMonitor(global_nodeclient_monitor_);
        monitor_mutex.unlock();
    }
}

NodeClient::~NodeClient() {
}

int NodeClient::PhxMqttConnect(const phxrpc::MqttConnectPb &req, phxrpc::MqttConnackPb *resp) {
    const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_nodeclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

            NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
            return stub.PhxMqttConnect(req, resp);
        }
    }

    return -1;
}

int NodeClient::PhxMqttPublish(const phxrpc::MqttPublishPb &req, phxrpc::MqttPubackPb *resp) {
    const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_nodeclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

            NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
            return stub.PhxMqttPublish(req, resp);
        }
    }

    return -1;
}

int NodeClient::PhxMqttDisconnect(const phxrpc::MqttDisconnectPb &req) {
    const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_nodeclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

            NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
            return stub.PhxMqttDisconnect(req);
        }
    }

    return -1;
}

int NodeClient::PhxEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp) {
    const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_nodeclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

            NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
            return stub.PhxEcho(req, resp);
        }
    }

    return -1;
}

int NodeClient::PhxBatchEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp) {
    int ret = -1;
    size_t echo_server_count = 2;
    uthread_begin;
    for (size_t i{0}; echo_server_count > i; ++i) {
        uthread_t [=, &uthread_s, &ret](void *) {
            const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetByIndex(i);
            if (ep != nullptr) {
                phxrpc::UThreadTcpStream socket;
                if(phxrpc::PhxrpcTcpUtils::Open(&uthread_s, &socket, ep->ip, ep->port,
                            global_nodeclient_config_.GetConnectTimeoutMS(), *(global_nodeclient_monitor_.get()))) {
                    socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());
                    NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
                    int this_ret = stub.PhxEcho(req, resp);
                    if (this_ret == 0) {
                        ret = this_ret;
                        uthread_s.Close();
                    }
                }
            }
        };
    }
    uthread_end;
    return ret;
}

int NodeClient::StartComponent(const magna::StartComponentRequest &req, magna::StartComponentResponse *resp) {
    const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetRandom();

    if (ep != nullptr) {
		phxrpc::BlockTcpStream socket;
		bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
			global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
			*(global_nodeclient_monitor_.get()));
		if (open_ret) {
			socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

			NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
			return stub.StartComponent(req, resp);
		}
    }

    return -1;
}


int NodeClient::StartComponent(const phxrpc::Endpoint_t & ep, const magna::StartComponentRequest &req, magna::StartComponentResponse *resp)
{
	phxrpc::BlockTcpStream socket;
	bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep.ip, ep.port,
		global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
		*(global_nodeclient_monitor_.get()));
	if (open_ret) {
		socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

		NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
		return stub.StartComponent(req, resp);
	}
}

int NodeClient::StopComponent(const magna::StopComponentRequest &req, magna::StopComponentResponse *resp) {
    const phxrpc::Endpoint_t *ep = global_nodeclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_nodeclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_nodeclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_nodeclient_config_.GetSocketTimeoutMS());

            NodeStub stub(socket, *(global_nodeclient_monitor_.get()));
            return stub.StopComponent(req, resp);
        }
    }

    return -1;
}

