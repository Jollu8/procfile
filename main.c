//
// Created by Jollu Emil on 19.03.2023.
//

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROCFS_MAX_SIZE 1024

static struct proc_dir_entry* proc_file_entry;
static char procfs_buffer[PROCFS_MAX_SIZE];
static int procfs_buffer_size = 0;

// Функция обработки записи в procfs файл
static ssize_t proc_file_write(struct file* file, const char __user* buffer, size_t count, loff_t* data) {
if (count > PROCFS_MAX_SIZE) {
count = PROCFS_MAX_SIZE;
}
if (copy_from_user(procfs_buffer, buffer, count)) {
return -EFAULT;
}
procfs_buffer_size = count;
return procfs_buffer_size;
}

// Функция обработки чтения из procfs файла
static ssize_t proc_file_read(struct file* file, char __user* buffer, size_t count, loff_t* data) {
if (*data > 0 || count < procfs_buffer_size) {
return 0;
}
if (copy_to_user(buffer, procfs_buffer, procfs_buffer_size)) {
return -EFAULT;
}
*data = procfs_buffer_size;
return procfs_buffer_size;
}

static const struct file_operations proc_file_fops = {
        .owner = THIS_MODULE,
        .write = proc_file_write,
        .read = proc_file_read,
};

static int __init test_module_init(void) {
    proc_file_entry = proc_create("test_task", 0, NULL, &proc_file_fops);
    if (proc_file_entry == NULL) {
        printk(KERN_ALERT "Error: Could not initialize /proc/test_task\n");
        return -ENOMEM;
    } else {
        printk(KERN_INFO "Module test_task created successfully\n");
    }
    return 0;
}

static void __exit test_module_cleanup(void) {
    proc_remove(proc_file_entry);
    printk(KERN_INFO "Module test_task removed successfully\n");
}

module_init(test_module_init);
module_exit(test_module_cleanup);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jollu8");