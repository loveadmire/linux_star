#!/bin/bash

KVER=`uname -r`
    mkdir -p /lib/modules/"$KVER"/kernel/drivers/riffa
	cp riffa.ko /lib/modules/"$KVER"/kernel/drivers/riffa/
	if [ -f "/etc/redhat-release" ]; then\
		printf "%b\n" "#!/bin/sh\nexec /sbin/modprobe riffa >/dev/null 2>&1" > /etc/sysconfig/modules/riffa.modules;\
		chmod 755 /etc/sysconfig/modules/riffa.modules;\
	else\
		if ! grep -Fxq "riffa" /etc/modules; then echo "riffa" >> /etc/modules; fi;\
	fi
	printf "%b\n" "KERNEL==\"riffa\", MODE=\"777\", GROUP=\"root\"" > /etc/udev/rules.d/99-riffa.rules
	printf "/usr/local/lib\n" > riffa.conf
	mv riffa.conf /etc/ld.so.conf.d/
	cp riffa_driver.h /usr/local/include/
	cp riffa.h /usr/local/include/
	mv libriffa.so.1.0 /usr/local/lib
	ln -sf /usr/local/lib/libriffa.so.1.0 /usr/local/lib/libriffa.so.1
	ln -sf /usr/local/lib/libriffa.so.1.0 /usr/local/lib/libriffa.so
	ldconfig
	depmod
