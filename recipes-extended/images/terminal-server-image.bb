DESCRIPTION = "A console-only image to be used as terminal server together with Lava"

IMAGE_FEATURES += "ssh-server-openssh"

IMAGE_FSTYPES += "btrfs.gz"

IMAGE_INSTALL = "\
	btrfs-tools \
	libubootenv-bin \
	packagegroup-core-boot \
	packagegroup-core-full-cmdline \
	${CORE_IMAGE_EXTRA_INSTALL} \
	dhcpcd \
	lua \
	openocd \
	ser2net \
	python3 \
	python3-datetime \
	python3-netclient \
	python3-netserver \
	swupdate \
	swupdate-progress \
	swupdate-www \
	swupdate-tools \
	u-boot-env \
	usbline \
	voltcraft \
	"
inherit core-image
