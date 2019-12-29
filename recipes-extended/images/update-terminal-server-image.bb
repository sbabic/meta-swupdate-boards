DESCRIPTION = "Terminal server SWU"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

FILESEXTRAPATHS_prepend := "${THISDIR}/update-image:"
inherit swupdate

SRC_URI = "\
    file://sw-description \
"

SYSTEM_IMAGE = "terminal-server-image"

# images to build before building swupdate image
IMAGE_DEPENDS_append = " ${SYSTEM_IMAGE}"

# images and files that will be included in the .swu image
SWUPDATE_IMAGES_append = " ${SYSTEM_IMAGE}"

SWUPDATE_IMAGES_FSTYPES[terminal-server-image] = ".ext4.gz"
