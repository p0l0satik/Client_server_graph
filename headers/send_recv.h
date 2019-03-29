#include <sys/socket.h>
#include <iostream>

#ifndef SEND_RECV_H_
#define SEND_RECV_H_

int send_arr(int sockfd, int *arr, int len, int flags);
int send_int(int sockfd, int snd, int flags);
int recv_arr(int sockfd, int *arr, int len, int flags);
int recv_int(int sockfd, int &snd, int flags);

#endif