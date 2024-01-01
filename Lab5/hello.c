#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct message_time {
	struct list_head next;
	ktime_t start_time;
	ktime_t finish_time;
};

MODULE_AUTHOR("Kuravskii Olexii IO-14");
MODULE_DESCRIPTION("Module can repeat \"Hello, world\" x n");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(head);

static uint repeat = 1;
module_param(repeat, uint, 0444);
MODULE_PARM_DESC(repeat, "How many times should I repeat?");

static int __init hello_init(void)
{
	uint i = 0;
    	struct message_time *tail;

    	pr_info("Count: %d\n", repeat);

    	BUG_ON(repeat == 0); // Використовуйте BUG_ON для викидання помилки, якщо repeat = 0

    	if (repeat >= 5 && repeat <= 10)
        	pr_warn("The parameter is %d, between 5 and 10, enter number less than 5", repeat);

    

	for (i = 0; i < repeat; i++) {
		tail = kmalloc(sizeof(struct message_time), GFP_KERNEL);

		if (i == 5)
			tail = 0;

		if (ZERO_OR_NULL_PTR(tail))
			goto exception;

		tail->start_time = ktime_get();
		pr_info("Hello, World!\n");
		tail->finish_time = ktime_get();

		list_add_tail(&(tail->next), &head);
	}

	return 0;

exception:
	{
		struct message_time *md, *tmp;

		pr_err("The end of memory...\n");

		list_for_each_entry_safe(md, tmp, &head, next) {
			list_del(&md->next);
			kfree(md);
		}

		BUG();
		return -ENOMEM;
	}
}


static void __exit hello_exit(void)
{
	struct message_time *md, *tmp;

	list_for_each_entry_safe(md, tmp, &head, next) {
		pr_info("Time: %lld",
		md->finish_time - md->start_time);

		list_del(&md->next);
		kfree(md);
	}

	BUG_ON(!list_empty(&head));
}

module_init(hello_init);
module_exit(hello_exit);
