# ipstack
A minimal IP stack

## Contents
- [Unit Testing](#unit-testing)
- [Running](#running)

## Unit Testing
Unit testing is taken care of courtesy of [Unity](http://www.throwtheswitch.org/unity/). To run the tests you need to download and extract Unity and set the path to Unity's `src` directory in the ipstack's Makefile. 
```
UNITYDIR = ../Unity/src
```
To run the tests at the command line simply enter:
```
$ make test
```
The results for individual tests are written to test files in ipstack's root directory.

## Running
To run the application at the command line:
```
$ ./start
```
To monitor the application:
```
$ tail -f log.txt
```
It is then possible to interact with the stack. For example:
```
$ ping 192.168.0.199
```
To shut the stack down:
```
$ ./stop
```
