/* Included on header file */

void tcc::print_hex_string(const unsigned char * content, int content_size) {
    if (content_size % 4 != 0) {
        printf(" (contains hidden bytes) ");
    }
    // TODO make generic (not multiple of 4)
    for  (int i = 0; i < content_size; i += 4) {
        printf("%02X:%02X:%02X:%02X", content[0], content[1], content[2], content[3]);
    }
}

void tcc::new_line() {
    printf("\n");
}

void display_box_content(const char * content) {
    char _print[MAX_WIDTH + 1];
    _print[0] = '\0';
    const char * pre = "| ";
    const char * post = "|";
 
    int w = MAX_WIDTH - strlen(content) - strlen(pre) - strlen(post);

    strcat(_print, pre);
    strcat(_print, content);
    while (w--) {
        strcat(_print, " ");
    }
    strcat(_print, post);
    printf("%s\n", _print);
}

void display_box_border() {
    char _print[MAX_WIDTH + 1];
    _print[0] = '\0';
    strcat(_print, "+");
    int w = MAX_WIDTH - 2;
    while (w--) {
        strcat(_print, "-");
    }
    strcat(_print, "+");
    printf("%s\n", _print);
}

void display_parameter_iterator(int index) { }

void display_parameter_iterator(int index, const char * par, unsigned int value) {
    char content[MAX_WIDTH + 1];
    const char * post = "   #";
    sprintf(content, "%s%d %s = %u", post, index, par, value);
    display_box_content(content);
}

template <typename ...Ts>
void display_parameter_iterator(int index, const char * par, unsigned int value, Ts ... a) {
    char content[MAX_WIDTH + 1];
    const char * post = "   #";
    sprintf(content, "%s%d %s = %u", post, index, par, value);
    display_box_content(content);
    display_parameter_iterator(index + 1, a...);
}

template <typename ...Ts>
void tcc::display_box(const char * title, Ts... a) {
    display_box_border();
    display_box_content(title);
    display_parameter_iterator(1, a...);
    display_box_border();
    tcc::new_line();
}

unsigned int status_id = 1;

void tcc::display_status_header(const char * header) {
    char _print[MAX_WIDTH + 1];
    _print[0] = '\0';

    char content[MAX_WIDTH + 1];
    sprintf(content, "%d. %s", status_id++, header);
    const char * post = " ";

    int w = MAX_WIDTH - strlen(content) - strlen(post);
    strcat(_print, content);
    while (w--) {
        strcat(_print, "_");
    }
    strcat(_print, post);
    printf("%s\n", _print);
}

void tcc::display_status_result(const char * result) {
    char _print[MAX_WIDTH + 1]; 
    _print[0] = '\0';
    const char * ident = "    L";
    const char * pre = " ";
    const char * post = " ";
    int w = MAX_WIDTH - strlen(ident) - strlen(pre) - strlen(post) - strlen(result);
    strcat(_print, ident);
    while (w--) {
        strcat(_print, ".");
    }
    strcat(_print, pre);
    strcat(_print, result);
    printf("%s\n", _print);
}