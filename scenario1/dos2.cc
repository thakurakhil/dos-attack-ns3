#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
 
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("DOS");
 
void TearDownLink (Ptr<Node> nodeA, Ptr<Node> nodeB, uint32_t interfaceA, uint32_t interfaceB)
{
  nodeA->GetObject<Ipv4> ()->SetDown (interfaceA);
  nodeB->GetObject<Ipv4> ()->SetDown (interfaceB);
}

/*
class DdosApp : public App
{
public:
  //static TypeId GetTypeId ();
 
  DdosApp ();
  virtual ~DdosApp ();
 
 
 
   // @brief Actually send packet
   
  void
  SendPacket ();
 
protected:
  // from App
  virtual void
  StartApplication ();
 
  virtual void
  StopApplication ();
 
 
  void
  DelayedStop ();
 
private:
  UniformVariable m_rand; ///< @brief nonce generator
  UniformVariable m_jitter; ///< @brief nonce generator
  uint32_t        m_seq;  ///< @brief currently requested sequence number
  EventId         m_nextSendEvent;
  Time            m_lifetime;
 
  Time m_avgGap; ///< @brief average gap between interests (should be short, but still should be, otherwise simulation will never finishes)
  // default interest
  // InterestHeader m_defaultInterest;
  Name m_prefix;
  bool m_evilBit;
  bool m_dataBasedLimit;
};
 
TypeId
DdosApp::GetTypeId ()
{
  static TypeId tid = TypeId ("DdosApp")
    .SetParent<App> ()
    .AddConstructor<DdosApp> ()
 
    .AddAttribute ("AvgGap", "AverageGap",
       StringValue ("1ms"),
                   MakeTimeAccessor (&DdosApp::m_avgGap),
                   MakeTimeChecker ())
    .AddAttribute ("Prefix","Name of the Interest",
                   StringValue ("/"),
                   MakeNameAccessor (&DdosApp::m_prefix),
                   MakeNameChecker ())
    .AddAttribute ("LifeTime","Interest lifetime",
                   StringValue ("1s"),
                   MakeTimeAccessor (&DdosApp::m_lifetime),
                   MakeTimeChecker ())
    .AddAttribute ("Evil", "Evil bit",
                   BooleanValue (false),
                   MakeBooleanAccessor (&DdosApp::m_evilBit),
                   MakeBooleanChecker ())
    .AddAttribute ("DataBasedLimits", "Calculate frequency based on how many data packets can be returned",
                   BooleanValue (true),
                   MakeBooleanAccessor (&DdosApp::m_dataBasedLimit),
                   MakeBooleanChecker ())
    ;
  return tid;
}
 
DdosApp::DdosApp ()
  : m_rand (0, std::numeric_limits<uint32_t>::max ())
  , m_jitter (0,1)
  , m_seq (0)
{
}
 
DdosApp::~DdosApp ()
{
}
 
 
void
DdosApp::SendPacket ()
{
  m_seq++;
  // send packet
  Ptr<NameComponents> nameWithSequence = Create<NameComponents> (m_prefix);
  nameWithSequence->appendSeqNum (m_seq);
 
  Ptr<Interest> interest = Create<Interest> ();
  interest->SetNonce            (m_rand.GetValue ());
  interest->SetName             (nameWithSequence);
  interest->SetInterestLifetime (m_lifetime);
       
  NS_LOG_INFO ("> Interest for " << m_seq << ", lifetime " << m_lifetime.ToDouble (Time::S) << "s");
 
  m_face->ReceiveInterest (interest);
  m_transmittedInterests (interest, this, m_face);
 
  // std::cout << "Size: " << packet->GetSize () << std::endl;
 
  // NS_LOG_DEBUG (m_avgGap+MilliSeconds (m_rand.GetValue ()));
  Time nextTime = m_avgGap + Time::FromDouble (m_jitter.GetValue (), Time::US);
  NS_LOG_DEBUG ("next time: " << nextTime.ToDouble (Time::S) << "s");
  m_nextSendEvent = Simulator::Schedule (nextTime,
                 &DdosApp::SendPacket, this);
}
 
void
DdosApp::StartApplication ()
{
  // calculate outgoing rate and set Interest generation rate accordingly
 
  double sumOutRate = 0.0;
 
  Ptr<Node> node = GetNode ();
  for (uint32_t deviceId = 0; deviceId < node->GetNDevices (); deviceId ++)
    {
      Ptr<PointToPointNetDevice> device = DynamicCast<PointToPointNetDevice> (node->GetDevice (deviceId));
      if (device == 0)
        continue;
 
      DataRateValue dataRate; device->GetAttribute ("DataRate", dataRate);
      sumOutRate += (dataRate.Get ().GetBitRate () / 8);
    }
 
  double maxInterestTo = sumOutRate / 40;
  double maxDataBack = sumOutRate / 1146;
 
  if (m_evilBit)
    {
      if (m_dataBasedLimit)
        {
          m_avgGap = Seconds (0.1 / maxDataBack);
        }
      else
        {
          m_avgGap = Seconds (1 / maxInterestTo);
        }
      // std::cout << "evil Gap: " << m_avgGap.ToDouble (Time::S) << "s\n";
    }
  else
    {
      // m_avgGap = Seconds (2 * 1 / maxDataBack); // request 50% of maximum link capacity
      // std::cout << "good Gap: " << m_avgGap.ToDouble (Time::S) << "s\n";
    }
 
  App::StartApplication ();
  SendPacket ();
}
 
void
DdosApp::StopApplication ()
{
  m_nextSendEvent.Cancel ();
 
  // std::cerr << "# references before delayed stop: " << m_face->GetReferenceCount () << std::endl;
  Simulator::Schedule (Seconds (10.0), &DdosApp::DelayedStop, this);
}
 
void
DdosApp::DelayedStop ()
{
  // std::cerr << "# references after delayed stop: " << m_face->GetReferenceCount () << std::endl;
  App::StopApplication ();
}
*/
 
 

