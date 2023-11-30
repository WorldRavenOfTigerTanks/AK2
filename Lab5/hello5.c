
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct list_item {
	struct list_head next;
	ktime_t start_time;
	ktime_t finish_time;
};

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(head);

static uint repeat = 1;
module_param(repeat, uint, 0444);
MODULE_PARM_DESC(repeat, "Counter (repeat)");

static int __init hello_init(void)
{
	uint i = 0;
    	struct list_item *tail;

    	pr_info("Count: %d\n", repeat);

    	BUG_ON(repeat == 0); // Використовуйте BUG_ON для викидання помилки, якщо repeat = 0

    	if (repeat >= 5 && repeat <= 10)
        	pr_warn("The parameter is %d, between 5 and 10, enter number less than 5", repeat);

    

	for (i = 0; i < repeat; i++) {
		tail = kmalloc(sizeof(struct list_item), GFP_KERNEL);

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
		struct list_item *md, *tmp;

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
	struct list_item *md, *tmp;

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