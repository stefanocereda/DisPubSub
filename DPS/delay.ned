package dps;

simple delayer
{
    @display("i=device/server1");
    gates:
        inout gate[];
}

network Delay
{
    @display("bgb=805,376");
    types:
        channel ChannelFast extends ned.DelayChannel
        {
            delay = 100ms;
        }

    submodules:
        broker: broker {
            @display("p=253.82375,171.9125");
        }
        client: client {
            @display("p=134.49625,105.17");
        }
        client1: client {
            @display("p=134.49625,258.88");
        }
        client2: client {
            @display("p=527.8725,170.90125");
        }

        delayer: delayer {
            @display("p=393.37625,171.9125;i=device/server");
        }
    connections allowunconnected:


        client.gate++ <--> ChannelFast <--> broker.gate++;
        broker.gate++ <--> ChannelFast <--> client1.gate++;

        broker.gate++ <--> ChannelFast <--> delayer.gate++;
        delayer.gate++ <--> ChannelFast <--> client2.gate++;
}