int main (int argc, char **argv)
{
  bool verbose = false;
  bool printRoutingTables = true;
  bool showPings = false;
  std::string SplitHorizon ("SplitHorizon");
 
  CommandLine cmd;
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.AddValue ("printRoutingTables", "Print routing tables at 30, 60 and 90 seconds", printRoutingTables);
  cmd.AddValue ("showPings", "Show Ping6 reception", showPings);
  cmd.AddValue ("splitHorizonStrategy", "Split Horizon strategy to use (NoSplitHorizon, SplitHorizon, PoisonReverse)", SplitHorizon);
  cmd.Parse (argc, argv);
 
  if (verbose)
    {
      LogComponentEnableAll (LogLevel (LOG_PREFIX_TIME | LOG_PREFIX_NODE));
      LogComponentEnable ("RipSimpleRouting", LOG_LEVEL_INFO);
      LogComponentEnable ("Rip", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv4Interface", LOG_LEVEL_ALL);
      LogComponentEnable ("Icmpv4L4Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv4L3Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("ArpCache", LOG_LEVEL_ALL);
      LogComponentEnable ("V4Ping", LOG_LEVEL_ALL);
    }
 
  if (SplitHorizon == "NoSplitHorizon")
    {
      Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::NO_SPLIT_HORIZON));
    }
  else if (SplitHorizon == "SplitHorizon")
    {
      Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::SPLIT_HORIZON));
    }
  else
    {
      Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::POISON_REVERSE));
    }
 
  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> server = CreateObject<Node> ();
  Names::Add ("ServerNode", server);
  Ptr<Node> R1 = CreateObject<Node> ();
  Names::Add ("Router1", R1);
  Ptr<Node> R2 = CreateObject<Node> ();
  Names::Add ("Router2", R2);
  Ptr<Node> g1 = CreateObject<Node> ();
  Names::Add ("GoodNode1", g1);
  Ptr<Node> g2 = CreateObject<Node> ();
  Names::Add ("GoodNode2", g2);
  Ptr<Node> g3= CreateObject<Node> ();
  Names::Add ("GoodNode3", g3);
  Ptr<Node> e1= CreateObject<Node> ();
  Names::Add ("BadNode1", e1);
  Ptr<Node> e2= CreateObject<Node> ();
  Names::Add ("BadNode2", e2);
  Ptr<Node> e3= CreateObject<Node> ();
  Names::Add ("BadNode3", e3);
 
  NodeContainer net1 (server, R1);
  NodeContainer net2 (R1, R2);
  NodeContainer net3 (R2, g1);
  NodeContainer net4 (R2, g2);
  NodeContainer net5 (R2, g3);
  NodeContainer net6 (R1, e1);
  NodeContainer net7 (R1, e2);
  NodeContainer net8 (R2, e3);
 
 
  NodeContainer routers (R1, R2);
  NodeContainer nodes (server, g1, g2, g3);
  NodeContainer goodNodes (g1,g2,g3);
  NodeContainer badNodes (e1,e2,e3);
   
 
 
 
  NS_LOG_INFO ("Create channels.");
  CsmaHelper csmaG, csmaE, csmaR;
  csmaR.SetChannelAttribute ("DataRate", StringValue ("200Mbps"));
  csmaR.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
 
  csmaG.SetChannelAttribute ("DataRate", StringValue ("60Mbps"));
  csmaG.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
 
  csmaE.SetChannelAttribute ("DataRate", StringValue ("80Mbps"));
  csmaE.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
 
  NetDeviceContainer ndc1 = csmaR.Install (net1);
  NetDeviceContainer ndc2 = csmaR.Install (net2);
  NetDeviceContainer ndc3 = csmaG.Install (net3);
  NetDeviceContainer ndc4 = csmaG.Install (net4);
  NetDeviceContainer ndc5 = csmaG.Install (net5);
  NetDeviceContainer ndc6 = csmaE.Install (net6);
  NetDeviceContainer ndc7 = csmaE.Install (net7);
  NetDeviceContainer ndc8 = csmaE.Install (net8);
 
  NS_LOG_INFO ("Create IPv4 and routing");
  RipHelper ripRouting;
 
  // Rule of thumb:
  // Interfaces are added sequentially, starting from 0
  // However, interface 0 is always the loopback...
  ripRouting.ExcludeInterface (R1, 1);
  ripRouting.ExcludeInterface (R1, 3);
  ripRouting.ExcludeInterface (R1, 4);
  ripRouting.ExcludeInterface (R2, 2);
  ripRouting.ExcludeInterface (R2, 3);
  ripRouting.ExcludeInterface (R2, 4);
  ripRouting.ExcludeInterface (R2, 5);
   
 
  ripRouting.SetInterfaceMetric (R1, 1, 8);
  ripRouting.SetInterfaceMetric (R1, 2, 10);
  ripRouting.SetInterfaceMetric (R1, 3, 10);
 
   
 
  Ipv4ListRoutingHelper listRH;
  listRH.Add (ripRouting, 0);
