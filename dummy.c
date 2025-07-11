/*
 * Copyright (c) 2025 Paulus Gandung Prakosa <rvn.plvhx@gmail.com>
 */

#include <linux/compiler_types.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_NAME "dummy"

static struct proc_dir_entry *pfs;

static ssize_t pfops_read(struct file *filp, char __user *ubuf,
                          size_t ulen, loff_t *poff)
{
    char *buf = "Hello, World!\n";
    size_t blen = strlen(buf);
    ssize_t ret = blen;

    if (*poff >= blen || copy_to_user(ubuf, buf, blen)) {
        pr_info("(pfops_read): copy_to_user failed.\n");
        ret = 0;
    } else {
        pr_info("(pfops_read): name: %s, bytes copied: %ld\n",
            filp->f_path.dentry->d_name.name,
            ret
        );

        *poff += blen;
    }

    return ret;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops pfops = {
    .proc_read = pfops_read
};
#else
static const struct file_operations pfops = {
    .read = pfops_read
};
#endif

static int __init foo_ctor(void)
{
    pfs = proc_create(PROCFS_NAME, 0644, NULL, &pfops);

    if (!pfs) {
        pr_alert("(foo_ctor): Failed to create /proc/%s.\n", PROCFS_NAME);
        return -ENOMEM;
    }

    pr_info("(foo_ctor): /proc/%s created.\n", PROCFS_NAME);
    return 0;
}

static void __exit foo_dtor(void)
{
    proc_remove(pfs);
    pr_info("(foo_dtor): /proc/%s removed.\n", PROCFS_NAME);
    return;
}

module_init(foo_ctor);
module_exit(foo_dtor);

MODULE_AUTHOR("Paulus Gandung Prakosa <rvn.plvhx@gmail.com>");
MODULE_DESCRIPTION("Dummy procfs that shows 'Hello, World!' with a newline");
MODULE_LICENSE("GPL");
