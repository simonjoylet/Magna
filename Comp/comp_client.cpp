/* comp_client.cpp

 Generated by phxrpc_pb2client from comp.proto

*/

#include "comp_client.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>

#include "phxrpc_comp_stub.h"

#include "phxrpc/rpc.h"


static phxrpc::ClientConfig global_compclient_config_;
static phxrpc::ClientMonitorPtr global_compclient_monitor_;


bool CompClient::Init(const char *config_file) {
    return global_compclient_config_.Read(config_file);
}

const char *CompClient::GetPackageName() {
    const char *ret = global_compclient_config_.GetPackageName();
    if (strlen(ret) == 0) {
        ret = "magna";
    }
    return ret;
}

CompClient::CompClient() {
    static std::mutex monitor_mutex;
    if (!global_compclient_monitor_.get()) {
        monitor_mutex.lock();
        if (!global_compclient_monitor_.get()) {
            global_compclient_monitor_ = phxrpc::MonitorFactory::GetFactory()
                ->CreateClientMonitor(GetPackageName());
        }
        global_compclient_config_.SetClientMonitor(global_compclient_monitor_);
        monitor_mutex.unlock();
    }
}

CompClient::~CompClient() {
}

int CompClient::PhxMqttConnect(const phxrpc::MqttConnectPb &req, phxrpc::MqttConnackPb *resp) {
    const phxrpc::Endpoint_t *ep = global_compclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_compclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_compclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());

            CompStub stub(socket, *(global_compclient_monitor_.get()));
            return stub.PhxMqttConnect(req, resp);
        }
    }

    return -1;
}

int CompClient::PhxMqttPublish(const phxrpc::MqttPublishPb &req, phxrpc::MqttPubackPb *resp) {
    const phxrpc::Endpoint_t *ep = global_compclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_compclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_compclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());

            CompStub stub(socket, *(global_compclient_monitor_.get()));
            return stub.PhxMqttPublish(req, resp);
        }
    }

    return -1;
}

int CompClient::PhxMqttDisconnect(const phxrpc::MqttDisconnectPb &req) {
    const phxrpc::Endpoint_t *ep = global_compclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_compclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_compclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());

            CompStub stub(socket, *(global_compclient_monitor_.get()));
            return stub.PhxMqttDisconnect(req);
        }
    }

    return -1;
}

int CompClient::PhxEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp) {
    const phxrpc::Endpoint_t *ep = global_compclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_compclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_compclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());

            CompStub stub(socket, *(global_compclient_monitor_.get()));
            return stub.PhxEcho(req, resp);
        }
    }

    return -1;
}

int CompClient::PhxBatchEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp) {
    int ret = -1;
    size_t echo_server_count = 2;
    uthread_begin;
    for (size_t i{0}; echo_server_count > i; ++i) {
        uthread_t [=, &uthread_s, &ret](void *) {
            const phxrpc::Endpoint_t *ep = global_compclient_config_.GetByIndex(i);
            if (ep != nullptr) {
                phxrpc::UThreadTcpStream socket;
                if(phxrpc::PhxrpcTcpUtils::Open(&uthread_s, &socket, ep->ip, ep->port,
                            global_compclient_config_.GetConnectTimeoutMS(), *(global_compclient_monitor_.get()))) {
                    socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());
                    CompStub stub(socket, *(global_compclient_monitor_.get()));
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

int CompClient::Handle(const magna::AppRequest &req, magna::AppResponse *resp) {
    const phxrpc::Endpoint_t *ep = global_compclient_config_.GetRandom();

    if (ep != nullptr) {
        phxrpc::BlockTcpStream socket;
        bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep->ip, ep->port,
                    global_compclient_config_.GetConnectTimeoutMS(), nullptr, 0,
                    *(global_compclient_monitor_.get()));
        if (open_ret) {
            socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());

            CompStub stub(socket, *(global_compclient_monitor_.get()));
            return stub.Handle(req, resp);
        }
    }

    return -1;
}


int CompClient::Handle(const phxrpc::Endpoint_t &ep, magna::AppRequest &req, magna::AppResponse *resp)
{
	phxrpc::BlockTcpStream socket;
	bool open_ret = phxrpc::PhxrpcTcpUtils::Open(&socket, ep.ip, ep.port,
		global_compclient_config_.GetConnectTimeoutMS(), nullptr, 0,
		*(global_compclient_monitor_.get()));
	if (open_ret) {
		socket.SetTimeout(global_compclient_config_.GetSocketTimeoutMS());

		CompStub stub(socket, *(global_compclient_monitor_.get()));
		return stub.Handle(req, resp);
	}

	return -1;
}