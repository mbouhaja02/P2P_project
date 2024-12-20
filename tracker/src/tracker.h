#ifndef TRACKER_H
#define TRACKER_H

#include "connection_handler.h"
#include "port.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>




void startTracker();

#endif
