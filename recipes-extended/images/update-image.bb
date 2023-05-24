DESCRIPTION = "Example image demonstrating how to build SWUpdate compound image"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit swupdate

SRC_URI = "\
    file://emmcsetup.lua \
    file://sw-description \
"

# images to build before building swupdate image
IMAGE_DEPENDS = "core-image-full-cmdline"

# images and files that will be included in the .swu image
SWUPDATE_IMAGES = "core-image-full-cmdline"

SWUPDATE_IMAGES_FSTYPES[core-image-full-cmdline] = ".ext4.gz"

do_swuimage:prepend () {
    machine = d.getVar("MACHINE", True) 
    workdir = d.getVar('WORKDIR', True)
    os.system(f'sed -i "s#@MACHINE@#{machine}#g" {workdir}/sw-description')
}
