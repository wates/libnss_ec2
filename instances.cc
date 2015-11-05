
#include "instances.h"

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/Region.h>
#include <aws/core/utils/Outcome.h>

#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/base64/Base64.h>
#include <aws/core/utils/Array.h>

#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/DescribeInstancesRequest.h>
#include <aws/ec2/model/DescribeInstancesResponse.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namemap* ListInstances(int *len){
    std::vector<namemap> names;
    //const std::shared_ptr<Aws::Utils::Logging::LogSystemInterface> logr(new Aws::Utils::Logging::ConsoleLogSystem(Aws::Utils::Logging::LogLevel::Debug));
    //Aws::Utils::Logging::InitializeAWSLogging(logr);

    Aws::Auth::EnvironmentAWSCredentialsProvider env_cred;
    //Aws::Auth::AWSCredentials cred(std::getenv("AWS_ACCESS_KEY_ID"),
    //    std::getenv("AWS_SECRET_ACCESS_KEY"), std::getenv("AWS_SESSION_TOKEN"));
    Aws::Client::ClientConfiguration config;
    config.region = Aws::Region::AP_NORTHEAST_1;

    //Aws::EC2::EC2Client ec2(cred, config);
    Aws::EC2::EC2Client ec2(env_cred.GetAWSCredentials(), config);
    Aws::EC2::Model::DescribeInstancesRequest req;
    Aws::EC2::Model::DescribeInstancesOutcome outcome = ec2.DescribeInstances(req);
    if (outcome.IsSuccess()){
        Aws::EC2::Model::DescribeInstancesResponse res = outcome.GetResultWithOwnership();
        for (auto reservation : res.GetReservations()){
            for (auto instance : reservation.GetInstances()){
                if (0 < instance.GetPublicIpAddress().size()){
                    for (auto tag : instance.GetTags()){
                        if (tag.GetKey() == "Name"){
                            namemap name;
                            name.address = ntohl(inet_addr(instance.GetPublicIpAddress().c_str()));
                            strcpy(name.name, tag.GetValue().c_str());
                            names.push_back(name);
                        }
                    }
                }
            }
        }
    }

    namemap *ret = new namemap[names.size()];
    memcpy(ret, &*names.begin(), sizeof(namemap)*names.size());
    *len = names.size();
    return ret;
}

void FreeMap(namemap *p){
    delete[]p;
}

unsigned int FindAddress(const char *name){
    unsigned int addr = 0x00000000;
    int len = 0;
    namemap *names = ListInstances(&len);
    for (int i = 0; i < len; i++){
        if (0 == strcmp(name, names[i].name))
            addr = names[i].address;
    }
    FreeMap(names);
    return addr;
}
