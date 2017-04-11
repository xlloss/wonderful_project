#ifndef BLE_GATTS_SERVICES_TABLE_H
#define BLE_GATTS_SERVICES_TABLE_H

#include "ble_gatt.h"

/**
 * @addtogroup BLE_GATT_SERVICE_TABLE BLE GATT SERVICE TABLE
 * @{
 * @brief Definitions of GATT Service Table
 */

/**@defgroup BLE_GATT_UUID_DEFINES  BLE_GATT_UUID_DEFINES
 * @brief GATT UUID Definitions
 * @{ */
#define UUID_PRIMARY_SERVICE						0x2800	/**< Primary Service Declaration.*/
#define UUID_SECONDARY_SERVICE						0x2801	/**< Secondary Service Declaration.*/
#define UUID_INCLUDE								0x2802	/**< Include Declaration.*/
#define UUID_CHARACTERISTIC 						0x2803	/**< Characteristic Declaration.*/
#define UUID_DESCRIPTOR_CHAR_EXT_PROP				0x2900	/**< Characteristic Extended Properties.*/
#define UUID_DESCRIPTOR_USER_DESC					0x2901	/**< Characteristic User Description Descriptor.*/
#define UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG			0x2902	/**< Client Characteristic Configuration Descriptor.*/
#define UUID_DESCRIPTOR_SERVER_CHAR_CONFIG			0x2903	/**< Server Characteristic Configuration Descriptor.*/
#define UUID_DESCRIPTOR_CHAR_PRE_FORMAT				0x2904	/**< Characteristic Presentation Format Descriptor.*/
#define UUID_DESCRIPTOR_CHAR_AGGRE_FROMAT			0x2905	/**< Characteristic Aggregate Format Descriptor.*/
/** @} */


/**@defgroup BLE_GATT_ATTRIBUTE_PERMISSIONS BLE_GATT_ATTRIBUTE_PERMISSIONS
 * @brief Attribute Permissions. All attributes are readable as default.
 * @{ */
#define PM_WRITABLE					0x02	/**< Access Permission: Writeable.*/
#define PM_SECURITY_READ_ENABLE		0x04	/**< Encryption/Authentication Permission on READ property attribute. If enabled, it requires pairing to device to access the attribute. Note that Encryption or Authentication permission is based on IO capability of device. */
#define PM_SECURITY_WRITE_ENABLE	0x08	/**< Encryption/Authentication Permission on WRITE property attribute. If enabled, it requires pairing to device to access the attribute. Note that Encryption or Authentication permission is based on IO capability of device. */
#define PM_MANUAL_WRITE_RESP		0x40	/**< Authorization Permission: Manual sending write response configuration. Application can send write response manually. */
#define PM_MANUAL_READ_RESP			0x80	/**< Authorization Permission: Manual sending read response configuration. Application can send read response with data manually. */
/** @} */


/**@defgroup BLE_GATT_CHARACTERISTIC_PROPERTIES BLE_GATT_CHARACTERISTIC_PROPERTIES
 * @brief Characteristic Properties
 * @{ */
#define BROADCAST							0x01	/**< If set, permits broadcasts of Characteristic Value using Server Characteristic Configuration Descriptor.*/
#define READ								0x02	/**< If set, permits reads of the Characteristic Value.*/
#define WRITE_WITHOUT_RESPONSE				0x04	/**< If set, permits writes of Characteristic Value without response.*/
#define WRITE								0x08	/**< If set, permits writes of Characteristic Value with response.*/
#define NOTIFY								0x10	/**< If set, permits notifications of a Characteristic Value without acknowledgement.*/
#define INDICATE							0x20	/**< If set, permits indications of Characteristic Value with acknowledgement.*/
#define AUTHENTICATED_SIGNED_WRITES			0x40	/**< If set, permits signed writes to the Characteristic Value.*/
#define EXTENDED_PROPERTIES					0x80	/**< If set, additional characteristic properties are defined in the Characteristic Extended Properties Descriptor.*/
/** @} */


/**@defgroup BLE_GATT_CHARACTERISTIC_EXTENDED_PROPERTIES BLE_GATT_CHARACTERISTIC_EXTENDED_PROPERTIES
 * @brief Characteristic Extended Properties bit field
 * @{ */
#define RELIABLE_WRITE						0x0001	/**< If set, permits reliable writes of Characteristic Value.*/
#define WRITABLE_AUXILIARIES				0x0002	/**< If set, permits writes to the Characteristic Descriptor.*/
/** @} */

