CODE_NAMES = trusty,xenial,artful,bionic
DPUT_CONFIGURATION_NAME = milter-manager-ppa
SOURCE = ../$(PACKAGE)-$(VERSION).tar.gz

all:

ensure-launchpad-configuration:
	@if test -z "$(LAUNCHPAD_UPLOADER_PGP_KEY)"; then		\
	  echo "--with-launchpad-uploader-pgp-key configure option must be specified.";	\
	  false;							\
	fi

upload: source ensure-launchpad-configuration
	rm -rf debian
	cp -a '$(srcdir)/../debian' debian
	sed -i -e '/cutter/d' debian/control
	sed -i -e '/TZ=Asia/d' debian/rules
	./upload.rb \
	  --package '$(PACKAGE)' \
	  --version '$(VERSION)' \
	  --source-archive '$(SOURCE)' \
	  --dput-configuration-name '$(DPUT_CONFIGURATION_NAME)' \
	  --code-names '$(CODE_NAMES)' \
	  --debian-directory 'debian' \
	  --pgp-sign-key '$(LAUNCHPAD_UPLOADER_PGP_KEY)'

source: $(SOURCE)

$(SOURCE):
	ln -s $(abs_top_builddir)/$(PACKAGE)-$(VERSION).tar.gz $(SOURCE)
