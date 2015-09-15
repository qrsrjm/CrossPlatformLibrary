#ifndef REMOTE_LIB_H
#define REMOTE_LIB_H

#include "remote/zmq_server.h"

#ifdef WIN32
#pragma comment(lib,"remote.lib") 
#pragma message("Automatically linking with remote.lib") 
#endif

#endif