# ipstack
A minimal IP stack suited for embedded systems. The stack uses SLIP as a data link layer. A demo application is included that can be run as a user space application without relying on external hardware.

## Contents
- [Unit Testing](#unit-testing)
- [Demo Application](#demo-application)

## Unit Testing
Unit testing is taken care of courtesy of [Unity](http://www.throwtheswitch.org/unity/). To run the tests you need to download and extract Unity and set the path to Unity's `src` directory in the ipstack's Makefile. 
```
UNITYDIR = ../Unity/src
```
To run the tests at the command line simply enter:
```
$ make test
```

## Demo Application
Ipstack comes with a demo application that can be run on the host machine. This is the default target for the makefile. Helper scripts are used to setup the environment so that the IP stack in the demo application can interact with the kernel space IP stack. 

The helper scripts use [socat](https://linux.die.net/man/1/socat) to setup a psuedo terminal (/dev/pty/...) with the demo application attached to one end. Explicit file descriptors are used in the demo application to receive traffic from the pty so that stdout can still be used for monitoring the application. The scripts then configure use [slattach](https://linux.die.net/man/8/slattach) to enable the SLIP driver on the the psuedo terminal. Finally ifconfig is used to setup on the interface. 

On a debian based system slattach and ifconfig can be installed from the net-tools package.
```
$ sudo apt install net-tools
```

In order to start the demo:
```
$ sudo ./start-demo
```
It is then possible to interact with the stack. For example:
```
$ ping 192.168.0.199
```
To shut down the demo application:
```
$ sudo ./stop-demo
```
The script using socat forces data to be sent and received from the demo application using STDIN and STDOUT as such status messages are written to a file, log.txt. This file can be watched whilst the demo is running as follows:
```
$ tail -f log.txt
```
