#include "usbd.h"
#include "usbd_ctlreq.h"

#define RFLINK_OUT_EP	0x81
#define RFLINK_IN_EP	0x01
#define RFLINK_OUT_EP_SIZE 64
#define RFLINK_IN_EP_SIZE  64

static uint8_t  USBD_RfLink_Init (USBD_HandleTypeDef *pdev,
				uint8_t cfgidx);

static uint8_t  USBD_RfLink_DeInit (USBD_HandleTypeDef *pdev,
				uint8_t cfgidx);

static uint8_t  USBD_RfLink_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_RfLink_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_RfLink_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_RfLink_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_RfLink_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_RfLink_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_RfLink_EP0_TxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_RfLink_SOF (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_RfLink_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_RfLink_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);


USBD_ClassTypeDef  USBD_RfLink_ClassDriver =
{
	USBD_RfLink_Init,
	USBD_RfLink_DeInit,
	USBD_RfLink_Setup,
	USBD_RfLink_EP0_TxReady,
	USBD_RfLink_EP0_RxReady,
	USBD_RfLink_DataIn,
	USBD_RfLink_DataOut,
	USBD_RfLink_SOF,
	USBD_RfLink_IsoINIncomplete,
	USBD_RfLink_IsoOutIncomplete,
	USBD_RfLink_GetCfgDesc,
	USBD_RfLink_GetCfgDesc,
	USBD_RfLink_GetCfgDesc,
	USBD_RfLink_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB RfLink device Configuration Descriptor */
static uint8_t USBD_RfLink_CfgDesc[USB_RFLINK_CONFIG_DESC_SIZ] =
{
	0x09, /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
	USB_RFLINK_CONFIG_DESC_SIZ,
	/* wTotalLength: Bytes returned */
	0x00,
	0x01,         /*bNumInterfaces: 1 interface*/
	0x01,         /*bConfigurationValue: Configuration value*/
	0x00,         /*iConfiguration: Index of string descriptor describing
			the configuration*/
	0x80,         /*bmAttributes: bus powered and Support Remote Wake-up */
	0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

	/************** Descriptor of RfLink interface ****************/

	0x09,         /*bLength: Interface Descriptor size*/
	USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
	0x00,         /*bInterfaceNumber: Number of Interface*/
	0x00,         /*bAlternateSetting: Alternate setting*/
	0x02,         /*bNumEndpoints*/
	0xFF,         /*bInterfaceClass: HID*/
	0xFF,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	0xFF,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	0,            /*iInterface: Index of string descriptor*/

	0x07,          /*bLength: Endpoint Descriptor size*/
	USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
	RFLINK_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
	0x02,          /*bmAttributes: Bulk endpoint*/
	RFLINK_IN_EP_SIZE, /*wMaxPacketSize: 64 Bytes max */
	0x00,
	0x00,          /*bInterval: Polling Interval (10 ms)*/

	0x07,          /*bLength: Endpoint Descriptor size*/
	USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
	RFLINK_OUT_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
	0x02,          /*bmAttributes: Bulk endpoint*/
	RFLINK_OUT_EP_SIZE, /*wMaxPacketSize: 64 Bytes max */
	0x00,
	0x00,          /*bInterval: Polling Interval (10 ms)*/

};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB Standard Device Descriptor */
static uint8_t USBD_RfLink_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
	USB_LEN_DEV_QUALIFIER_DESC,
	USB_DESC_TYPE_DEVICE_QUALIFIER,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	0x40,
	0x01,
	0x00,
};

/**
 * @}
 */

/** @defgroup USBD_RfLink_Private_Functions
 * @{
 */

/**
 * @brief  USBD_RfLink_Init
 *         Initialize the RfLink interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t data_buf[RFLINK_OUT_EP_SIZE];
static int data_buf_len = 0;
static uint8_t  USBD_RfLink_Init (USBD_HandleTypeDef *pdev,
				uint8_t cfgidx)
{
	uint8_t ret = 0;
	USBD_LL_OpenEP(pdev,
		RFLINK_OUT_EP,
		USBD_EP_TYPE_BULK,
		RFLINK_OUT_EP_SIZE);

	USBD_LL_OpenEP(pdev,
		RFLINK_IN_EP,
		USBD_EP_TYPE_BULK,
		RFLINK_IN_EP_SIZE);

	/* Prepare Out endpoint to receive 1st packet */
	USBD_LL_PrepareReceive(pdev,
			RFLINK_OUT_EP,
			data_buf,
			RFLINK_OUT_EP_SIZE);

	return ret;
}

