/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
* Copyright (c) 2011-2015  Regents of the University of California.
*
* This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
* contributors.
*
* ndnSIM is free software: you can redistribute it and/or modify it under the terms
* of the GNU General Public License as published by the Free Software Foundation,
* either version 3 of the License, or (at your option) any later version.
*
* ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
**/
// congestioncontrolsenario2.cpp

#include <memory>
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/command-line.h"
#include <list>
#include <vector>
#include "ns3/queue.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/network-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traced-callback.h"
#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ndnSIM/NFD/daemon/face/face.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"

namespace ns3 {
int main(int argc, char* argv[])
{
CommandLine cmd;
cmd.Parse (argc, argv);
cmd.Parse(argc, argv);

AnnotatedTopologyReader topologyReader("", 25);
topologyReader.SetFileName("src/ndnSIM/examples/topologies/palapa ring.txt");
topologyReader.Read();

// Install NDN stack on all nodes
ndn::StackHelper ndnHelper;
ndnHelper.SetDefaultRoutes(true);

// Getting containers for the consumer/producer
Ptr<Node> GR1= Names::Find<Node>("balikpapan");
Ptr<Node> GR2= Names::Find<Node>("medan");
Ptr<Node> GR3= Names::Find<Node>("makassar");
Ptr<Node> GR4= Names::Find<Node>("jakarta");
Ptr<Node> GR5= Names::Find<Node>("bali");
Ptr<Node> GR6= Names::Find<Node>("ambon");
Ptr<Node> GR7= Names::Find<Node>("papua");
Ptr<Node> consumer1= Names::Find<Node>("Bogor");


// Initial condition : no cache
ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache");
ndnHelper.InstallAll();

// Choosing forwarding strategy

ndn::StrategyChoiceHelper::InstallAll("/com/news", "/localhost/nfd/strategy/multicast");

// install application helper

ndn::AppHelper consumerHelper1("ns3::ndn::ConsumerCbr");
consumerHelper1.SetAttribute("Frequency", StringValue("300")); // 300 interests a second


// that will express interests in /com/video namespace

consumerHelper1.SetPrefix("/com/video");
consumerHelper1.Install(consumer1);


ndn::AppHelper producerHelper1("ns3::ndn::Producer");
producerHelper1.SetAttribute("PayloadSize", StringValue("1024"));
producerHelper1.SetPrefix("/com/video");
producerHelper1.Install(jakarta);


// Collect traffic information

// Drop tracer
L2RateTracer::InstallAll("drop-trace_GEANT2_myproposal.txt", Seconds(1));
// Rate tracer
ndn::L3RateTracer::InstallAll("rate-trace_GEANT2_myproposal.txt", Seconds(1));
// Delay Rate tracer
ndn::AppDelayTracer::InstallAll("Delay-trace_GEANT2_myproposal.txt");
// Delay CS
ndn::CsTracer::InstallAll("cs-trace_GEANT2_myproposal.txt", Seconds(1));
//Running time
Simulator::Stop(Seconds(14400)); // 14400 seconds= 4 hours
Simulator::Run();

return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
return ns3::main(argc, argv);

}
