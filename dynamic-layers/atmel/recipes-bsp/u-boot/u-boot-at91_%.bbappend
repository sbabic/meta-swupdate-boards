FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

PACKAGE_BEFORE_PN += "${PN}-env"
RPROVIDES_${PN}-env += "u-boot-default-env"
