# meta-swupdate-boards

Please see the corresponding sections below for details.
Has Raspberrypi2 support, but is not tested yet.

Dependencies
============

This layer depends on:

* URI: git://git.openembedded.org/bitbake
  * branch: master
  * revision: HEAD

* URI: git://git.openembedded.org/openembedded-core
  * branch: master
  * revision: HEAD

or

* URI: git://git.yoctoproject.org/poky
  * branch: master
  * revision: HEAD

and

* URI: git://github.com/sbabic/meta-swupdate.git
  * branch: master
  * revision: HEAD

For usage with Raspberry Pi boards additional layer is required:

* URI: git://github.com/agherzan/meta-raspberrypi.git
  * branch: master
  * revision: HEAD

Usage
-----

The layer contains examples on how to use the SWUpdate project. Examples
on how to deploy a "dual-copy" update strategy with some common boards
(Beaglebone Black, Raspberry Pi, Sama5d27-som1-ek-sd and Wandboard) are
provided.

Setup your environment accordingly and update `MACHINE` to desired target.

To build simply run:

	bitbake update-image

Above will generate a `swu` file suitable for usage with SWUpdate on
your device.

Note that `update-image` depends on `ext4.gz` and you must make sure
that it is part of `IMAGE_FSTYPES`.

For usage with Raspberry Pi one must add the following to `local.conf`

	RPI_USE_U_BOOT = "1"

Above will enable U-boot which Raspberry Pi does not default to, and
instead boots straight to Linux. U-boot is required to do the "swapping"
of partitions in the "dual-copy" layout.

Maintainer
----------

Stefano Babic <sbabic@denx.de>

Submitting patches
------------------

You can submit your patches or post questions regarding
this layer to the swupdate Mailing List:

	swupdate@googlegroups.com

When creating patches, please use something like:

    git format-patch -s --subject-prefix='meta-swupdate-boards][PATCH' <revision range>

Please use 'git send-email' to send the generated patches to the ML
to bypass changes from your mailer.
