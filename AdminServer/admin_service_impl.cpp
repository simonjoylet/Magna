/* admin_service_impl.cpp

 Generated by phxrpc_pb2service from admin.proto

*/

#include "admin_service_impl.h"

#include "admin_server_config.h"
#include "admin.pb.h"
#include "phxrpc/file.h"
#include "AdminData.h"
#include <mutex>


AdminServiceImpl::AdminServiceImpl(ServiceArgs_t &app_args)
    : args_(app_args) {
}

AdminServiceImpl::~AdminServiceImpl() {
}

int AdminServiceImpl::PhxMqttConnect(const phxrpc::MqttConnectPb &req, phxrpc::MqttConnackPb *resp) {
    return 0;
}

int AdminServiceImpl::PhxMqttPublish(const phxrpc::MqttPublishPb &req, phxrpc::MqttPubackPb *resp) {
    // TODO: process req.content()

    return -1;
}

int AdminServiceImpl::PhxMqttDisconnect(const phxrpc::MqttDisconnectPb &req) {
    return 0;
}

int AdminServiceImpl::PhxEcho(const google::protobuf::StringValue &req, google::protobuf::StringValue *resp) {
    resp->set_value(req.value());

    return 0;
}

int AdminServiceImpl::RegisterNode(const magna::RegisterNodeRequest &req, magna::RegisterNodeResponse *resp) {
	localdata::NodeInfo nodeInfo;
	nodeInfo.addr.ip = req.addr().ip();
	nodeInfo.addr.port = req.addr().port();
	nodeInfo.type = req.type();

	extern phxrpc::ClientConfig global_nodeclient_config_;
	
	phxrpc::Endpoint_t ep;
	snprintf(ep.ip, sizeof(ep.ip), "%s", nodeInfo.addr.ip.c_str());
	ep.port = nodeInfo.addr.port;

	AdminData * ad = AdminData::GetInstance();
	ad->lock();
	global_nodeclient_config_.Add(ep);
	ad->m_nodeList.insert(make_pair(req.addr().ip(), nodeInfo));
	ad->unlock();
	
	resp->set_ack(true);
	resp->set_msg("Register success");
    return 0;
}

int AdminServiceImpl::NodeHeatbeat(const magna::NodeHeartbeatRequest &req, magna::NodeHeartbeatResponse *resp) {
	AdminData * ad = AdminData::GetInstance();
	ad->lock();

	if (ad->m_nodeList.find(req.addr().ip()) == ad->m_nodeList.end())
	{
		printf("\nNode not registered: %s\n", req.addr().ip().c_str());
		resp->set_ack(false);
		resp->set_msg("Node not found");
	}
	else
	{
		localdata::NodeInfo & info = ad->m_nodeList.find(req.addr().ip())->second;
		info.heatbeat = 0;
		info.cpuload = req.load().cpu();
		info.diskload = req.load().disk();
		auto rtt = req.load().rtt();
		if (rtt.size() > 0)
		{
			info.netrtt.clear();
		}
		for (auto it = rtt.begin(); it != rtt.end(); ++it)
		{
			info.netrtt.insert(make_pair(it->first, it->second));
		}
		
		resp->set_ack(true);
		resp->set_msg("success");

		printf("\nNode status updated: %s cpu: %f\n", req.addr().ip().c_str(), req.load().cpu());
	}

	ad->unlock();
    return 0;
}

int AdminServiceImpl::RegisterService(const magna::RegisterServiceRequest &req, magna::RegisterServiceResponse *resp) {

	AdminData * ad = AdminData::GetInstance();
	int serviceId = ad->GetNewServiceId();
	localdata::ServiceInfo info;
	info.addr.ip = req.addr().ip();
	info.addr.port = req.addr().port();
	info.heatbeat = 0;
	info.id = serviceId;
	info.name = req.servicename();
	ad->lock();
	ad->m_serviceList[serviceId] = info;
	ad->unlock();

	resp->set_ack(true);
	resp->set_msg("success");
	resp->set_serviceid(serviceId);
    return 0;
}

int AdminServiceImpl::ServiceHeatbeat(const magna::ServiceHeartbeatRequest &req, magna::ServiceHeartbeatResponse *resp) {
    
	AdminData * ad = AdminData::GetInstance();
	uint32_t serviceId = req.serviceid();
	ad->lock();

	if (ad->m_serviceList.find(serviceId) == ad->m_serviceList.end())
	{
		printf("\n[ERROR]: Service not found, id: %d\n", serviceId);
		resp->set_ack(false);
		resp->set_msg("serviceId not found");
	}
	else
	{
		localdata::ServiceInfo & info = ad->m_serviceList[serviceId];
		info.heatbeat = 0;
		info.lamda = req.lamda();
		info.queueLength = req.queuelength();
		
		resp->set_ack(true);
		resp->set_msg("success");
	}
	
	ad->unlock();
	return 0;
}

int AdminServiceImpl::GetServiceTable(const magna::ServiceTableRequest &req, magna::ServiceTableResponse *resp) {
	
	AdminData * ad = AdminData::GetInstance();
	ad->lock();
	vector<localdata::RouterItem> & router = ad->m_router;
	magna::ServiceScale * ss = NULL;
	for (uint32_t i = 0; i < router.size(); ++i)
	{
		localdata::RouterItem & item = router[i];
		ss = resp->add_routertable();
		ss->set_name(item.compName);
		ss->mutable_ep()->set_ip(item.ip);
		ss->mutable_ep()->set_port(item.port);
		ss->set_percentage(item.percentage);
	}
	ad->unlock();

// 	// 用两个组件进行测试 
// 
// 	ss = resp->add_routertable();
// 	ss->set_name("Comp1");
// 	ss->mutable_ep()->set_ip("223.3.69.5");
// 	ss->mutable_ep()->set_port(20001);
// 	ss->set_percentage(0.3);
// 
// 	ss = resp->add_routertable();
// 	ss->set_name("Comp1");
// 	ss->mutable_ep()->set_ip("223.3.69.5");
// 	ss->mutable_ep()->set_port(20003);
// 	ss->set_percentage(0.7);
// 	
// 	ss = resp->add_routertable();
// 	ss->set_name("Comp2");
// 	ss->mutable_ep()->set_ip("223.3.69.5");
// 	ss->mutable_ep()->set_port(20002);
// 	ss->set_percentage(1);

    return 0;
}