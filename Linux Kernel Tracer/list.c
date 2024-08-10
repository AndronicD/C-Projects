// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * TODO 1/0: Fill in name / email
 * Author: Andronic Dragos Sebastian dragos.andronic@stud.acs.upb.ro
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

#define PROCFS_MAX_SIZE		512
#define MAX_DATA_SIZE	20
#define MAX_COMMAND		5

#define procfs_dir_name		"list"
#define procfs_file_read	"preview"
#define procfs_file_write	"management"

struct proc_dir_entry *proc_list;
struct proc_dir_entry *proc_list_read;
struct proc_dir_entry *proc_list_write;

/* TODO 2: define your list! */
typedef struct list_node
{
	char data[MAX_DATA_SIZE];
	struct list_head next;
} list_node;

static struct list_head head;

static int list_proc_show(struct seq_file *m, void *v)
{
	/* TODO 3: print your list. One element / line. */
	list_node *current_node;
    
    list_for_each_entry(current_node, &head, next) {
        seq_printf(m, "%s\n", current_node->data);
    }

	return 0;
}

static int list_read_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static ssize_t list_write(struct file *file, const char __user *buffer,
			  size_t count, loff_t *offs)
{
	char local_buffer[PROCFS_MAX_SIZE];
	unsigned long local_buffer_size = 0;
	char command[MAX_COMMAND];
    char data[MAX_DATA_SIZE];
    list_node *node;

	local_buffer_size = count;
	if (local_buffer_size > PROCFS_MAX_SIZE)
		local_buffer_size = PROCFS_MAX_SIZE;

	memset(local_buffer, 0, PROCFS_MAX_SIZE);
	if (copy_from_user(local_buffer, buffer, local_buffer_size))
		return -EFAULT;

	/* local_buffer contains your command written in /proc/list/management
	 * TODO 4/0: parse the command and add/delete elements.
	 */
	node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node)
        return -ENOMEM;

	sscanf(local_buffer, "%s %s", command, data);
	strcpy(node->data, data);

	INIT_LIST_HEAD(&node->next);

	switch (command[0]) {
		case 'a':
			if (strcmp(command, "addf") == 0)
				list_add(&node->next, &head);
			else if (strcmp(command, "adde") == 0)
				list_add_tail(&node->next, &head);
			break;

		case 'd':
			if (strcmp(command, "delf") == 0 || strcmp(command, "dela") == 0) {
				list_node *pos, *temp;
				list_for_each_entry_safe(pos, temp, &head, next) {
					if (strcmp(pos->data, data) == 0) {
						list_del(&pos->next);
						kfree(pos);
						if (strcmp(command, "delf") == 0)
							break;
					}
				}
			}
			break;

		default:
			kfree(node);
			return -EINVAL;
		}

	return local_buffer_size;
}

static const struct proc_ops r_pops = {
	.proc_open		= list_read_open,
	.proc_read		= seq_read,
	.proc_release	= single_release,
};

static const struct proc_ops w_pops = {
	.proc_open		= list_write_open,
	.proc_write		= list_write,
	.proc_release	= single_release,
};

static int list_init(void)
{
	INIT_LIST_HEAD(&head);

	proc_list = proc_mkdir(procfs_dir_name, NULL);
	if (!proc_list)
		return -ENOMEM;

	proc_list_read = proc_create(procfs_file_read, 0000, proc_list,
				     &r_pops);
	if (!proc_list_read)
		goto proc_list_cleanup;

	proc_list_write = proc_create(procfs_file_write, 0000, proc_list,
				      &w_pops);
	if (!proc_list_write)
		goto proc_list_read_cleanup;

	return 0;

proc_list_read_cleanup:
	proc_remove(proc_list_read);
proc_list_cleanup:
	proc_remove(proc_list);
	return -ENOMEM;
}

static void list_exit(void)
{
	proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);

MODULE_DESCRIPTION("Linux kernel list API");
/* TODO 5: Fill in your name / email address */
MODULE_AUTHOR("Andronic Dragos Sebastian dragos.andronic@stud.acs.upb.ro");
MODULE_LICENSE("GPL v2");