//  Ipv4StaticRoutingHelper staticRh;
//  listRH.Add (staticRh, 5);
 
  InternetStackHelper internet;
  internet.SetIpv6StackInstall (false);
  internet.SetRoutingHelper (listRH);
  internet.Install (routers);
 
  InternetStackHelper internetNodes;
  internetNodes.SetIpv6StackInstall (false);
  internetNodes.Install (nodes);
  internetNodes.Install (badNodes);
 
  // Assign addresses.
  // The source and destination networks have global addresses
  // The "core" network just needs link-local addresses for routing.
  // We assign global addresses to the routers as well to receive
  // ICMPv6 errors.
  NS_LOG_INFO ("Assign IPv4 Addresses.");
  Ipv4AddressHelper ipv4;
 
  ipv4.SetBase (Ipv4Address ("10.0.0.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic1 = ipv4.Assign (ndc1);
 
  ipv4.SetBase (Ipv4Address ("10.0.1.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic2 = ipv4.Assign (ndc2);
 
  ipv4.SetBase (Ipv4Address ("10.0.2.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic3 = ipv4.Assign (ndc3);
 
  ipv4.SetBase (Ipv4Address ("10.0.3.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic4 = ipv4.Assign (ndc4);
 
  ipv4.SetBase (Ipv4Address ("10.0.4.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic5 = ipv4.Assign (ndc5);
 
  ipv4.SetBase (Ipv4Address ("10.0.5.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic6 = ipv4.Assign (ndc6);
 
  ipv4.SetBase (Ipv4Address ("10.0.6.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc7);
 
  ipv4.SetBase (Ipv4Address ("10.0.7.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic8 = ipv4.Assign (ndc8);
 
 
  Ptr<Ipv4StaticRouting> staticRouting;
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (server->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.0.2", 1 );
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (g1->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.2.1", 1 );
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (g2->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.3.1", 1 );
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (g3->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.4.1", 1 );
 staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (e1->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.5.1", 1 );
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (e2->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.6.1", 1 );
 staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (e3->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.7.1", 1 );
 
 
  if (printRoutingTables)
    {
      RipHelper routingHelper;
 
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
 
      routingHelper.PrintRoutingTableAt (Seconds (10.0), R1, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (10.0), R2, routingStream);
     
      routingHelper.PrintRoutingTableAt (Seconds (50.0), R1, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (50.0), R2, routingStream);
     
      //routingHelper.PrintRoutingTableAt (Seconds (90.0), R1, routingStream);
      //routingHelper.PrintRoutingTableAt (Seconds (90.0), R2, routingStream);
      }
 
  NS_LOG_INFO ("Create Applications.");
 
    uint16_t port = 4000;
    UdpServerHelper serverHelper (port);
    ApplicationContainer apps = serverHelper.Install (server);
    apps.Start (Seconds (5.0));
    apps.Stop (Seconds (110.0));
 
 
   /*Check here for bugs*/
    Address serverAddress = Address(iic1.GetAddress(0));
    uint32_t MaxPacketSize = 1024;
    Time interPacketInterval = Seconds (3); //20 packets per second
    uint32_t maxPacketCount = 1200;
    UdpClientHelper client (serverAddress, port);
    client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
    client.SetAttribute ("Interval", TimeValue (interPacketInterval));
    client.SetAttribute ("PacketSize", UintegerValue (MaxPacketSize));
    apps = client.Install (g1);
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (60.0));
 
    apps = client.Install (g2);
    apps.Start (Seconds (15.0));
    apps.Stop (Seconds (60.0));
 
    apps = client.Install (g3);
    apps.Start (Seconds (30.0));
    apps.Stop (Seconds (60.0));
 
    Time interPacketIntervalB1 = Seconds (0.75);
    Time interPacketIntervalB = Seconds (1);
    uint32_t maxPacketCountB = 12000;
    UdpClientHelper clientB (serverAddress, port);
    clientB.SetAttribute ("MaxPackets", UintegerValue (maxPacketCountB));
    clientB.SetAttribute ("Interval", TimeValue (interPacketIntervalB));
    clientB.SetAttribute ("PacketSize", UintegerValue (MaxPacketSize));
    apps = clientB.Install (e1);
    apps.Start (Seconds (10.0));
    apps.Stop (Seconds (60.0));
 
    apps = clientB.Install (e2);
    apps.Start (Seconds (10.0));
    apps.Stop (Seconds (60.0));
    
    UdpClientHelper clientB1 (serverAddress, port);
    clientB1.SetAttribute ("MaxPackets", UintegerValue (maxPacketCountB));
    clientB1.SetAttribute ("Interval", TimeValue (interPacketIntervalB1));
    clientB1.SetAttribute ("PacketSize", UintegerValue (MaxPacketSize));
    
    apps = clientB1.Install (e3);
    apps.Start (Seconds (30.0));
    apps.Stop (Seconds (60.0));
 
 
/*
  uint32_t packetSize = 1024;
  Time interPacketInterval = Seconds (1.0);
  V4PingHelper ping ("10.0.0.1");
 
  ping.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping.SetAttribute ("Size", UintegerValue (packetSize));
  if (showPings)
    {
      ping.SetAttribute ("Verbose", BooleanValue (true));
    }
 
 
  ApplicationContainer app1 = ping.Install (g1);
  app1.Start (Seconds (1.0));
  app1.Stop (Seconds (110.0));
 
  ApplicationContainer app2 = ping.Install (g2);
  app2.Start (Seconds (10.0));
  app2.Stop (Seconds (110.0));
 
  ApplicationContainer app3 = ping.Install (g3);
  app3.Start (Seconds (20.0));
  app3.Stop (Seconds (110.0));
 */
  //AsciiTraceHelper ascii;
  //csma.EnableAsciiAll (ascii.CreateFileStream ("rip-simple-routing.tr"));
  //csma.EnablePcapAll ("rip-simple-routing", true);
 
  //Simulator::Schedule (Seconds (40), &TearDownLink, b, d, 3, 2);
 
  /* Now, do the actual simulation. */
 
 
/*
 
  AppHelper evilAppHelper ("DdosApp");
  evilAppHelper.SetAttribute ("Evil", BooleanValue (true));
  evilAppHelper.SetAttribute ("LifeTime", StringValue ("1s"));
  evilAppHelper.SetAttribute ("DataBasedLimits", BooleanValue (true));
 
  AppHelper goodAppHelper ("DdosApp");
  goodAppHelper.SetAttribute ("LifeTime",  StringValue ("1s"));
  goodAppHelper.SetAttribute ("DataBasedLimits", BooleanValue (true));
 
  ApplicationContainer goodApp;
 
  goodAppHelper.SetAttribute ("AvgGap", TimeValue (Seconds (1.100 / maxNonCongestionShare)));
 
  goodApp.Add (goodAppHelper.Install (g1));
  goodApp.Add (goodAppHelper.Install (g2));
  goodApp.Add (goodAppHelper.Install (g3));
  UniformVariable rand (0, 1);
  goodApp.Start (Seconds (0.0) + Time::FromDouble (rand.GetValue (), Time::S));
 
*/
 
 
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();
 
 
  AnimationInterface anim ("animation.xml");
  anim.SetConstantPosition (net1.Get(0), 50, 10);
  anim.SetConstantPosition (net2.Get(0), 40, 20);
  anim.SetConstantPosition (net2.Get(1), 35, 30);
  anim.SetConstantPosition (net3.Get(1), 10, 50);
  anim.SetConstantPosition (net4.Get(1), 25, 50);
  anim.SetConstantPosition (net5.Get(1), 35, 50);
  anim.SetConstantPosition (net6.Get(1), 60, 10);
  anim.SetConstantPosition (net7.Get(1), 50, 30);
  anim.SetConstantPosition (net8.Get(1), 45, 50);
 
 
  AsciiTraceHelper ascii;
  csmaR.EnableAsciiAll (ascii.CreateFileStream ("dos-attack.tr"));
  csmaR.EnablePcapAll ("dos-attack", true);
 
 NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (80.0));
 
 
 
 
  Simulator::Run ();
  flowMonitor->SerializeToXmlFile("dosFlowMonitor.xml", true, true);
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
