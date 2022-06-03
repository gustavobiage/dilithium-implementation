#ifndef __display_
#define __display_

#include <stdio.h>
#include <string.h>

#define MAX_WIDTH 60

namespace tcc {
	void print_hex_string(const unsigned char *, int);

	void print_integer_in_binary(const unsigned char *, int);

	void new_line();

	template <typename ...Ts>
	void display_box(const char * title, Ts... a);

	void display_status_header(const char * header);

	void display_status_result(const char * result);

}

#include <display.cc>

#endif