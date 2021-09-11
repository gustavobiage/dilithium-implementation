#ifndef __display_
#define __display_

#include <stdio.h>
#include <string.h>

#define MAX_WIDTH 60

void new_line();

template <typename ...Ts>
void display_box(const char * title, Ts... a);

void display_status_header(const char * header);

void display_status_result(const char * result);

#include <display.cc>

#endif