usb_src = ['dwhcidevice.cpp', 'dwhciregister.cpp', 'dwhcixferstagedata.cpp',
           'usbconfigparser.cpp', 'usbdevice.cpp', 'usbendpoint.cpp',
           'usbhostcontroller.cpp', 'usbrequest.cpp', 'smsc951x.cpp',
           'macaddress.cpp', 'netdevice.cpp', 'usbdevicefactory.cpp',
           'usbstandardhub.cpp', 'dwhciframeschednper.cpp', 'dwhciframeschedper.cpp',
           'dwhcirootport.cpp', 'usbgamepad.cpp', 'usbfunction.cpp',
           'dwhciframeschednsplit.cpp']

libusb = static_library('circle_usb', usb_src, include_directories: circle_include)

libusb_dep = declare_dependency(
	include_directories: circle_include,
	link_with: libusb)
