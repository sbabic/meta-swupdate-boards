FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append_class-target = " file://fw_env.config"

do_install_append_class-target() {
	install -d ${D}${sysconfdir}
	install -m 644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}
}

FILES_${PN}_append_class-target = " ${sysconfdir}"
