struct v4l2_queryctrl queryctrl;
struct v4l2_querymenu querymenu;

static void enumrate_menu(void)
{
	printf("Menu items:\n");

	memset(&querymenu, 0, sizeof(querymenu));
	querymenu.id = queryctrl.id;

	for (querymenu.index = queryctrl.minmum;
			querymenu.index <= queryctrl.maxmum;
			querymenu.index++) {
		if (0 == ioctl(fd, VIDIOC_QUERYMENU, &querymenu)) {
			printf("%s\n",querymenu.name);
		} else {
			perror("VIDIOC_QUERYMENU");
			exit(EXIT_FAILURE);
		}
	}
}

static void enum_all_control(void)
{
	memset(&queryctrl, 0, sizeof(queryctrl));

	for (queryctrl.id = V4L2_CID_BASE;
			queryctrl.id < V4L2_CID_LASTP1;
			queryctrl.id++) {
		if (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
			if (queryctrl.fags & V4L2_CTRL_FLAG_DISABLED)
				continue;
			printf("Control %s\n", queryctrl.name);

			if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
				enumerate_menu();
		} else {
			if (errno == EINVAL)
				break;
			perror("VIDIOC_QUERYCTRL");
			exit(EXIT_FAILURE);
		}
	}

	for (queryctrl.id = V4L2_CID_PRIVATE_BASE; ; queryctrl.id++) {
		if (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
			if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
				continue;

			printf("Control %s\n", queryctrl.name);

			if (queryctr.type == V4L2_CTRL_TYPE_MENU)
				enumerate_menu();

		} else {
			if (errno == EINVAL)
				break;

			perror("VIDIOC_QUERYCTRL");
			exit(EXIT_FAILURE);
		}
	}
}
