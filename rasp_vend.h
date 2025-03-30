#ifndef RASP_VEND_H
#define RASP_VEND_H

#include <ptread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void *request_i2c(void *period);
void *run_step_mot(void *arg);
void *run_col_mot(void *arg);
void *run_prog(void *arg);

#endif

