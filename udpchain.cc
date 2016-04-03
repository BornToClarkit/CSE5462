/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
  Connor Clark and Curtis Holton
*/
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  //LogComponentEnableAll(LOG_LEVEL_INFO);
  
  // make 4 nodes
  NodeContainer nodes;
  nodes.Create (4);
  
  // set point to point parameters described by lab document
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("10ms"));
  // assign nodes to devices
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes.Get(0),nodes.Get(1));
  NetDeviceContainer devices2;
  devices2=pointToPoint.Install (nodes.Get(1),nodes.Get(2));
  NetDeviceContainer devices3;
  devices3=pointToPoint.Install (nodes.Get(2),nodes.Get(3));
  //set nodes up to receive ip addresses
  InternetStackHelper stack;
  stack.Install (nodes);
  // give ip addresses to devices
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  
  
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces2 = address.Assign (devices2);
  
  
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces3 = address.Assign (devices3);
  
  //make echoServer
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (3));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  

    //make echoClient
  UdpEchoClientHelper echoClient (interfaces3.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  //make it so ip can send to each other
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  // output data files
  pointToPoint.EnablePcapAll ("udpChain");
  //end
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
