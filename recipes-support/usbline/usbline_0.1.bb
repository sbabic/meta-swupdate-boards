DESCRIPTION = "Simple daemon to get ser2net port"
SECTION = "app"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://usbline \
	   file://usbline-start \
	   file://usbline.service \
           "

S = "${WORKDIR}"

inherit update-rc.d systemd

do_install () {
	install -d ${D}/${bindir}
	install -d ${D}/${sysconfdir}/init.d
    	install -d ${D}${systemd_system_unitdir}
	install -m 755 ${WORKDIR}/usbline ${D}/${bindir}
	install -m 755 ${WORKDIR}/usbline-start ${D}/${sysconfdir}/init.d
    	install -m 0644 ${WORKDIR}/usbline.service ${D}${systemd_system_unitdir}
}

FILES:${PN} = "${bindir} ${sysconfdir} ${systemd_system_unitdir}"

INITSCRIPT_NAME = "usbline-start"
INITSCRIPT_PARAMS:${PN} = "defaults 70 70"
SYSTEMD_SERVICE:${PN} = "usbline.service"