/**@defgroup BLE_GATT_CCCD_BITS BLE_GATT_CCCD_BITS
 * @brief Client Characteristic Configuration bit field definition
 * @{ */
#define NOTIFICATION						0x0001	/**< The Characteristic Value shall be notified.*/
#define INDICATION							0x0002	/**< The Characteristic Value shall be indicated.*/
/** @} */


/**@defgroup BLE_GATT_SCCD_BITS BLE_GATT_SCCD_BITS
 * @brief Server Characteristic Configuration bit field definition
 * @{ */
#define SCCD_BROADCAST						0x0001	/**< The Characteristic Value shall be broadcast.*/
/** @} */


/**@defgroup BLE_ATT_UUID_LENGTH_TYPES BLE_ATT_UUID_LENGTH_TYPES
 * @brief BLE ATT UUID lengths
 * @{ */
#define BLE_ATT_UUID_LENGTH_2		2		/**< 16-bit Bluetooth UUID. */
#define BLE_ATT_UUID_LENGTH_4		4		/**< 32-bit Bluetooth UUID. */
#define BLE_ATT_UUID_LENGTH_16		16		/**< 128-bit Bluetooth UUID. */
/** @} */


/** @brief The Possible Maximum Attribute Length. */
#define ATTRIBUTE_MAX_LENGTH		(sizeof(ATT_ATTRUBUTE_HEADER_16)+sizeof(ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE))

/**
 * @brief The attribute header part definition of one attribute with attribute type is 16-bit UUID.
 */
typedef struct ATT_ATTRUBUTE_HEADER_2
{
	u8			attrLength;							/**< Length of the Whole Attribute. Including Header and Value. */
	u8			attrPermission;						/**< The Attribute Permission. */
	u16			attrHandle;							/**< The Attribute Handle. */
	u8			attrTypeLength;						/**< Length of the Attribute Type. */
	u8			attrType[BLE_ATT_UUID_LENGTH_2];	/**< The 16-bit Attribute Type. */
}	ATT_ATTRUBUTE_HEADER_2;

/**
 * @brief The attribute header part definition of one attribute with attribute type is 32-bit UUID.
 */
typedef struct ATT_ATTRUBUTE_HEADER_4
{
	u8			attrLength;							/**< Length of the Whole Attribute. Including Header and Value. */
	u8			attrPermission;						/**< The Attribute Permission. */
	u16			attrHandle;							/**< The Attribute Handle. */
	u8			attrTypeLength;						/**< Length of the Attribute Type. */
	u8			attrType[BLE_ATT_UUID_LENGTH_4];	/**< The 32-bit Attribute Type. */
}	ATT_ATTRUBUTE_HEADER_4;

/**
 * @brief The attribute header part definition of one attribute with attribute type is 128-bit UUID.
 */
typedef struct ATT_ATTRUBUTE_HEADER_16
{
	u8			attrLength;							/**< Length of the Whole Attribute. Including Header and Value. */
	u8			attrPermission;						/**< The Attribute Permission. */
	u16			attrHandle;							/**< The Attribute Handle. */
	u8			attrTypeLength;						/**< Length of the Attribute Type. */
	u8			attrType[BLE_ATT_UUID_LENGTH_16];	/**< The 128-bit Attribute Type. */
}	ATT_ATTRUBUTE_HEADER_16;

/**
 * @brief The attribute value part definition of one attribute. The value format is for 16-bit Primary/Secondary Service definition.
 */
typedef struct ATT_ATTRIBUTE_VALUE_SERVICE_2
{
	u16			endHandle;							/**< End Handle of the Service. */
	u8			servUUID[BLE_ATT_UUID_LENGTH_2];	/**< 16-bit Bluetooth UUID for Service. */
}	ATT_ATTRIBUTE_VALUE_SERVICE_2;

/**
 * @brief The attribute value part definition of one attribute. The value format is for 128-bit Primary/Secondary Service definition.
 */
typedef struct ATT_ATTRIBUTE_VALUE_SERVICE_16
{
	u16			endHandle;							/**< End Handle of the Service. */
	u8			servUUID[BLE_ATT_UUID_LENGTH_16];	/**< 128-bit UUID for Service. */
}	ATT_ATTRIBUTE_VALUE_SERVICE_16;

