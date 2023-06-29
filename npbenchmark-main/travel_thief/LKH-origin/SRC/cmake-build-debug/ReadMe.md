# TTP solver

## Introduction

This is a simple alarm clock built with Python using the tkinter library for the graphical user interface and the winsound library for playing sound alerts.

## Usage

On windows platform, change directory to the exe file path. Then run command: LKH.exe argument1. The argument 1 is path of instance file, for example C:\wamp64\www\npbenchmark\npbenchmark-main\travel_thief\LKH-origin\SRC\data\a280_n279_bounded-strongly-corr_01.ttp

On Linux platform, change directory to path of LKH file. The run command: ./LKH argument1.  The argument 1 is path of instance file, for example ../data/a280_n279_bounded-strongly-corr_01.ttp

## Building

On windows platform, build in IDE according to CMakeLists.txt.

On Linux platform, run command:  cmake . ; make 

## Requirements

This program requires c++ version >= 17 and C version >= 11.