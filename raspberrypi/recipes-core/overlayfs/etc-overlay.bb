SUMMARY = "Mount overlayfs on read-only rootfs"

DESCRIPTION = "The purpose is to make /etc read writeable"

LICENSE = "MIT"
LIC_FILES_CHKSUM="file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SECTION = "base"

PV = "0.1"

SRC_URI = " \
    file://etc.mount \
    file://overlays-etc-dirs.service \
"

inherit systemd

do_install () {
    install -d ${D}${systemd_system_unitdir}

    install -d ${D}/etc/ecs
    install -m 0644 ${WORKDIR}/etc.mount ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/overlays-etc-dirs.service ${D}${systemd_system_unitdir}
}

SYSTEMD_SERVICE_${PN} = "etc.mount overlays-etc-dirs.service"
