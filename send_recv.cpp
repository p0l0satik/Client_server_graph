#include "send_recv.h"

int send_arr(int sockfd, int *arr, int len, int flags) {
    int alr_sent = 0;
    int n = 0;
    while(alr_sent < len / sizeof(int)) {
        n = send(sockfd, (arr + alr_sent), len - alr_sent * sizeof(int), flags);
        if (n == -1) {
            std::cout << "error while sending" << std::endl;
            return -1;
        }
        alr_sent += n / sizeof(int);
    }
    return alr_sent;
}

int send_int(int sockfd, int snd, int flags) {
    int bytes = 0;
    while (bytes < sizeof(snd)) {
        bytes = send(sockfd, &snd, sizeof(snd), flags);
        if (bytes == -1) {
            std::cout << "error while sending" << std::endl;
            return -1;
        }
    }
    return bytes;
}

int recv_arr(int sockfd, int *arr, int len, int flags) {
    int alr_recv = 0;
    int n = 0;
    while(alr_recv < len / sizeof(int)) {
        
        n = recv(sockfd, (arr + alr_recv), len - alr_recv * sizeof(int), flags);
        if (n == -1) {
            std::cout << "error while receiving" << std::endl;
            return -1;
        }
        alr_recv += n / sizeof(int);
    }
    return alr_recv;
}

int recv_int(int sockfd, int &snd, int flags) {
    int bytes = 0;
    while (bytes < sizeof(snd)) {
        bytes = recv(sockfd, &snd, sizeof(snd), flags);
        if (bytes == -1) {
            std::cout << "error while sending" << std::endl;
            return -1;
        }
    }
    return bytes;
}