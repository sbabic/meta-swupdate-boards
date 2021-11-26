DESCRIPTION = "A console-only image to be used as terminal server together with Lava"

IMAGE_FEATURES += "ssh-server-openssh"

IMAGE_FSTYPES += "btrfs.gz"
# images to build before building swupdate image
IMAGE_DEPENDS:append = " virtual/bootloader"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "\
    file://sw-description \
"

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
	swupdate-tools-ipc \
	u-boot-env \
	usbline \
	voltcraft \
	"

SYSTEM_IMAGE = "terminal-server-image"
SWUPDATE_IMAGES_FSTYPES[terminal-server-image] = ".ext4.gz"
SWUPDATE_IMAGES += "u-boot"
SWUPDATE_IMAGES_FSTYPES[u-boot] = ".bin"
inherit core-image
inherit swupdate-image
