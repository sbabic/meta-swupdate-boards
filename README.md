# meta-swupdate-boards

The layer contains examples how to use the SWUpdate projects. Examples
with some common boards (Beaglebone Black, RaspberryPI) are provided.

Please see the corresponding sections below for details.

Dependencies
============

This layer depends on:

  URI: git://git.openembedded.org/bitbake

  URI: git://git.openembedded.org/openembedded-core
  layers: meta

  URI: git://github.com/sbabic/meta-swupdate.git
  layers: meta-swupdate

Maintainer
----------

Stefano Babic <sbabic@denx.de>

Submitting patches
------------------

You can submit your patches (or post questions reagarding
this layer to the swupdate Mailing List:

	swupdate@googlegroups.com

When creating patches, please use something like:

    git format-patch -s --subject-prefix='meta-swupdate][PATCH' <revision range>

Please use 'git send- email' to send the generated patches to the ML
to bypass changes from your mailer.
