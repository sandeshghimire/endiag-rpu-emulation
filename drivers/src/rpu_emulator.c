#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define READ_DATA_SIZE 0x100
#define WRITE_DATA_SIZE 0x100

#define METADATA_OFFSET 0x00
#define APU_MESSAGE_OFFSET 0x100 / 4
#define RPU_MESSAGE_OFFSET 0x200 / 4

unsigned int valid_flag = 0;
unsigned int receive_counter = 0;
unsigned int send_counter = 0;

static dev_t dev_num = 0;
static struct cdev *chardev_cdev = NULL;
static struct class *chardev_class = NULL;
// static unsigned char read_buffer[READ_DATA_SIZE];
// static unsigned char write_buffer[READ_DATA_SIZE];

char *DEVICE_NAME = "rpu_emulator";
#define MAX_SIZE (PAGE_SIZE * 2)
static unsigned int *sh_mem = NULL;

static int rpu_emulator_open(struct inode *, struct file *);
static int rpu_emulator_close(struct inode *, struct file *);
static ssize_t rpu_emulator_read(struct file *, char *, size_t, loff_t *);
static ssize_t rpu_emulator_write(struct file *, const char *, size_t,
                                  loff_t *);
static int mchar_mmap(struct file *filp, struct vm_area_struct *vma);

static int rpu_emulator_open(struct inode *i, struct file *f) {
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}
static int rpu_emulator_close(struct inode *i, struct file *f) {
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static ssize_t rpu_emulator_read(struct file *f, char __user *buf, size_t len,
                                 loff_t *off) {
  // int i = 0;
  printk(KERN_INFO "Driver: read()\n");

  /*
    for (i = 0; i < READ_DATA_SIZE; i++) {
      printk("valid_flag%X %X\n", i, sh_mem[i]);
    }

    memcpy(write_buffer, sh_mem, READ_DATA_SIZE);

    memcpy(&valid_flag, write_buffer, 4);
    printk("valid_flag %X\n", valid_flag);

    memcpy(&receive_counter, write_buffer + 4, 4);
    printk("receive_counter %X\n", receive_counter);

    memcpy(&send_counter, write_buffer + 8, 4);
    printk("send_counter %X\n\n", send_counter);

    valid_flag = 0x01;
    memcpy(read_buffer, &valid_flag, 4);
    printk("valid_flag %X\n", valid_flag);


      memcpy(read_buffer + 4, &receive_counter, 4);
      printk("receive_counter %X\n", receive_counter);

      memcpy(read_buffer + 8, &send_counter, 4);
      printk("send_counter %X\n\n", send_counter);
    */

  (void)copy_to_user(buf, (sh_mem + APU_MESSAGE_OFFSET), READ_DATA_SIZE);
  return READ_DATA_SIZE;
}
static ssize_t rpu_emulator_write(struct file *f, const char __user *buf,
                                  size_t len, loff_t *off) {
  printk(KERN_INFO "Driver: write()\n");
  (void)copy_from_user((sh_mem + RPU_MESSAGE_OFFSET), buf, WRITE_DATA_SIZE);

  /*memcpy(&valid_flag, write_buffer, 4);
  printk("valid_flag %X\n", valid_flag);

  memcpy(&receive_counter, write_buffer + 4, 4);
  printk("receive_counter %X\n", receive_counter);

  memcpy(&send_counter, write_buffer + 8, 4);
  printk("send_counter %X\n\n", send_counter);

  memcpy(sh_mem, write_buffer, WRITE_DATA_SIZE);
*/
  return WRITE_DATA_SIZE;
}

static struct file_operations pugs_fops = {.owner = THIS_MODULE,
                                           .open = rpu_emulator_open,
                                           .release = rpu_emulator_close,
                                           .read = rpu_emulator_read,
                                           .mmap = mchar_mmap,
                                           .write = rpu_emulator_write};

static int __init rpu_emulator_init(void) /* Constructor */
{
  int ret;
  int err = 0;

  printk(KERN_INFO "Rpu_emulator registered \n");
  if ((ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME)) < 0) {
    return ret;
  }
  chardev_class = class_create(THIS_MODULE, DEVICE_NAME);

  // Allocate and initialize the char device
  chardev_cdev = cdev_alloc();
  chardev_cdev->ops = &pugs_fops;
  chardev_cdev->owner = THIS_MODULE;

  // Add the character device to the kernel
  if ((err = cdev_add(chardev_cdev, dev_num, 1)) < 0) {
    printk(KERN_ERR "chardev: cdev_add() error %d\n", err);
    return err;
  }
  device_create(chardev_class, NULL, dev_num, NULL, DEVICE_NAME);

  sh_mem = kmalloc(MAX_SIZE, GFP_KERNEL);
  if (sh_mem == NULL) {
    ret = -ENOMEM;
  }

  return 0;
}

static int mchar_mmap(struct file *filp, struct vm_area_struct *vma) {
  int ret = 0;
  struct page *page = NULL;
  unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

  if (size > MAX_SIZE) {
    ret = -EINVAL;
    goto out;
  }
  printk(KERN_ERR "chardev: mapping size %lu\n", size);

  page = virt_to_page((unsigned long)sh_mem + (vma->vm_pgoff << PAGE_SHIFT));
  ret = remap_pfn_range(vma, vma->vm_start, page_to_pfn(page), size,
                        vma->vm_page_prot);
  if (ret != 0) {
    goto out;
  }

out:
  return ret;
}

static void __exit rpu_emulator_exit(void) {
  device_destroy(chardev_class, dev_num);
  cdev_del(chardev_cdev);
  class_destroy(chardev_class);
  unregister_chrdev_region(dev_num, 1);
  printk(KERN_INFO "Rpu_emulator unregistered\n");
}

module_init(rpu_emulator_init);
module_exit(rpu_emulator_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sandesh Ghimire <sandesh@soccentric.com>");
MODULE_DESCRIPTION("RPU Emulation Driver");