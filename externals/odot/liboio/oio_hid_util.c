#include "oio_hid_util.h"

t_oio_err oio_hid_util_getDeviceProductID(IOHIDDeviceRef device, long bufsize, char *buf){
	CFTypeRef productKey = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
	if(productKey){
		if(CFStringGetTypeID() == CFGetTypeID(productKey)){
			CFIndex len = (int)CFStringGetLength((CFStringRef)productKey);
			if(len > 0){
				if(CFStringGetCString((CFStringRef)productKey, buf, bufsize, kCFStringEncodingUTF8) == false){
					OIO_ERROR(OIO_ERR_CFTYPE);
					return OIO_ERR_CFTYPE;
				}
			}
		}
	}
	return OIO_ERR_NONE;
}

void oio_hid_util_dumpDeviceInfo(IOHIDDeviceRef device){
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDProductKey                    ));

	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDTransportKey                  ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDVendorIDKey                   ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDVendorIDSourceKey             ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDProductIDKey                  ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDVersionNumberKey              ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDManufacturerKey               ));

	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDSerialNumberKey               ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDCountryCodeKey                ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDLocationIDKey                 ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDDeviceUsageKey                ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDDeviceUsagePageKey            ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDDeviceUsagePairsKey           ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDPrimaryUsageKey               ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDPrimaryUsagePageKey           ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDMaxInputReportSizeKey         ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDMaxOutputReportSizeKey        ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDMaxFeatureReportSizeKey       ));
	oio_hid_util_postDeviceKey(device, CFSTR( kIOHIDReportIntervalKey             ));

	printf("************************************************************************************************\n");
	printf("************************************************************************************************\n");
	printf("************************************************************************************************\n");

}

void oio_hid_util_dumpElementInfo(IOHIDElementRef element){
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCookieKey                      ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementTypeKey                        ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCollectionTypeKey              ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementUsageKey                       ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementUsagePageKey                   ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementMinKey                         ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementMaxKey                         ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementScaledMinKey                   ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementScaledMaxKey                   ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementSizeKey                        ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementReportSizeKey                  ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementReportCountKey                 ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementReportIDKey                    ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementIsArrayKey                     ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementIsRelativeKey                  ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementIsWrappingKey                  ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementIsNonLinearKey                 ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementHasPreferredStateKey           ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementHasNullStateKey                ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementFlagsKey                       ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementUnitKey                        ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementUnitExponentKey                ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementNameKey                        ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementValueLocationKey               ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementDuplicateIndexKey              ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementParentCollectionKey            ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementVendorSpecificKey              ));

	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationMinKey              ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationMaxKey              ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationSaturationMinKey    ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationSaturationMaxKey    ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationDeadZoneMinKey      ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationDeadZoneMaxKey      ));
	oio_hid_util_postElementKey(element, CFSTR(kIOHIDElementCalibrationGranularityKey      ));

	printf("************************************************************************************************\n");
	printf("************************************************************************************************\n");
	printf("************************************************************************************************\n");
}

void oio_hid_util_postDeviceKey(IOHIDDeviceRef device, CFStringRef key){
	CFTypeRef k = IOHIDDeviceGetProperty(device, key);
	char buf[256];
	CFStringGetCString(key, buf, 256, kCFStringEncodingUTF8);
	printf("%s:\n", buf);
	CFShow(k);
}

void oio_hid_util_postElementKey(IOHIDElementRef element, CFStringRef key){
	CFTypeRef k = IOHIDElementGetProperty(element, key);
	char buf[256];
	CFStringGetCString(key, buf, 256, kCFStringEncodingUTF8);
	printf("%s:\n", buf);
	CFShow(k);
}
