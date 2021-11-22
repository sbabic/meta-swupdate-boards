DESCRIPTION = "A console-only image to be used as terminal server together with Lava"

IMAGE_FEATURES += "ssh-server-openssh"

IMAGE_FSTYPES += "btrfs.gz"

IMAGE_INSTALL = "\
	btrfs-tools \
	packagegroup-core-boot \
	packagegroup-core-full-cmdline \
	${CORE_IMAGE_EXTRA_INSTALL} \
	dhcp-client \
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
	u-boot-fw-utils \
	usbline \
	voltcraft \
	"
inherit core-image