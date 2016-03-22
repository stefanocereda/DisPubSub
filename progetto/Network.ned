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
//
// TODO documentation
//
simple client
{
    @display("i=old/laptop");
    gates:
        input in;
        output out;
}

//
// TODO documentation
//
simple broker
{
    @display("i=device/server2");
    gates:
        //Client
        input ic1;
        input ic2;
        input ic3;
        input ic4;
        //Broker
        input ib1;
        input ib2;
        input ib3;
        input ib4;


        //Client
        output oc1;
        output oc2;
        output oc3;
        output oc4;
        //Broker
        output ob1;
        output ob2;
        output ob3;
        output ob4;

}

//
// TODO documentation
//
network Network
{
    @display("bgb=1381.9525,586.125");
    types:
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
        client1.out --> broker1.ic1;
        client2.out --> broker1.ic2;
        broker1.oc1 --> client1.in;
        broker1.oc2 --> client2.in;
        broker1.ob1 --> broker2.ib1;
        broker2.ob1 --> broker1.ib1;
        client3.out --> broker2.ic1;
        broker2.oc1 --> client3.in;
        client4.out --> broker2.ic2;
        broker2.oc2 --> client4.in;
        broker2.ob2 --> broker3.ib1;
        broker3.ob1 --> broker2.ib2;
        broker3.ob2 --> broker4.ib1;
        broker4.ob1 --> broker3.ib2;
        broker3.ob3 --> broker5.ib1;
        broker5.ob1 --> broker3.ib3;
        broker3.ob4 --> broker6.ib1;
        broker6.ob1 --> broker3.ib4;
        client5.out --> broker4.ic1;
        broker4.oc1 --> client5.in;
        client6.out --> broker4.ic2;
        broker4.oc2 --> client6.in;
        client7.out --> broker5.ic1;
        broker5.oc1 --> client7.in;
        client8.out --> broker5.ic2;
        broker5.oc2 --> client8.in;
        client9.out --> broker6.ic1;
        broker6.oc1 --> client9.in;
        client10.out --> broker6.ic2;
        broker6.oc2 --> client10.in;
}

