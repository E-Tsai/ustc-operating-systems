# USTC OS Course Project

*This is the tutorial for USTC Operating Systems course project, Spring 2019.* [gitbook version](https://e-tsai.gitbook.io/ustc-os-lab).

## Introduction

This repository contains introduction and minor improvements for experiments of [USTC](https://en.wikipedia.org/wiki/University_of_Science_and_Technology_of_China) **Operating System Principles And Implementation** 2019 Spring, lectured by Prof. [Xianglan Chen](http://staff.ustc.edu.cn/~xlanchen/). Please check out our [home page](http://staff.ustc.edu.cn/~xlanchen/OperatingSystemConcepts2019Spring/OperatingSystem2019Spring.htm) for more information. [Chinese translation.](https://e-tsai.gitbook.io/ustc-os-lab/introduction-to-os-lab-cn)

There are 4 experiments in total. Participants are expected to gain basic understandings of operating system concepts through the process.

Any suggestions/contribution (including typo fixing) to this project are deeply appreciated. Feel free to [ask questions](http://www.catb.org/~esr/faqs/smart-questions.html) in our group chat or/and contact teaching assistants. 

Welcome to the world of operating systems! Have fun exploring! Please remember: google is always your friend :)

## **Environment**

Participants are expected to complete the following experiments on a Linux operating system, either installed on bare metal or a virtual machine.

For beginners, we recommend using Linux distributions: debian and ubuntu (>= 14.04). Any other stable Linux distributions are suitable for our experiments. Note that when using some legacy releases, you might encounter tool chain compatibility issues following this tutorial.

We'll be using QEMU as the emulator for our simple operating system, and GCC to compile our source code. Please find detailed instruction in lab 1.

## **Outline**

- [**Lab 1**](lab1-preparation/README.md): Warm-up. Compile a Linux kernel and create a root file system. Create boot disk with GRUB to run on QEMU.  
- [**Lab 2**](https://e-tsai.gitbook.io/ustc-os-lab/lab-2-a-simple-task-manager): Starting from boot loader, enter protected mode and realize a simple OS with a FCFS scheduler. 
- [**Lab 3**](https://e-tsai.gitbook.io/ustc-os-lab/lab-3-priority-rr-mlq-scheduler): Realize a Shortest Job First/Priority/Round Robin/Multilevel (Feedback) Queue scheduler based on lab 2. 
- [**Lab 4**](https://e-tsai.gitbook.io/ustc-os-lab/lab-4-memory-management): Memory management. 