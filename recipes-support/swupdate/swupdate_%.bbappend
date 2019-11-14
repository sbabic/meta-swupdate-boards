FILESEXTRAPATHS_append := "${THISDIR}/${PN}:"

PACKAGECONFIG_CONFARGS = ""

SRC_URI += " \
    file://09-swupdate-args \
    file://swupdate.cfg \
    "

SRC_URI_append_beaglebone-yocto = " file://10-remove-force-ro"

do_install_append() {
    install -m 0644 ${WORKDIR}/09-swupdate-args ${D}${libdir}/swupdate/conf.d/

    install -d ${D}${sysconfdir}
    install -m 644 ${WORKDIR}/swupdate.cfg ${D}${sysconfdir}
}

do_install_append_beaglebone-yocto() {
    # Recent swupdate as well as libubootenv handles force_ro flags automatically
    if ${@bb.utils.contains('DEPENDS','libubootenv','false','true',d)}; then
        install -m 0644 ${WORKDIR}/10-remove-force-ro ${D}${libdir}/swupdate/conf.d/
    fi
}