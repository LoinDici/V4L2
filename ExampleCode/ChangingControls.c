/*V4L2 application Changing controls sample code*/

static void change_control(void)
{
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;

	memset(&queryctrl, 0, sizeof(queryctrl));
	queryctrl.id = V4L2_CID_BRIGHTNESS;

	if (-1 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
		if (errno != EINVAL) {
			perror("VIDIOC_QUERYCTRL");
			exit(EXIT_FAILURE);
		} else
			printf("V4L2_CID_BRIGHTNESS is not supported\n");
	} else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
		printf("V4L2_CID_BRIGHTNESS is not supported\n");
	else {
		memset(&control, 0, siezeof(control));
		control.id = V4L2_CID_BRIGHTNESS;
		control.value  = queryctrl.default_value;

		if (-1 == ioctl(fd, VIDIOC_S_CTRL, &control)) {
			perror("VIDIOC_S_CTRL");
			exit(EXIT_FAILURE);
		}
	}

	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_CONTRAST;

	if (0 == ioctl(fd, VIDIOC_G_CTRL, &control)) {
		control.value += 1;
		/*The driver may clamp the value or return ERANGE, ignored here*/

		if (-1 == ioctl(fd, VIDIOC_S_CTRL, &control)
				&& errno != ERANGE) {
			perror("VIDIOC_S_CTRL");
			exit(EXIT_FAILURE);
		}
		/*Ignore if V4L2_CID_CONTRAST is unsupported*/
	} else if (errno != EINVAL) {
		perror("VIDIOC_G_CTRL");
		exit(EXIT_FAILURE);
	}

	control.id = V4L2_CID_AUDIO_MUTE;
	control.value = TRUE;	/*silence*/

	/*Errors ignored*/
	ioctl(fd, VIDIOC_S_CTRL, &control);
}
