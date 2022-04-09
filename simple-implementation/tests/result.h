#include <display.h>
#include <assert.h>

void assert_value(const char * test, int result) {
    display_status_header(test);
    if (!result) {
        display_status_result("OK!"); new_line();
    } else {
        display_status_result("ERROR!"); new_line();
    }
}