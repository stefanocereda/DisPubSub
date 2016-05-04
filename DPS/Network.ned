package dps;

simple client
{
    @display("i=old/laptop");
    gates:
        inout gate[];
}

simple broker
{
    @display("i=device/server2");
    gates:
        inout gate[];
}

simple delayer
{
    @display("i=device/server");
    gates:
        inout gate[];
}

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
            @display("p=44,158");
        }
        client2: client {
            @display("p=59,284");
        }
        broker1: broker {
            @display("p=192,222");
        }
        broker2: broker {
            @display("p=347,125");
        }
        client3: client {
            @display("p=237,35");
        }
        client4: client {
            @display("p=400,24");
        }
        broker3: broker {
            @display("p=569.1925,201.8875");
        }
        broker5: broker {
            @display("p=853.1375,285.2475");
        }
        broker6: broker {
            @display("p=472,336");
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

        client: client {
            @display("p=129,98");
        }
        client11: client {
            @display("p=222,335");
        }
        client12: client {
            @display("p=359,227");
        }
        client13: client {
            @display("p=359,452");
        }
        client14: client {
            @display("p=569,70");
        }
        client15: client {
            @display("p=800,35");
        }
        client16: client {
            @display("p=831,409");
        }
        client17: client {
            @display("p=662,316");
        }
        delayer: delayer {
            @display("p=448,159");
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
        delayer.gate++ <--> ChannelBroker <--> broker3.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker4.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker5.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker6.gate++;

        client.gate++ <--> broker1.gate++;
        client11.gate++ <--> broker1.gate++;
        client12.gate++ <--> broker2.gate++;
        client14.gate++ <--> broker3.gate++;
        client17.gate++ <--> broker3.gate++;
        client13.gate++ <--> broker6.gate++;
        client16.gate++ <--> broker5.gate++;
        client15.gate++ <--> broker4.gate++;
        delayer.gate++ <--> ChannelBroker <--> broker2.gate++;
}
