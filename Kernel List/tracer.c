// SPDX-License-Identifier: GPL-2.0+

/*
 * tracer.c - Kprobe based tracer
 *
 * Author: Andronic Dragos Sebastian dragos.andronic@stud.acs.upb.ro
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include "tracer.h"

#define MAX_ACTIVE 64
#define PROC_TRACER_FILE "tracer"

// Spinlock
DEFINE_SPINLOCK(process_list_spinlock);

// Add memory entry
struct address_size {
    long address;
    long size;
    struct list_head next_node;
};

// List to keep track of processes
static struct list_head process_list;

typedef struct process_node {
    int pid;
    atomic_t number_of_kmalloc;
    atomic_t number_of_kfree;
    atomic_t size_of_kmalloc;
    atomic_t size_of_kfree;
    atomic_t number_of_schedule;
    atomic_t number_of_up;
    atomic_t number_of_down;
    atomic_t number_of_lock;
    atomic_t number_of_unlock;
    struct list_head memory_list;
    struct list_head next_process_node;
} process_node;

struct data{
    int size;
};

static void add_address_size_entry(long address, long size){
    struct address_size *memory_node;
	struct process_node *process_node;

	memory_node = kmalloc(sizeof(*memory_node), GFP_ATOMIC);
	memory_node->address = address;
	memory_node->size = size;

    list_for_each_entry(process_node, &process_list, next_process_node) {
        if (process_node->pid != current->pid)
            continue;
        list_add(&memory_node->next_node, &process_node->memory_list);
        break;
    }
}

static void free_address_size_list(struct list_head *list){
	struct list_head *pos, *n;
	struct address_size *memory_node;

	list_for_each_safe(pos, n, list) {
		memory_node = list_entry(pos, struct address_size, next_node);
		list_del(&memory_node->next_node);
		kfree(memory_node);
	}
}

static void free_process_list(struct list_head *list) {
    struct list_head *pos, *n;
    struct process_node *process_node;

    list_for_each_safe(pos, n, list) {
        process_node = list_entry(pos, struct process_node, next_process_node);
        free_address_size_list(&process_node->memory_list);
        list_del(&process_node->next_process_node);
        kfree(process_node);
    }
}

static void add_process_node(int pid) {
    struct process_node *new_node;

    new_node = kmalloc(sizeof(*new_node), GFP_ATOMIC);

    atomic_set(&new_node->number_of_kmalloc, 0);
    atomic_set(&new_node->number_of_kfree, 0);
    atomic_set(&new_node->size_of_kmalloc, 0);
    atomic_set(&new_node->size_of_kfree, 0);
    atomic_set(&new_node->number_of_schedule, 0);
    atomic_set(&new_node->number_of_up, 0);
    atomic_set(&new_node->number_of_down, 0);
    atomic_set(&new_node->number_of_lock, 0);
    atomic_set(&new_node->number_of_unlock, 0);

    new_node->pid = pid;
    INIT_LIST_HEAD(&new_node->memory_list);

    spin_lock(&process_list_spinlock);
    list_add(&new_node->next_process_node, &process_list);
    spin_unlock(&process_list_spinlock);
}

static void remove_process_node(int pid) {
    struct list_head *pos, *n;
    struct process_node *process_node;

    spin_lock(&process_list_spinlock);
    list_for_each_safe(pos, n, &process_list) {
        process_node = list_entry(pos, struct process_node, next_process_node);
        if (process_node->pid == pid) {
            free_address_size_list(&process_node->memory_list);
            list_del(pos);
            spin_unlock(&process_list_spinlock);
            kfree(process_node);
            return;
        }
    }
    spin_unlock(&process_list_spinlock);
}

// Miscdevice declaration

static int tracer_open(struct inode *inode, struct file *file){
    return 0;
}

static int tracer_release(struct inode *inode, struct file *file){
    return 0;
}

static long tracer_ioctl(struct file *file, unsigned int cmd, unsigned long pid){
    switch(cmd) {
        case TRACER_ADD_PROCESS:
                add_process_node(pid);
                break;
        case TRACER_REMOVE_PROCESS:
                remove_process_node(pid);
                break;
        default:
                break;
    }
    return 0;
}

static struct file_operations tracer_fops = {
	.open = tracer_open,
	.release = tracer_release,
    .unlocked_ioctl = tracer_ioctl,
};

static struct miscdevice tracer_device = {
	.minor  = TRACER_DEV_MINOR,
	.name   = TRACER_DEV_NAME,
	.fops   = &tracer_fops,
};

// /proc/tracer file definitions

static int tracer_print(struct seq_file *m, void *v) {
    struct process_node *process;
    seq_puts(m, "PID   kmalloc  kfree  kmalloc_mem  kfree_mem sched  up   down  lock  unlock\n");

    spin_lock(&process_list_spinlock);
    list_for_each_entry(process, &process_list, next_process_node) {

        seq_printf(m, "%d ", process->pid);
		seq_printf(m, "%d ", atomic_read(&process->number_of_kmalloc));
		seq_printf(m, "%d ", atomic_read(&process->number_of_kfree));
		seq_printf(m, "%d ", atomic_read(&process->size_of_kmalloc));
		seq_printf(m, "%d ", atomic_read(&process->size_of_kfree));
		seq_printf(m, "%d ", atomic_read(&process->number_of_schedule));
		seq_printf(m, "%d ", atomic_read(&process->number_of_up));
		seq_printf(m, "%d ", atomic_read(&process->number_of_down));
		seq_printf(m, "%d ", atomic_read(&process->number_of_lock));
		seq_printf(m, "%d ", atomic_read(&process->number_of_unlock));
		seq_puts(m, "\n");
    }
    spin_unlock(&process_list_spinlock);

    return 0;
}

static int tracer_read_open(struct inode *inode, struct file *file) {
    return single_open(file, tracer_print, NULL);
}

static struct proc_dir_entry *proc_list_read;

static const struct proc_ops r_pops = {
	.proc_open		= tracer_read_open,
	.proc_read		= seq_read,
	.proc_release	= single_release,
};

// Increase contors

static void increase_kmalloc(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_kmalloc);
            break;
        }
    }
}

static void increase_kfree(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_kfree);
            break;
        }
    }
    return;
}

static void increase_schedule(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_schedule);
            break;
        }
    }
}

static void increase_up(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_up);
            break;
        }
    }
}

static void increase_down(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_down);
            break;
        }
    }
}

static void increase_lock(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_lock);
            break;
        }
    }
}

static void increase_unlock(int pid)
{
	struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_inc(&node->number_of_unlock);
            break;
        }
    }
}

static void increase_kmalloc_size(int pid, long size){
    struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_add(size, &node->size_of_kmalloc);
            break;
        }
    }
}

static void increase_kfree_size(int pid, long size){
    struct process_node *node;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == pid) {
            atomic_add(size, &node->size_of_kfree);
            break;
        }
    }
}

// Define entry handlers

static int kmalloc_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct data *data;

	increase_kmalloc(ri->task->pid);

	data = (struct data *)ri->data;
	data->size = regs->ax;
	return 0;
}

static int kmalloc_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct data *data;
    long address;

    data = (struct data*)ri->data;
    address = regs_return_value(regs);
	increase_kmalloc_size(ri->task->pid, data->size);

    add_address_size_entry(address, data->size);
	return 0;
}

static int kfree_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs){
    struct process_node *node;
    struct address_size *memory;
    long address;
    long size;

    address = regs->ax;

    list_for_each_entry(node, &process_list, next_process_node) {
        if (node->pid == ri->task->pid) {
            list_for_each_entry(memory, &node->memory_list, next_node) {
                if (memory->address == address) {
                    size = memory->size;
                    list_del(&memory->next_node);
                    kfree(memory);
                    break;
                }
            }
            break;
        }
    }
    
    increase_kfree(ri->task->pid);
    increase_kfree_size(ri->task->pid, size);
    return 0;
}

static int schedule_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs){
    increase_schedule(ri->task->pid);
    return 0;
}

static int up_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs){
    increase_up(ri->task->pid);
    return 0;
}

static int down_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs){
    increase_down(ri->task->pid);
    return 0;
}

static int mutex_lock_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs){
    increase_lock(ri->task->pid);
    return 0;
}

static int mutex_unlock_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs){
    increase_unlock(ri->task->pid);
    return 0;
}

// Kprobes declaration

static struct kretprobe kmalloc_probe = {
		.kp = {
			.symbol_name = "__kmalloc"
		},
		.entry_handler	= kmalloc_entry_handler,
		.handler	= kmalloc_handler,
		.maxactive	= MAX_ACTIVE,
};

static struct kretprobe kfree_probe = {
		.kp = {
			.symbol_name = "kfree"
		},
		.entry_handler	= kfree_entry_handler,
		.maxactive	= MAX_ACTIVE,
};

static struct kretprobe schedule_probe = {
		.kp = {
			.symbol_name = "schedule"
		},
		.entry_handler	= schedule_entry_handler,
		.maxactive	= MAX_ACTIVE,
};

static struct kretprobe up_probe = {
		.kp = {
			.symbol_name = "up"
		},
		.entry_handler	= up_entry_handler,
		.maxactive	= MAX_ACTIVE,
};

static struct kretprobe down_probe = {
		.kp = {
			.symbol_name = "down_interruptible"
		},
		.entry_handler	= down_entry_handler,
		.maxactive	= MAX_ACTIVE,
};

static struct kretprobe mutex_lock_probe = {
		.kp = {
			.symbol_name = "mutex_lock_nested"
		},
		.entry_handler	= mutex_lock_entry_handler,
		.maxactive	= MAX_ACTIVE,
};

static struct kretprobe mutex_unlock_probe = {
		.kp = {
			.symbol_name = "mutex_unlock"
		},
		.entry_handler	= mutex_unlock_entry_handler,
		.maxactive	= MAX_ACTIVE,
};

static int tracer_init(void){
    INIT_LIST_HEAD(&process_list);
    int ret = 0;

    ret = register_kretprobe(&kmalloc_probe);
	if (ret) {
		pr_err("Unable to register kmalloc_probe\n");
        return ret;
	}

    ret = register_kretprobe(&kfree_probe);
	if (ret) {
		pr_err("Unable to register kfree_probe\n");
        return ret;
	}

    ret = register_kretprobe(&schedule_probe);
	if (ret) {
		pr_err("Unable to register schedule_probe\n");
        return ret;
	}

    ret = register_kretprobe(&up_probe);
	if (ret) {
		pr_err("Unable to register up_probe\n");
        return ret;
	}

    ret = register_kretprobe(&down_probe);
	if (ret) {
		pr_err("Unable to register down_probe\n");
        return ret;
	}

    ret = register_kretprobe(&mutex_lock_probe);
	if (ret) {
		pr_err("Unable to register mutex_lock_probe\n");
        return ret;
	}

    ret = register_kretprobe(&mutex_unlock_probe);
	if (ret) {
		pr_err("Unable to register mutex_unlock_probe\n");
        return ret;
	}

    proc_list_read = proc_create(PROC_TRACER_FILE, 0000, NULL,
				     &r_pops);
	if (!proc_list_read){
        ret = -EINVAL;
		proc_remove(proc_list_read);
        return ret;
    }


    if(misc_register(&tracer_device)){
        ret = -EINVAL;
        misc_deregister(&tracer_device);
        return ret;
    }

    return ret;
}

static void tracer_exit(void){

    unregister_kretprobe(&kmalloc_probe);
    unregister_kretprobe(&kfree_probe);
    unregister_kretprobe(&schedule_probe);
    unregister_kretprobe(&up_probe);
    unregister_kretprobe(&down_probe);
    unregister_kretprobe(&mutex_lock_probe);
    unregister_kretprobe(&mutex_unlock_probe);

    free_process_list(&process_list);

    misc_deregister(&tracer_device);

    proc_remove(proc_list_read);
}

module_init(tracer_init);
module_exit(tracer_exit);

MODULE_DESCRIPTION("Kprobe based tracer");
MODULE_AUTHOR("Andronic Dragos Sebastian dragos.andronic@stud.acs.upb.ro");
MODULE_LICENSE("GPL v2");