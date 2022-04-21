This is a test program for the `HIDIOCREVOKE` ioctl. Usage:

```
$ make
$ sudo ./revoke /dev/hidraw1
```

This program opens the given path, revokes the fd, verifies it gets ENODEV on
read and then closes the fd again.
