#include "comp_service_impl.cpp"
#include "InnerHandle.h"
#include <iostream>
#include <math.h>
std::string g_compName = "Comp_1";
int InnerHandle(const magna::AppRequest &req, magna::AppResponse *resp) {
	//usleep(5000);
	static uint32_t count = 0;
	uint64_t ts = phxrpc::Timer::GetSteadyClockMS();
	printf("%d req received. id = %d, servicename = %s, clienttype = %d, ", ++count, req.id(), req.servicename().c_str(), req.clienttype());
	uint32_t calNum = 10000 * 40;
	for (uint32_t i = 0; i < calNum; ++i)
	{
		pow(rand(), i);
	}


	magna::RetRequest retReq;
	magna::RetResponse retRsp;
	retReq.set_id(req.id());
	int ret = g_simuProxy->GetRet(retReq, &retRsp);
	printf("time used: %dms, return %s\n", phxrpc::Timer::GetSteadyClockMS() - ts, ret == 0 ? "succ" : "fail");

    return 0;
}

