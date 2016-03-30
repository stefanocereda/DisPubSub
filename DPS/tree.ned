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

network Tree
{
    @display("bgb=805,376");
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
            @display("p=369,47");
        }
        broker1: broker {
            @display("p=185,105");
        }
        broker2: broker {
            @display("p=548,105");
        }
        broker3: broker {
            @display("p=86,185");
        }
        broker4: broker {
            @display("p=266,185");
        }
        broker5: broker {
            @display("p=462,185");
        }
        broker6: broker {
            @display("p=650,185");
        }
        broker7: broker {
            @display("p=36,270");
        }
        broker8: broker {
            @display("p=133,270");
        }
        broker9: broker {
            @display("p=223,270");
        }
        broker10: broker {
            @display("p=313,270");
        }
        broker11: broker {
            @display("p=412,270");
        }
        broker12: broker {
            @display("p=507,270");
        }
        broker13: broker {
            @display("p=601,270");
        }
        broker14: broker {
            @display("p=699,273");
        }

        client: client {
            @display("p=36,342");
        }
        client1: client {
            @display("p=601,342");
        }
    connections allowunconnected:

        broker.gate++ <--> ChannelBroker <--> broker1.gate++;
        broker.gate++ <--> ChannelBroker <--> broker2.gate++;
        broker1.gate++ <--> ChannelBroker <--> broker3.gate++;
        broker1.gate++ <--> ChannelBroker <--> broker4.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker6.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker5.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker7.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker8.gate++;
        broker4.gate++ <--> ChannelBroker <--> broker9.gate++;
        broker4.gate++ <--> ChannelBroker <--> broker10.gate++;
        broker5.gate++ <--> ChannelBroker <--> broker11.gate++;
        broker5.gate++ <--> ChannelBroker <--> broker12.gate++;
        broker6.gate++ <--> ChannelBroker <--> broker13.gate++;
        broker6.gate++ <--> ChannelBroker <--> broker14.gate++;

        broker7.gate++ <--> ChannelClient <--> client.gate++;
        broker13.gate++ <--> ChannelClient <--> client1.gate++;
}
