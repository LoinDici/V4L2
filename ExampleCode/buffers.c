/* Mapping buffers */

static void mapping_buffers(void)
{
	struct v4l2_requestbuffers reqbuf;
	struct {
		void *start;
		size_t length;
	} *buffers;
	unsigned int i;

	memset(&reqbuf, 0, sizeof(reqbuf));
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP;
	reqbuf.count = 20;

	if (-1 == ioctl(fd, VIDIOC_REQBUFS, &reqbuf)) {
		if (errno == EINVAL)
			printf("Video capturing or mmap-streaming is
					not supported\n");
		else
			perror("VIDIOC_REQBUFS");

		exit(EXIT_FAILURE);
	}

	/* We want at least five buffers. */
	if (reqbuf.count < 5) {
		/* You may need to free the buffers here. */
		printf("Not enough buffer memory\n");
		exit(EXIT_FAILURE);
	}

	buffers = calloc(reqbuf.count, sizeof(*buffers));
	assert(buffers != NULL);

	for (i = 0; i < reqbuf.count; i++) {
		struct v4l2_buffer buffer;

		memset(&buffer, 0, sizeof(buffer));
		buffer.type = reqbuf.type;
		buffer.memory = V4L2_MEMORY_MMAP;
		buffer.index = i;

		if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buffer)) {
			perror("VIDIOC_QUERYBUF");
			exit(EXIT_FAILURE);
		}

		buffers[i].length = buffer.length; /* remember for munmap */
		buffers[i].start = mmap(NULL, buffer.length,
				PORT_READ | PORT_WRITE, /* recommended*/
				MAP_SHARED,	/* recommended*/
				fd, buffer.m.offset);

		if (MAP_FAILED == buffers[i].start) {
			/* If you do not exit here you should unmap() and free()*/
			perror("mmap");
			exit(EXIT_FAILURE);
		}
	}

	/* Cleanup. */
	for (i = 0; i < reqbuf.count; i++)
		munmap(buffers[i].start, buffers[i].length);
}

/* Initiating streaming I/O with user pointers */

static void initiating_streaming_io_with_user_pointers(void)
{
	struct v4l2_requestbuffers reqbuf;

	memset(&reqbuf, 0, sizeof(reqbuf));
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_USERPTR;

	if (-1 == ioctl(fd, VIDIOC_REQBUFS, &reqbuf)){
		if (errno == EINVAL)
			printf("Video capturing or user pointer streaming
					is not supported\n");
		else
			perror("VIDIOC_REQBUFS");

		exit(EXIT_FAILURE);
	}
}
