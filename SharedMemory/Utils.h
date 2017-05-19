#ifndef _MY_UTILS
#pragma once
#define _MY_UITLS
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#define TRYCATCH(pred,msg) if(pred) {fprintf(stderr, "ERROR: %s -- %s\n", strerror(errno), msg); exit(1);}
#endif
