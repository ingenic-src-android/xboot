#ifndef __JZUSB_H
#define	__JZUSB_H
#include<config.h>
/*#ifndef u8
#define u8	unsigned char
#endif

#ifndef u16
#define u16	unsigned short
#endif

#ifndef u32
#define u32	unsigned int
#endif

#ifndef s8
#define s8	char
#endif

#ifndef s16
#define s16	short
#endif

#ifndef s32
#define s32	int
#endif

extern int usbdebug;*/

// Max packet size
#define EP0_MAXPACKETSIZE  	64
#define EPBULK_MAXPACKETSIZE  	512
#define EPINTR_MAXPACKETSIZE  	64
#define UDC_MAX_ENDPOINTS 4

#define USB_ENDPOINT_XFERTYPE_MASK	0x03	/* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL	0
#define USB_ENDPOINT_XFER_ISOC		1
#define USB_ENDPOINT_XFER_BULK		2
#define USB_ENDPOINT_XFER_INT		3
#define USB_ENDPOINT_MAX_ADJUSTABLE	0x80

/* CONTROL REQUEST SUPPORT */

/*
 * USB directions
 *
 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
 * It's also one of three fields in control requests bRequestType.
 */
#define USB_DIR_OUT			0		/* to device */
#define USB_DIR_IN			0x80		/* to host */

/*
 * Standard requests, for the bRequest field of a SETUP packet.
 *
 * These are qualified by the bRequestType field, so that for example
 * TYPE_CLASS or TYPE_VENDOR specific feature flags could be retrieved
 * by a GET_STATUS request.
 */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
#define USB_REQ_SET_FEATURE		0x03
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

#define USB_REQ_SET_ENCRYPTION		0x0D	/* Wireless USB */
#define USB_REQ_GET_ENCRYPTION		0x0E
#define USB_REQ_RPIPE_ABORT		0x0E
#define USB_REQ_SET_HANDSHAKE		0x0F
#define USB_REQ_RPIPE_RESET		0x0F
#define USB_REQ_GET_HANDSHAKE		0x10
#define USB_REQ_SET_CONNECTION		0x11
#define USB_REQ_SET_SECURITY_DATA	0x12
#define USB_REQ_GET_SECURITY_DATA	0x13
#define USB_REQ_SET_WUSB_DATA		0x14
#define USB_REQ_LOOPBACK_DATA_WRITE	0x15
#define USB_REQ_LOOPBACK_DATA_READ	0x16
#define USB_REQ_SET_INTERFACE_DS	0x17

/*
 * STANDARD DESCRIPTORS ... as returned by GET_DESCRIPTOR, or
 * (rarely) accepted by SET_DESCRIPTOR.
 *
 * Note that all multi-byte values here are encoded in little endian
 * byte order "on the wire".  But when exposed through Linux-USB APIs,
 * they've been converted to cpu byte order.
 */

/*
 * Descriptor types ... USB 2.0 spec table 9.5
 */
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05
#define USB_DT_DEVICE_QUALIFIER		0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07
#define USB_DT_INTERFACE_POWER		0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG			0x09
#define USB_DT_DEBUG			0x0a
#define USB_DT_INTERFACE_ASSOCIATION	0x0b
/* these are from the Wireless USB spec */
#define USB_DT_SECURITY			0x0c
#define USB_DT_KEY			0x0d
#define USB_DT_ENCRYPTION_TYPE		0x0e
#define USB_DT_BOS			0x0f
#define USB_DT_DEVICE_CAPABILITY	0x10
#define USB_DT_WIRELESS_ENDPOINT_COMP	0x11
#define USB_DT_WIRE_ADAPTER		0x21
#define USB_DT_RPIPE			0x22
#define USB_DT_CS_RADIO_CONTROL		0x23

/* Conventional codes for class-specific descriptors.  The convention is
 * defined in the USB "Common Class" Spec (3.11).  Individual class specs
 * are authoritative for their usage, not the "common class" writeup.
 */
#define USB_DT_CS_DEVICE		(USB_TYPE_CLASS | USB_DT_DEVICE)
#define USB_DT_CS_CONFIG		(USB_TYPE_CLASS | USB_DT_CONFIG)
#define USB_DT_CS_STRING		(USB_TYPE_CLASS | USB_DT_STRING)
#define USB_DT_CS_INTERFACE		(USB_TYPE_CLASS | USB_DT_INTERFACE)
#define USB_DT_CS_ENDPOINT		(USB_TYPE_CLASS | USB_DT_ENDPOINT)

enum USB_ENDPOINT_TYPE
{
	ENDPOINT_TYPE_CONTROL,
	ENDPOINT_TYPE_ISOCHRONOUS,
	ENDPOINT_TYPE_BULK,
	ENDPOINT_TYPE_INTERRUPT
};

