saveenv
fdt addr ${fdt_addr} && fdt get value bootargs /chosen bootargs
fatload mmc 0:1 ${kernel_addr_r} uImage;
if env exists rpipart;then setenv bootargs ${bootargs} root=/dev/mmcblk0p${rpipart}; fi
bootm ${kernel_addr_r} - ${fdt_addr}
