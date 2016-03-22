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
        //    input ic3;
        //    input ic4;
        //Broker
        input ib1;
     //   input ib2;
       // input ib3;
        //input ib4;


        //Client
        output oc1;
        output oc2;
        //    output oc3;
        //    output oc4;
        //Broker
        output ob1;
     //   output ob2;
      //  output ob3;
       // output ob4;

}

//
// TODO documentation
//
network Network
{
    @display("bgb=1038,467");
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
    connections:
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
}

