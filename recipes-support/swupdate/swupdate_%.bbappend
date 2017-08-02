FILESEXTRAPATHS_append := "${THISDIR}/${PN}:"

PACKAGECONFIG_CONFARGS = ""

SRC_URI += " \
     file://swupdate.cfg \
     "

do_install_append() {
    install -d ${D}${sysconfdir}
    install -m 644 ${WORKDIR}/swupdate.cfg ${D}${sysconfdir}
}
