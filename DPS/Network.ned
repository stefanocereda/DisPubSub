package dps;

simple client
{
    @display("i=old/laptop");
    gates:
        inout gate[]; //TODO: il client non dovrebbe avere un solo link?
}

//
// TODO documentation
//
simple broker
{
    @display("i=device/server2");
    gates:
        inout gate[];
}

//
// TODO documentation
//
network Network
{
    types:
        channel ChannelClient extends ned.DelayChannel
        {
            delay = 300ms;
        }
        channel ChannelBroker extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        client1: client {
            @display("p=69,179");
        }
        client2: client {
            @display("p=69,273");
        }
        broker1: broker {
            @display("p=192,222");
        }
        broker2: broker {
            @display("p=347,125");
        }
        client3: client {
            @display("p=261,42");
        }
        client4: client {
            @display("p=360,35");
        }
        broker3: broker {
            @display("p=569.1925,201.8875");
        }
        broker5: broker {
            @display("p=853.1375,285.2475");
        }
        broker6: broker {
            @display("p=587.4275,427.22");
        }
        broker4: broker {
            @display("p=877.885,106.805");
        }
        client5: client {
            @display("p=976.875,24.7475");
        }
        client6: client {
            @display("p=1031.58,105.5025");
        }
        client7: client {
            @display("p=1017.2525,272.2225");
        }
        client8: client {
            @display("p=989.9,401.17");
        }
        client9: client {
            @display("p=705.955,526.21");
        }
        client10: client {
            @display("p=514.4875,543.1425");
        }

    connections allowunconnected:
        broker1.gate++ <--> ChannelClient <--> client1.gate++;
        broker1.gate++ <--> ChannelClient <--> client2.gate++;

        broker2.gate++ <--> ChannelClient <--> client3.gate++;
        broker2.gate++ <--> ChannelClient <--> client4.gate++;

        broker4.gate++ <--> ChannelClient <--> client5.gate++;
        broker4.gate++ <--> ChannelClient <--> client6.gate++;

        broker5.gate++ <--> ChannelClient <--> client7.gate++;
        broker5.gate++ <--> ChannelClient <--> client8.gate++;

        broker6.gate++ <--> ChannelClient <--> client9.gate++;
        broker6.gate++ <--> ChannelClient <--> client10.gate++;

        broker1.gate++ <--> ChannelBroker <--> broker2.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker3.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker4.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker5.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker6.gate++;

	//Broker 1
//	broker1.out++ --> Channel --> client1.in;
//	broker1.in++ <-- Channel <-- client1.out;

//	broker1.out++ --> Channel --> client2.in;
//	broker1.in++ <-- Channel <-- client2.out;

//	broker1.out++ --> Channel --> broker2.in++;
//	broker1.in++ <-- Channel <-- broker2.out++;


	//Broker 2
//	broker2.out++ --> Channel --> client3.in;
//	broker2.in++ <-- Channel <-- client3.out;

//	broker2.out++ --> Channel --> client4.in;
//	broker2.in++ <-- Channel <-- client4.out;

//	broker2.out++ --> Channel --> broker3.in++;
//	broker2.in++ <-- Channel <-- broker3.out++;

	//Broker 3
//	broker3.out++ --> Channel --> broker4.in++;
//	broker3.in++ <-- Channel <-- broker4.out++;

//	broker3.out++ --> Channel --> broker5.in++;
//	broker3.in++ <-- Channel <-- broker5.out++;

//	broker3.out++ --> Channel --> broker6.in++;
//	broker3.in++ <-- Channel <-- broker6.out++;

	//Broker 4
//	broker4.out++ --> Channel --> client5.in;
//	broker4.in++ <-- Channel <-- client5.out;

//	broker4.out++ --> Channel --> client6.in;
//	broker4.in++ <-- Channel <-- client6.out;

	//Broker 5
//	broker5.out++ --> Channel --> client7.in;
//	broker5.in++ <-- Channel <-- client7.out;

//	broker5.out++ --> Channel --> client8.in;
//	broker5.in++ <-- Channel <-- client8.out;

	//Broker 6
//	broker6.out++ --> Channel --> client9.in;
//	broker6.in++ <-- Channel <-- client9.out;

//	broker6.out++ --> Channel --> client10.in;
//	broker6.in++ <-- Channel <-- client10.out;

}
