DESCRIPTION = "Tool to remotely set a Voltcraft Power Supply"
SECTION = "app"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://voltcraft.c \
	   file://Makefile \
	   "

S = "${WORKDIR}"

FILES_${PN} = "${bindir}"

do_install () {
	install -d ${D}/${bindir}
	install -m 755 voltcraft ${D}/${bindir}
}