/**
 * @brief  USBD_RfLink_Init
 *         DeInitialize the RfLink layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t  USBD_RfLink_DeInit (USBD_HandleTypeDef *pdev,
				uint8_t cfgidx)
{

	USBD_LL_CloseEP(pdev, RFLINK_OUT_EP);
	USBD_LL_CloseEP(pdev, RFLINK_IN_EP);
	return USBD_OK;
}

/**
 * @brief  USBD_RfLink_Setup
 *         Handle the RfLink specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t  USBD_RfLink_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{

	switch (req->bmRequest & USB_REQ_TYPE_MASK)
	{
	case USB_REQ_TYPE_CLASS :
		switch (req->bRequest)
		{

		default:
			USBD_CtlError (pdev, req);
			return USBD_FAIL;
		}
		break;

	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest)
		{

		default:
			USBD_CtlError (pdev, req);
			return USBD_FAIL;
		}
	}
	return USBD_OK;
}


/**
 * @brief  USBD_RfLink_GetCfgDesc
 *         return configuration descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t  *USBD_RfLink_GetCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_RfLink_CfgDesc);
	return USBD_RfLink_CfgDesc;
}

/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
uint8_t  *USBD_RfLink_DeviceQualifierDescriptor (uint16_t *length)
{
	*length = sizeof (USBD_RfLink_DeviceQualifierDesc);
	return USBD_RfLink_DeviceQualifierDesc;
}


/**
 * @brief  USBD_RfLink_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t  USBD_RfLink_DataIn (USBD_HandleTypeDef *pdev,
				uint8_t epnum)
{

	/* Transmit next packet */
	data_buf_len = 0;
	USBD_LL_PrepareReceive(pdev,
			RFLINK_OUT_EP,
			data_buf,
			RFLINK_OUT_EP_SIZE);

	return USBD_OK;
}

/**
 * @brief  USBD_RfLink_EP0_RxReady
 *         handle EP0 Rx Ready event
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t  USBD_RfLink_EP0_RxReady (USBD_HandleTypeDef *pdev)
{

	return USBD_OK;
}
/**
 * @brief  USBD_RfLink_EP0_TxReady
 *         handle EP0 TRx Ready event
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t  USBD_RfLink_EP0_TxReady (USBD_HandleTypeDef *pdev)
{

	return USBD_OK;
}
/**
 * @brief  USBD_RfLink_SOF
 *         handle SOF event
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t  USBD_RfLink_SOF (USBD_HandleTypeDef *pdev)
{

	return USBD_OK;
}
/**
 * @brief  USBD_RfLink_IsoINIncomplete
 *         handle data ISO IN Incomplete event
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t  USBD_RfLink_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

	return USBD_OK;
}
/**
 * @brief  USBD_RfLink_IsoOutIncomplete
 *         handle data ISO OUT Incomplete event
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t  USBD_RfLink_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

	return USBD_OK;
}
/**
 * @brief  USBD_RfLink_DataOut
 *         handle data OUT Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t  USBD_RfLink_DataOut (USBD_HandleTypeDef *pdev,
				uint8_t epnum)
{
	int data_size = USBD_LL_GetRxDataSize (pdev, epnum);
	if (data_buf_len == 0)
	{
		data_buf_len = data_size;
		USBD_LL_Transmit(pdev,
				RFLINK_IN_EP,
				data_buf,
				data_size);
	}
	if (data_size == 0)
		USBD_LL_PrepareReceive(pdev,
				RFLINK_OUT_EP,
				data_buf,
				RFLINK_OUT_EP_SIZE);

	return USBD_OK;
}

/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
uint8_t  *USBD_RfLink_GetDeviceQualifierDesc (uint16_t *length)
{
	*length = sizeof (USBD_RfLink_DeviceQualifierDesc);
	return USBD_RfLink_DeviceQualifierDesc;
}

/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
