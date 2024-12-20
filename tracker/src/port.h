#ifndef PORT_FINDER_H
#define PORT_FINDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int find_free_port();

void write_assigned_port_to_config();

#endif /* PORT_FINDER_H */
