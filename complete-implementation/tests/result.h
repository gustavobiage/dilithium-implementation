#include <display.h>
#include <assert.h>

void assert_value(const char * test, int result) {
    tcc::display_status_header(test);
    if (!result) {
        tcc::display_status_result("OK!"); tcc::new_line();
    } else {
        tcc::display_status_result("ERROR!"); tcc::new_line();
    }
}