FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://ser2net.conf \
		file://ser2net.service \
"

inherit systemd

do_install:append() {
	install -d ${D}/${sysconfdir}
	install -m 644 ${WORKDIR}/ser2net.conf ${D}/${sysconfdir}
    	install -d ${D}${systemd_system_unitdir}
    	install -m 0644 ${WORKDIR}/ser2net.service ${D}${systemd_system_unitdir}
}

FILES:${PN} += "${sysconfdir} ${systemd_system_unitdir}"
SYSTEMD_SERVICE:${PN} = "ser2net.service"
