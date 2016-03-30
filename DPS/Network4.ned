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

network Network4
{
    @display("bgb=786,378");
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
            @display("p=394,191");
        }
        broker1: broker {
            @display("p=292,131");
        }
        broker2: broker {
            @display("p=487,124");
        }
        broker3: broker {
            @display("p=292,274");
        }
        broker4: broker {
            @display("p=495,274");
        }
        broker5: broker {
            @display("p=143,70");
        }
        broker6: broker {
            @display("p=282,31");
        }
        broker7: broker {
            @display("p=530,24");
        }
        broker8: broker {
            @display("p=612,63");
        }
        broker9: broker {
            @display("p=565,344");
        }
        broker10: broker {
            @display("p=612,259");
        }
        broker11: broker {
            @display("p=209,344");
        }
        broker12: broker {
            @display("p=178,239");
        }
        client: client {
            @display("p=37,36");
        }
        client1: client {
            @display("p=693,336");
        }
        client2: client {
            @display("p=704,36");
        }
        client3: client {
            @display("p=68,318");
        }
        
        connections allowunconnected:
            
        broker.gate++ <--> ChannelBroker <--> broker1.gate++;
        broker.gate++ <--> ChannelBroker <--> broker2.gate++;
        broker.gate++ <--> ChannelBroker <--> broker3.gate++;
        broker.gate++ <--> ChannelBroker <--> broker4.gate++;
        broker1.gate++ <--> ChannelBroker <--> broker6.gate++;
        broker1.gate++ <--> ChannelBroker <--> broker5.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker7.gate++;
        broker2.gate++ <--> ChannelBroker <--> broker8.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker12.gate++;
        broker3.gate++ <--> ChannelBroker <--> broker11.gate++;
        broker4.gate++ <--> ChannelBroker <--> broker9.gate++;
        broker4.gate++ <--> ChannelBroker <--> broker10.gate++;

        broker5.gate++ <--> ChannelClient <--> client.gate++;
        broker11.gate++ <--> ChannelClient <--> client3.gate++;
        broker10.gate++ <--> ChannelClient <--> client1.gate++;
        broker8.gate++ <--> ChannelClient <--> client2.gate++;
}