/**
 * @brief The attribute value part definition of one attribute. The value format is for 16-bit Include Service definition.
 *        The Service UUID shall only be present when the UUID is a 16-bit Bluetooth UUID. Core Spec. 4.2 vol.3, Part G, Sec. 3.2.
 */
typedef struct ATT_ATTRIBUTE_VALUE_INCLUDE_2
{
	u16			includeHandle;						/**< Included Service Attribute Handle. */
	u16			endGroupHandle;						/**< End Group Handle. */
	u8			servUUID[BLE_ATT_UUID_LENGTH_2];	/**< 16-bit Service UUID. */
}	ATT_ATTRIBUTE_VALUE_INCLUDE_2;

/**
 * @brief The attribute value part definition of one attribute. The value format is for 16-bit Characteristic definition.
 */
typedef struct ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2
{
	u8			property;							/**< Bit field of characteristic properties. */
	u16			valueHandle;						/**< Handle of the Attribute containing the value of this characteristic. */
	u8			charUUID[BLE_ATT_UUID_LENGTH_2];	/**< 16-bit Bluetooth UUID for Characteristic Value. */
}	ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2;

/**
 * @brief The attribute value part definition of one attribute. The value format is for 128-bit Characteristic definition.
 */
typedef struct ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16
{
	u8			property;							/**< Bit field of characteristic properties. */
	u16			valueHandle;						/**< Handle of the Attribute containing the value of this characteristic. */
	u8			charUUID[BLE_ATT_UUID_LENGTH_16];	/**< 128-bit UUID for Characteristic Value. */
}	ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Characteristic Value definition.
 */
typedef struct ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE
{
	u8			charValueLength;							/**< Length of the Characteristic Value. */
	u8			charValue[BLE_ATT_ATTRIBUTE_VALUE_LEN];		/**< Characteristic Value. */
}	ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Extended Properties Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_DESCRIPTOR_EXTENDED_PROP
{
	u16			bitField;		/**< Characteristic Configuration Bits. */
}	ATT_ATTRIBUTE_DESCRIPTOR_EXTENDED_PROP;

/**
 * @brief The attribute value part definition of one attribute. The value format is for User Description Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_DESCRIPTOR_USER_DESC
{
	u8			descLength;									/**< Length of the description string. */
	u8			description[BLE_ATT_ATTRIBUTE_VALUE_LEN];	/**< Characteristic User Description UTF-8 String. */
}	ATT_ATTRIBUTE_DESCRIPTOR_USER_DESC;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Client Characteristic Configuration Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_DESCRIPTOR_CCCD
{
	u16			bitField;		/**< Characteristic Configuration Bits. */
}	ATT_ATTRIBUTE_DESCRIPTOR_CCCD;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Server Characteristic Configuration Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_DESCRIPTOR_SCCD
{
	u16			bitField;			/**< Characteristic Configuration Bits. */
}	ATT_ATTRIBUTE_DESCRIPTOR_SCCD;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Presentation Format Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_DESCRIPTOR_PRE_FORMAT
{
	u8			format;				/**< Format of the value of this characteristic. */
	u8			exponent;			/**< Exponent field to determine how the value of this characteristic is further formatted. */
	u16			unit;				/**< The unit of this characteristic. */
	u8			nameSpace;			/**< The name space of the description. */
	u16			description;		/**< The description of this characteristic as defined in a higher layer profile.. */
}	ATT_ATTRIBUTE_DESCRIPTOR_PRE_FORMAT;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Aggregate Format Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_DESCRIPTOR_AGGRE_FORMAT
{
	u8			numOfHandles;										/**< Number of handles */
	u16			attrHandles[(BLE_ATT_ATTRIBUTE_VALUE_LEN/2)];		/**< List of Attribute Handles for the Characteristic Presentation Format Declarations. */
}	ATT_ATTRIBUTE_DESCRIPTOR_AGGRE_FORMAT;

/**
 * @brief The attribute value part definition of one attribute. The value format is for Proprietary defined Descriptor definition.
 */
typedef struct ATT_ATTRIBUTE_GENERIC_DESCRIPTOR
{
	u8			descLength;									/**< Length of the descriptor value. */
	u8			descValue[BLE_ATT_ATTRIBUTE_VALUE_LEN];		/**< Value of the Descriptor. */
}	ATT_ATTRIBUTE_GENERIC_DESCRIPTOR;

#endif

/**
  @}
 */