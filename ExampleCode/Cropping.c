/* Resetting the cropping parameters sample code
 * A video capture device is assumed; change V4L2_BUF_TYPE_VIDEO_CAPTURE
 * for other devices.
 */

struct v4l2_crocap cropcap;

static reset_cropping_parametes foo(void)
{
	struct v4l2_crop crop;

	memset(&cropcap, 0, sizeof(cropcap));
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl(fd, VIDICO_CROPCAP, &cropcap)) {
		perror("VIDIOC_CROPCAP");
		exit(EXIT_FAILURE);
	}

	memset(&crop, 0, sizeof(crop));
	crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	crop.c = cropcap.defrect;

	/* Ignore if cropping is not supported (EINVAL) */
	if (-1 == ioctl(fd, VIDIOC_S_CROP, &crop)
			&& errno != EINVAL) {
		perror("VIDIOC_S_CROP");
		exit(EXIT_FAILURE);
	}
}

static void simple_downscaling(void)
{
	struct v4l2_format format;

	reset_cropping_parameters();

	/* Scale down to 1/4 size of full picture. */
	memset(&format, 0, sizeof(format)); /* defaults */

	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	fromat.fmt.pix.width = cropcap.defrect.width >> 1;
	format.fmt.pix.height = crocap.defrect.height >> 1;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

	if (-1 == ioctl(fd, VIDIOC_S_FMT, &format)) {
		perror("VIDIOC_S_FMT");
		exit(EXIT_FAILURE);
	}
	/* We could check the actual image size now, the actual scaling
	 * factor or if the driver can scale at all. 
 	 */
}

static void selecting_an_ouput_area(void)
{
	struct v4l2_cropcap crocap;
	struct v4l2_crop crop;

	memset(&crocap, 0, sizeof(crocap));
	crocap.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

	if (-1 == ioctl(fd, VIDIOC_CROPCAP, &crocap)) {
		perror("VIDIOC_CAROPCAP");
		exit(EXIT_FAILURE);
	}

	memset(&crop, 0, sizeof(crop));

	crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	crop.c = crocap.defrect;

	/* Scale the width and height to 50% of their original size
	 * and center the output
	 */

	crop.c.width /= 2;
	crop.c.height /= 2;
	crop.c.letf += crop.c.width;
	crop.c.top += crop.c.height;

	/* Ignore if cropping is not supported (EINVAL). */
	if (-1 == ioctl(fd, VIDIOC_S_CROP, &crop)
		&& errno != EINVAL) {
		perror("VIDIOC_S_CROP");
		exit(EXIT_FAILURE);
	}
}

/* Current scaling factor and pixel aspect ratio 
 * A video capture device is assumed.
 */

static void current_scaling_factore_pix_aspect_ratio(void)
{
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format format;
	double hscale, vscale, aspect;
	int dwidth, dheight;

	memset(&cropcap, 0, sizeof(cropcap));
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		perror("VIDIOC_CROCAP");
		exit(EXIT_FAILURE);
	}

	memset(&crop, 0, sizeof(crop));
	crop.type = V4L2_BUF_TYPE_CAPTURE;

	if (-1 == ioctl(fd, VIDIOC_G_CROP, &crop)) {
		if (errno != EINVAL) {
			perror("VIDIOC_G_CROP");
			exit(EXIT_FAILURE);
		}
		/* Cropping not supported. */
		crop.c = cropcap.defrect;
	}

	memset(&format, 0, sizeof(format));
	format.fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl(fd, VIDIOC_G_FMT, &format)) {
		perror("VIDIOC_G_FMT");
		exit(EXIT_FAILURE);
	}
	
	/* The scaling applied by the driver. */
	hscale = format.fmt.pix.width / (double) crop.c.width;
	vscale = format.fmt.pix.height / (double) crop.c.height;

	aspect = cropcap.pixelaspect.numerator /
		(double) cropcap.pixelaspect.denominator;
	aspect = aspect * hscale / vscale;
	
	/* Devices following ITU-R BT.601 do not capture
	 * square pixels. For playback on a computer monitor
	 * we should scale the images to this size.
	 */
	dwidth = format.fmt.pix.width / aspect;
	dheight = format.fmt.pix.height;
}
