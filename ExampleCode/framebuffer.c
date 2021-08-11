/* Finding a framebuffer device for OSD */
#include <linux/fb.h>

static void finding_a_framebuffer_device_for_OSD(void)
{
	struct v4l2_framebuffer fbuf;
	unsigned int i;
	int fb_fd;

	if (-1 == ioctl(fd, VIDIOC_G_FBUF, &fbuf)) {
		perror("VIDIOC_G_FBUF");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 30; i++) {
		char dev_name[16];
		struct fb_fix_screeninfo si;

		snprintf(dev_name, sizeof(dev_name), "/dev/fb%u", i);

		fb_fd = open(dev_name, O_RDWR);
		if (-1 == fb_fd) {
			switch (errno) {
			case ENOENT:	/* no such file */
			case ENXIO:	/* no driver */
				continue;
			default:
				perror("open");
				exit(EXIT_FAILURE);
			}
		}

		if (0 == ioctl(fb_fd, FBIOGET_FSCREENINFO, &si)) {
			if (si.smem_start == (unsigned long) fbuf.base)
				break;
		} else {
			/* Apparently not a framebuffer device. */
		}

		close(fb_fd);
		fb_fd = -1;
	}
	/* fb_fd is the file descriptor of the framebuffer device
	 * for the video output overlay, or -1 if no device was found. 
	 */
}
