//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package dps;

network Network2
{
    @display("bgb=810,395");
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
        broker: broker {
            @display("p=109,223;b=17,5");
        }
        broker1: broker {
            @display("p=206,293");
        }
        broker2: broker {
            @display("p=268,199");
        }
        broker3: broker {
            @display("p=389,302");
        }
        broker4: broker {
            @display("p=206,127");
        }
        broker5: broker {
            @display("p=244,29");
        }
        broker6: broker {
            @display("p=113,18");
        }
        broker7: broker {
            @display("p=359,127");
        }
        broker8: broker {
            @display("p=405,38");
        }
        broker9: broker {
            @display("p=541,48");
        }
        broker10: broker {
            @display("p=655,68");
        }
        broker11: broker {
            @display("p=736,127");
        }
        broker12: broker {
            @display("p=516,148");
        }
        broker13: broker {
            @display("p=625,215");
        }
        broker14: broker {
            @display("p=508,246");
        }
        broker15: broker {
            @display("p=607,323");
        }
        client: client {
            @display("p=34,187");
        }
        client1: client {
            @display("p=34,279");
        }
        client2: client {
            @display("p=426,181");
        }
        client3: client {
            @display("p=706,335");
        }
        client4: client {
            @display("p=785,198");
        }
        client5: client {
            @display("p=766,18");
        }

    connections allowunconnected:

        broker.gate++ <--> ChannelBroker <--> broker1.gate++;
        broker1.gate++ <--> ChannelBroker <--> broker2.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker3.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker4.gate++;
        broker4.gate++ <--> ChannelBroker <--> broker5.gate++;
        broker5.gate++ <--> ChannelBroker <--> broker6.gate++;
        broker4.gate++ <--> ChannelBroker <--> broker7.gate++;
        broker7.gate++ <--> ChannelBroker <--> broker8.gate++;
        broker8.gate++ <--> ChannelBroker <--> broker9.gate++;
        broker9.gate++ <--> ChannelBroker <--> broker10.gate++;
        broker9.gate++ <--> ChannelBroker <--> broker12.gate++;
        broker12.gate++ <--> ChannelBroker <--> broker13.gate++;
        broker12.gate++ <--> ChannelBroker <--> broker14.gate++;
        broker10.gate++ <--> ChannelBroker <--> broker11.gate++;
        broker14.gate++ <--> ChannelBroker <--> broker15.gate++;

        broker.gate++ <--> ChannelClient <--> client.gate++;
        broker.gate++ <--> ChannelClient <--> client1.gate++;
        broker7.gate++ <--> ChannelClient <--> client2.gate++;
        broker11.gate++ <--> ChannelClient <--> client4.gate++;
        broker10.gate++ <--> ChannelClient <--> client5.gate++;
        broker15.gate++ <--> ChannelClient <--> client3.gate++;
}
