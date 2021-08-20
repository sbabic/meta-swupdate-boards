FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

PACKAGE_BEFORE_PN += "${PN}-env"
RPROVIDES:${PN}-env += "u-boot-default-env"