/*enum USB_STANDARD_REQUEST_CODE 
{
	GET_STATUS,
	CLEAR_FEATURE,
	SET_FEATURE = 3,
	SET_ADDRESS = 5,
	GET_DESCRIPTOR,
	SET_DESCRIPTOR,
	GET_CONFIGURATION,
	SET_CONFIGURATION,
	GET_INTERFACE,
	SET_INTERFACE,
	SYNCH_FRAME
	};*/


enum USB_DESCRIPTOR_TYPE {
	DEVICE_DESCRIPTOR = 1,
	CONFIGURATION_DESCRIPTOR,
	STRING_DESCRIPTOR,
	INTERFACE_DESCRIPTOR,
	ENDPOINT_DESCRIPTOR,
	DEVICE_QUALIFIER_DESCRIPTOR,
	OTHER_SPEED_CONFIGURATION_DESCRIPTOR,
	INTERFACE_POWER1_DESCRIPTOR
};


enum USB_FEATURE_SELECTOR {
	ENDPOINT_HALT,
	DEVICE_REMOTE_WAKEUP,
	TEST_MODE
};

enum USB_CLASS_CODE {
	CLASS_DEVICE,
	CLASS_AUDIO,
	CLASS_COMM_AND_CDC_CONTROL,
	CLASS_HID,
	CLASS_PHYSICAL = 0x05,
	CLASS_STILL_IMAGING,
	CLASS_PRINTER,
	CLASS_MASS_STORAGE,
	CLASS_HUB,
	CLASS_CDC_DATA,
	CLASS_SMART_CARD,
	CLASS_CONTENT_SECURITY = 0x0d,
	CLASS_VIDEO,
	CLASS_DIAGNOSTIC_DEVICE = 0xdc,
	CLASS_WIRELESS_CONTROLLER = 0xe0,
	CLASS_MISCELLANEOUS = 0xef,
	CLASS_APP_SPECIFIC = 0xfe,
	CLASS_VENDOR_SPECIFIC = 0xff
};


typedef struct {
	u8 bmRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed)) USB_DeviceRequest;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8 iManufacturer;
	u8 iProduct;
	u8 iSerialNumber;
	u8 bNumConfigurations;
} __attribute__ ((packed)) USB_DeviceDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u8 bNumConfigurations;
	u8 bReserved;
} __attribute__ ((packed)) USB_DeviceQualifierDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 MaxPower;
} __attribute__ ((packed)) USB_ConfigDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 bMaxPower;
} __attribute__ ((packed)) USB_OtherSpeedConfigDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bInterfaceNumber;
	u8 bAlternateSetting;
	u8 bNumEndpoints;
	u8 bInterfaceClass;
	u8 bInterfaceSubClass;
	u8 bInterfaceProtocol;
	u8 iInterface;
} __attribute__ ((packed)) USB_InterfaceDescriptor;


typedef struct {
	u8 bLegth;
	u8 bDescriptorType;
	u8 bEndpointAddress;
	u8 bmAttributes;
	u16 wMaxPacketSize;
	u8 bInterval;
} __attribute__ ((packed)) USB_EndPointDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 SomeDesriptor[1];
} __attribute__ ((packed)) USB_StringDescriptor;

/**
 * struct usb_ctrlrequest - SETUP data for a USB device control request
 * @bRequestType: matches the USB bmRequestType field
 * @bRequest: matches the USB bRequest field
 * @wValue: matches the USB wValue field (le16 byte order)
 * @wIndex: matches the USB wIndex field (le16 byte order)
 * @wLength: matches the USB wLength field (le16 byte order)
 *
 * This structure is used to send control requests to a USB device.  It matches
 * the different fields of the USB 2.0 Spec section 9.3, table 9-2.  See the
 * USB spec for a fuller description of the different fields, and what they are
 * used for.
 *
 * Note that the driver for any interface can issue control requests.
 * For most devices, interfaces don't coordinate with each other, so
 * such requests may be made at any time.
 */
struct usb_ctrlrequest {
	u8 bRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed));

enum ep0state {
	USB_EP0_IDLE,
	USB_EP0_TX,
	USB_EP0_RX,
	USB_EP0_SET_FUNADDR/*add by hfwang*/
};

typedef enum ep_type {
	ep_control, ep_bulk_in, ep_bulk_out, ep_interrupt
} ep_type_t;

struct usb_ep {
	char *name;
	int maxpacket;
};

struct jz4740_ep {
	struct usb_ep ep;
	const struct usb_endpoint_descriptor *desc;
	u8 stopped;
	u8 bEndpointAddress;
	u8 bmAttributes;
	ep_type_t ep_type;
	u32 fifo;
	u32 csr;
	u32 reg_addr;
};

struct jz4740_udc {
	enum ep0state ep0state;
	struct jz4740_ep ep[UDC_MAX_ENDPOINTS];
	unsigned char usb_address;
};

#endif // !defined(__USB_H)

