### Description
Dummy procfs that shows 'Hello, World!' with a newline.

### Build

```
$ git clone https://github.com/pgandung/dummy-procfs.git
$ cd dummy-procfs
$ make
$ sudo insmod dummy.ko
```

### Verify that module is installed and /proc/dummy exists

```
$ lsmod | grep dummy
dummy                  12288  0
$ ls -l /proc/dummy
-rw-r--r-- 1 root root 0 Jul 11 13:44 /proc/dummy
```

### Then, try to read 'Hello, World!' from /proc/dummy

```
$ cat /proc/dummy
Hello, World!
```

## Unloading module and remove /proc/dummy

```
$ sudo rmmod dummy
```
