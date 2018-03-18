/* comp_main.cpp

 Generated by phxrpc_pb2server from comp.proto

*/

#include <iostream>
#include <memory>
#include <unistd.h>
#include <signal.h>

#include "phxrpc_comp_dispatcher.h"
#include "comp_service_impl.h"
#include "comp_server_config.h"

#include "phxrpc/rpc.h"
#include "phxrpc/msg.h"
#include "phxrpc/file.h"
#include "../AdminServer/admin_client.h"
#include <thread>
#include "InnerHandle.h"
#include "../SimuClient/simu_client.h"

using namespace std;
void Dispatch(const phxrpc::BaseRequest *request,
              phxrpc::BaseResponse *response,
              phxrpc::DispatcherArgs_t *args) {
    ServiceArgs_t *service_args = (ServiceArgs_t *)(args->service_args);

    CompServiceImpl service(*service_args, args->server_worker_uthread_scheduler);
    CompDispatcher dispatcher(service, args);

    phxrpc::BaseDispatcher<CompDispatcher> base_dispatcher(
            dispatcher, CompDispatcher::GetMqttFuncMap(),
            CompDispatcher::GetURIFuncMap());
    if (!base_dispatcher.Dispatch(request, response)) {
		if (dispatcher.Handle_Post(request, response) < 0)
		{
			response->DispatchErr();
		}
    }
}

void ShowUsage(const char *program) {
    printf("\n");
    printf("Usage: %s [-c <config>] [-d] [-l <log level>] [-v]\n", program);
    printf("\n");

    exit(0);
}


AdminClient * g_adminProxy;
uint32_t g_serviceId = 0;
bool g_hbShouldRun = true;

// 测试AdminServer是否可用
bool testAdminEcho()
{
	g_adminProxy = new AdminClient;
	//AdminClient ac;
	google::protobuf::StringValue req;
	google::protobuf::StringValue resp;
	req.set_value("Access AdminServer Success");
	int ret = g_adminProxy->PhxEcho(req, &resp);
	printf("AdminServer.PhxEcho return %d\n", ret);
	printf("resp: {\n%s}\n", resp.DebugString().c_str());
	return ret == 0;
}

int32_t RegisterComponent(string ip, uint16_t port)
{
	// 注册服务组件
	magna::RegisterServiceRequest req;
	magna::RegisterServiceResponse rsp;
	req.mutable_addr()->set_ip(ip);
	req.mutable_addr()->set_port(port);
	req.set_servicename(g_compName);
	int ret = g_adminProxy->RegisterService(req, &rsp);
// 	printf("AdminServer.RegisterService return %d\n", ret);
// 	printf("resp: \n{\n%s\n}\n", rsp.DebugString().c_str());
	if (ret == 0)
	{
		g_serviceId = rsp.serviceid();
	}
	return ret;
}

// 心跳线程
void ServiceHb(string ip, uint16_t port)
{
	printf("\nService heatbeat thread start running...\n");
	magna::ServiceHeartbeatRequest req;
	magna::ServiceHeartbeatResponse rsp;
	bool lostAdminServer = false;
	uint32_t adminNonAckCount = 0;
	while (g_hbShouldRun)
	{
		sleep(2);
		int ret = -1;
		if (lostAdminServer)
		{
			ret = RegisterComponent(ip, port);
			if (ret != 0)
			{
				continue;
			}
			lostAdminServer = false;
		}

		req.set_serviceid(g_serviceId);
		req.mutable_services(); // TODO，更新接口统计数据
		
		ret = g_adminProxy->ServiceHeatbeat(req, &rsp);
		if (0 != ret)
		{
			printf("\n[WARNNING]: ServiceHeartbeat failed\n");
			++adminNonAckCount;
			if (adminNonAckCount > 5)
			{
				lostAdminServer = true;
				printf("\n[ERROR]: AdminServer lost\n");
			}
			continue;
		}
		
		adminNonAckCount = 0;
		if (!rsp.ack())
		{
			printf("\nNode heatbeat response ERROR. msg:%s\n", rsp.msg().c_str());
		}
		else
		{
			printf("\n[DEBUG]: Service heartbeat received\n");
		}
	}
	printf("\nNode heatbeat thread stopped...\n");
}
#include "Semaphore.h"
#include <list>
Semaphore g_sema(0);
std::mutex g_queueMutex;
list<magna::AppRequest> g_reqQueue;
SimuClient * g_simuProxy;
void HandleFunc()
{
	while (true)
	{
		g_sema.wait();
		g_queueMutex.lock();
		magna::AppRequest req = g_reqQueue.front();
		g_reqQueue.pop_front();
		g_queueMutex.unlock();
		// TODO 处理请求
		static int doneCount = 0;
		magna::AppResponse rsp;
		InnerHandle(req, &rsp);
		printf("doneCount: %d\n", ++doneCount);
	}
}

int main(int argc, char **argv) {
	printf("argc: %d\n", argc);
    const char *config_file{nullptr};
    bool daemonize{false};
    int log_level{-1};
    extern char *optarg;
    int c;
    while (EOF != (c = getopt(argc, argv, "c:vl:d"))) {
        switch (c) {
            case 'c' : config_file = optarg; break;
            case 'd' : daemonize = true; break;
            case 'l' : log_level = atoi(optarg); break;

            case 'v' :
            default: ShowUsage(argv[0]); break;
        }
    }

    if (daemonize) phxrpc::ServerUtils::Daemonize();

    assert(signal(SIGPIPE, SIG_IGN) != SIG_ERR);

    //set customize log/monitor
    //phxrpc::setlog(openlog, closelog, vlog);
    //phxrpc::MonitorFactory::SetFactory(new YourSelfsMonitorFactory());

    if (nullptr == config_file) ShowUsage(argv[0]);

    CompServerConfig config;
    if (!config.Read(config_file)) ShowUsage(argv[0]);

    if (log_level > 0) config.GetHshaServerConfig().SetLogLevel(log_level);

    phxrpc::openlog(argv[0], config.GetHshaServerConfig().GetLogDir(),
            config.GetHshaServerConfig().GetLogLevel());

	// 获取当前组件绑定的IP和端口号
	string bindIP = config.GetHshaServerConfig().GetBindIP();
	uint16_t bindPort = config.GetHshaServerConfig().GetPort();

	// 向AdminServer注册服务
	AdminClient::Init("../AdminServer/admin_client.conf");
	bool adminOK = testAdminEcho();
	if (adminOK)
	{
		RegisterComponent(bindIP, bindPort);
	}

	// 起一个线程向AdminServer更新服务状态。
	std::thread hb(ServiceHb, bindIP, bindPort);
	std::thread handleTh(HandleFunc);

	SimuClient::Init("../SimuClient/simu_client.conf");
	g_simuProxy = new SimuClient();
    ServiceArgs_t service_args;
    service_args.config = &config;

    phxrpc::HshaServer server(config.GetHshaServerConfig(), Dispatch, &service_args);
    server.RunForever();

    phxrpc::closelog();

    return 0;
}

