# USTC OS lab Spring 2019

*This is the tutorial for USTC Operating Systems lab, Spring 2019*. gitbook version.

## Introduction

This repo contains introduction and minor improvements for experiments of [USTC](https://en.wikipedia.org/wiki/University_of_Science_and_Technology_of_China) **Operating System Principles And Implementation** 2019 Spring, lectured by Prof. [Xianglan Chen](http://staff.ustc.edu.cn/~xlanchen/). Please check out our [home page](http://staff.ustc.edu.cn/~xlanchen/OperatingSystemConcepts2019Spring/OperatingSystem2019Spring.htm) for more information.

There are 4 experiments in total. Participants are expected to gain basic understands of operating system concepts through the process.

Any suggestions/contribution (including typo fixing) to this project are deeply appreciated. Feel free to ask questions in our group chat or/and contact teaching assistants. 

Welcome to the world of operating systems! Have fun exploring! Please remember: google is always your friend :)

## **Environment**

Participants are expected to complete the following experiments on a Linux operating system, either on bare metal or in a virtual machine.

For beginners, we recommend using the latest stable Ubuntu (18.04 in 2019). Any other stable Linux distributions are suitable for our experiments. Note that when using some legacy releases, you might encounter tool chain compatibility issues following this tutorial.

## **Outline**

- [**Lab 1**](lab1-preparation/README.md): Warm-up. Compile a Linux kernel and create a root file system. Create boot disk with GRUB to run on QEMU. Learn GDB basics.
- [**Lab 2**](lab2-boot2C_64bit/README.md): Starting from boot loader, realize a simple OS with a FCFS scheduler.
- [**Lab 3**/README.md](lab3-interrupt_timer): Realize a Shortest Job First/Priority/Round Robin/Multilevel Queue scheduler based on lab 2.