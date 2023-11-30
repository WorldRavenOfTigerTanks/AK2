#include <linux/types.h>

struct hello_data {
    struct list_head tlist;
    ktime_t before_time;
    ktime_t after_time;
};

void print_hello(void);