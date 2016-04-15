
/* Includes ------------------------------------------------------------------*/
#include "USBD_SP.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"



static uint8_t  USBD_SP_Init (USBD_HandleTypeDef *pdev,
                              uint8_t cfgidx);

static uint8_t  USBD_SP_DeInit (USBD_HandleTypeDef *pdev,
                                uint8_t cfgidx);

static uint8_t  USBD_SP_Setup (USBD_HandleTypeDef *pdev,
                               USBD_SetupReqTypedef *req);

static uint8_t  USBD_SP_DataIn (USBD_HandleTypeDef *pdev,
                                uint8_t epnum);

static uint8_t  USBD_SP_DataOut (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  USBD_SP_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  *USBD_SP_GetFSCfgDesc (uint16_t *length);

static uint8_t  *USBD_SP_GetHSCfgDesc (uint16_t *length);

static uint8_t  *USBD_SP_GetOtherSpeedCfgDesc (uint16_t *length);

static uint8_t  *USBD_SP_GetOtherSpeedCfgDesc (uint16_t *length);

uint8_t  *USBD_SP_GetDeviceQualifierDescriptor (uint16_t *length);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_SP_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
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

/** @defgroup USBD_SP_Private_Variables
 * @{
 */


/* SP interface class callbacks structure */
USBD_ClassTypeDef  USBD_SP =
{
	USBD_SP_Init,
	USBD_SP_DeInit,
	USBD_SP_Setup,
	NULL,                 /* EP0_TxSent, */
	USBD_SP_EP0_RxReady,
	USBD_SP_DataIn,
	USBD_SP_DataOut,
	NULL,
	NULL,
	NULL,
	USBD_SP_GetHSCfgDesc,
	USBD_SP_GetFSCfgDesc,
	USBD_SP_GetOtherSpeedCfgDesc,
	USBD_SP_GetDeviceQualifierDescriptor,
};

/* USB SP device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_SP_CfgHSDesc[USB_SP_CONFIG_DESC_SIZ] __ALIGN_END =
{
	/*Configuration Descriptor*/
	0x09,   /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
	USB_SP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
	0x00,
	0x01,   /* bNumInterfaces: 1 interface */
	0x01,   /* bConfigurationValue: Configuration value */
	0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
	0x00,   /* bmAttributes: bus powered */
	0xC8,   /* MaxPower 400 mA */

	/*---------------------------------------------------------------------------*/

	/*Interface Descriptor */
	0x09,   /* bLength: Interface Descriptor size */
	USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x00,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints: Two endpoints used */
	0xFF,   /* bInterfaceClass: Private Interface Class */
	0xFF,   /* bInterfaceSubClass: Private Subclass */
	0x01,   /* bInterfaceProtocol: Private protocol */
	0x00,   /* iInterface: */

	/*Endpoint 1 Descriptor*/
	0x07,                           /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	SP_IN_EP,                     /* bEndpointAddress */
	0x02,                           /* bmAttributes: Bulk */
	LOBYTE(SP_DATA_HS_MAX_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(SP_DATA_HS_MAX_PACKET_SIZE,
	0x00,                           /* bInterval: */

       	/*Endpoint 1 Descriptor*/
	0x07,                           /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	SP_OUT_EP,                     /* bEndpointAddress */
	0x02,                           /* bmAttributes: Bulk */
	LOBYTE(SP_DATA_HS_MAX_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(SP_DATA_HS_MAX_PACKET_SIZE,
	0x00,                           /* bInterval: */

	/*---------------------------------------------------------------------------*/
} ;


/* USB SP device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_SP_CfgFSDesc[USB_SP_CONFIG_DESC_SIZ] __ALIGN_END =
{
	/*Configuration Descriptor*/
	0x09,   /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
	USB_SP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
	0x00,
	0x01,   /* bNumInterfaces: 1 interface */
	0x01,   /* bConfigurationValue: Configuration value */
	0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
	0x00,   /* bmAttributes: bus powered */
	0xC8,   /* MaxPower 400 mA */

	/*---------------------------------------------------------------------------*/

	/*Interface Descriptor */
	0x09,   /* bLength: Interface Descriptor size */
	USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x00,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints: Two endpoints used */
	0xFF,   /* bInterfaceClass: Private Interface Class */
	0xFF,   /* bInterfaceSubClass: Private Subclass */
	0x01,   /* bInterfaceProtocol: Private protocol */
	0x00,   /* iInterface: */

	/*Endpoint 1 Descriptor*/
	0x07,                           /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	SP_IN_EP,                     /* bEndpointAddress */
	0x02,                           /* bmAttributes: Bulk */
	LOBYTE(SP_DATA_FS_MAX_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(SP_DATA_FS_MAX_PACKET_SIZE,
	0x00,                           /* bInterval: */

       	/*Endpoint 1 Descriptor*/
	0x07,                           /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	SP_OUT_EP,                     /* bEndpointAddress */
	0x02,                           /* bmAttributes: Bulk */
	LOBYTE(SP_DATA_FS_MAX_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(SP_DATA_FS_MAX_PACKET_SIZE,
	0x00,                           /* bInterval: */

	/*---------------------------------------------------------------------------*/
} ;

__ALIGN_BEGIN uint8_t USBD_SP_OtherSpeedCfgDesc[USB_SP_CONFIG_DESC_SIZ] __ALIGN_END =
{
		/*Configuration Descriptor*/
	0x09,   /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
	USB_SP_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
	0x00,
	0x01,   /* bNumInterfaces: 1 interface */
	0x01,   /* bConfigurationValue: Configuration value */
	0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
	0x00,   /* bmAttributes: bus powered */
	0xC8,   /* MaxPower 400 mA */

	/*---------------------------------------------------------------------------*/

	/*Interface Descriptor */
	0x09,   /* bLength: Interface Descriptor size */
	USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x00,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints: Two endpoints used */
	0xFF,   /* bInterfaceClass: Private Interface Class */
	0xFF,   /* bInterfaceSubClass: Private Subclass */
	0x01,   /* bInterfaceProtocol: Private protocol */
	0x00,   /* iInterface: */

	/*Endpoint 1 Descriptor*/
	0x07,                           /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	SP_IN_EP,                     /* bEndpointAddress */
	0x02,                           /* bmAttributes: Bulk */
	LOBYTE(SP_DATA_FS_MAX_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(SP_DATA_FS_MAX_PACKET_SIZE,
	0x00,                           /* bInterval: */

       	/*Endpoint 1 Descriptor*/
	0x07,                           /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	SP_OUT_EP,                     /* bEndpointAddress */
	0x02,                           /* bmAttributes: Bulk */
	LOBYTE(SP_DATA_FS_MAX_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(SP_DATA_FS_MAX_PACKET_SIZE,
	0x00,                           /* bInterval: */

	/*---------------------------------------------------------------------------*/
};

/**
 * @}
 */

/** @defgroup USBD_SP_Private_Functions
 * @{
 */

/**
 * @brief  USBD_SP_Init
 *         Initialize the SP interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t  USBD_SP_Init (USBD_HandleTypeDef *pdev,
                              uint8_t cfgidx)
{
	uint8_t ret = 0;
	USBD_SP_HandleTypeDef   *hcdc;

	if(pdev->dev_speed == USBD_SPEED_HIGH  )
	{
		/* Open EP IN */
		USBD_LL_OpenEP(pdev,
		               SP_IN_EP,
		               USBD_EP_TYPE_BULK,
		               SP_DATA_HS_IN_PACKET_SIZE);

		/* Open EP OUT */
		USBD_LL_OpenEP(pdev,
		               SP_OUT_EP,
		               USBD_EP_TYPE_BULK,
		               SP_DATA_HS_OUT_PACKET_SIZE);

	}
	else
	{
		/* Open EP IN */
		USBD_LL_OpenEP(pdev,
		               SP_IN_EP,
		               USBD_EP_TYPE_BULK,
		               SP_DATA_FS_IN_PACKET_SIZE);

		/* Open EP OUT */
		USBD_LL_OpenEP(pdev,
		               SP_OUT_EP,
		               USBD_EP_TYPE_BULK,
		               SP_DATA_FS_OUT_PACKET_SIZE);
	}

	pdev->pClassData = USBD_malloc(sizeof (USBD_SP_HandleTypeDef));

	if(pdev->pClassData == NULL)
	{
		ret = 1;
	}
	else
	{
		hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

		/* Init  physical Interface components */
		((USBD_SP_ItfTypeDef *)pdev->pUserData)->Init();

		/* Init Xfer states */
		hcdc->TxState =0;
		hcdc->RxState =0;

		if(pdev->dev_speed == USBD_SPEED_HIGH  )
		{
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
			                       SP_OUT_EP,
			                       hcdc->RxBuffer,
			                       SP_DATA_HS_OUT_PACKET_SIZE);
		}
		else
		{
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
			                       SP_OUT_EP,
			                       hcdc->RxBuffer,
			                       SP_DATA_FS_OUT_PACKET_SIZE);
		}


	}
	return ret;
}

/**
 * @brief  USBD_SP_Init
 *         DeInitialize the SP layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t  USBD_SP_DeInit (USBD_HandleTypeDef *pdev,
                                uint8_t cfgidx)
{
	uint8_t ret = 0;

	/* Close EP IN */
	USBD_LL_CloseEP(pdev,
	                SP_IN_EP);

	/* Close EP OUT */
	USBD_LL_CloseEP(pdev,
	                SP_OUT_EP);


	/* DeInit  physical Interface components */
	if(pdev->pClassData != NULL)
	{
		((USBD_SP_ItfTypeDef *)pdev->pUserData)->DeInit();
		USBD_free(pdev->pClassData);
		pdev->pClassData = NULL;
	}

	return ret;
}

/**
 * @brief  USBD_SP_Setup
 *         Handle the SP specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t  USBD_SP_Setup (USBD_HandleTypeDef *pdev,
                               USBD_SetupReqTypedef *req)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;
	static uint8_t ifalt = 0;

	switch (req->bmRequest & USB_REQ_TYPE_MASK)
	{
	case USB_REQ_TYPE_CLASS :
		if (req->wLength)
		{
			if (req->bmRequest & 0x80)
			{
				((USBD_SP_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest,
				                                                 (uint8_t *)hcdc->data,
				                                                 req->wLength);
				USBD_CtlSendData (pdev,
				                  (uint8_t *)hcdc->data,
				                  req->wLength);
			}
			else
			{
				hcdc->CmdOpCode = req->bRequest;
				hcdc->CmdLength = req->wLength;

				USBD_CtlPrepareRx (pdev,
				                   (uint8_t *)hcdc->data,
				                   req->wLength);
			}

		}
		else
		{
			((USBD_SP_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest,
			                                                 (uint8_t*)req,
			                                                 0);
		}
		break;

	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest)
		{
		case USB_REQ_GET_INTERFACE :
			USBD_CtlSendData (pdev,
			                  &ifalt,
			                  1);
			break;

		case USB_REQ_SET_INTERFACE :
			break;
		}

	default:
		break;
	}
	return USBD_OK;
}

/**
 * @brief  USBD_SP_DataIn
 *         Data sent on non-control IN endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t  USBD_SP_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	if(pdev->pClassData != NULL)
	{

		hcdc->TxState = 0;

		return USBD_OK;
	}
	else
	{
		return USBD_FAIL;
	}
}

/**
 * @brief  USBD_SP_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t  USBD_SP_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	/* Get the received data length */
	hcdc->RxLength = USBD_LL_GetRxDataSize (pdev, epnum);

	/* USB data will be immediately processed, this allow next USB traffic being
	   NAKed till the end of the application Xfer */
	if(pdev->pClassData != NULL)
	{
		((USBD_SP_ItfTypeDef *)pdev->pUserData)->Receive(hcdc->RxBuffer, &hcdc->RxLength);

		return USBD_OK;
	}
	else
	{
		return USBD_FAIL;
	}
}



/**
 * @brief  USBD_SP_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t  USBD_SP_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	if((pdev->pUserData != NULL) && (hcdc->CmdOpCode != 0xFF))
	{
		((USBD_SP_ItfTypeDef *)pdev->pUserData)->Control(hcdc->CmdOpCode,
		                                                 (uint8_t *)hcdc->data,
		                                                 hcdc->CmdLength);
		hcdc->CmdOpCode = 0xFF;

	}
	return USBD_OK;
}

/**
 * @brief  USBD_SP_GetFSCfgDesc
 *         Return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t  *USBD_SP_GetFSCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_SP_CfgFSDesc);
	return USBD_SP_CfgFSDesc;
}

/**
 * @brief  USBD_SP_GetHSCfgDesc
 *         Return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t  *USBD_SP_GetHSCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_SP_CfgHSDesc);
	return USBD_SP_CfgHSDesc;
}

/**
 * @brief  USBD_SP_GetCfgDesc
 *         Return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t  *USBD_SP_GetOtherSpeedCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_SP_OtherSpeedCfgDesc);
	return USBD_SP_OtherSpeedCfgDesc;
}

/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
uint8_t  *USBD_SP_GetDeviceQualifierDescriptor (uint16_t *length)
{
	*length = sizeof (USBD_SP_DeviceQualifierDesc);
	return USBD_SP_DeviceQualifierDesc;
}

/**
 * @brief  USBD_SP_RegisterInterface
 * @param  pdev: device instance
 * @param  fops: CD  Interface callback
 * @retval status
 */
uint8_t  USBD_SP_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                     USBD_SP_ItfTypeDef *fops)
{
	uint8_t  ret = USBD_FAIL;

	if(fops != NULL)
	{
		pdev->pUserData= fops;
		ret = USBD_OK;
	}

	return ret;
}

/**
 * @brief  USBD_SP_SetTxBuffer
 * @param  pdev: device instance
 * @param  pbuff: Tx Buffer
 * @retval status
 */
uint8_t  USBD_SP_SetTxBuffer  (USBD_HandleTypeDef   *pdev,
                               uint8_t  *pbuff,
                               uint16_t length)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	hcdc->TxBuffer = pbuff;
	hcdc->TxLength = length;

	return USBD_OK;
}


/**
 * @brief  USBD_SP_SetRxBuffer
 * @param  pdev: device instance
 * @param  pbuff: Rx Buffer
 * @retval status
 */
uint8_t  USBD_SP_SetRxBuffer  (USBD_HandleTypeDef   *pdev,
                               uint8_t  *pbuff)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	hcdc->RxBuffer = pbuff;

	return USBD_OK;
}

/**
 * @brief  USBD_SP_DataOut
 *         Data received on non-control Out endpoint
 * @param  pdev: device instance
 * @param  epnum: endpoint number
 * @retval status
 */
uint8_t  USBD_SP_TransmitPacket(USBD_HandleTypeDef *pdev)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	if(pdev->pClassData != NULL)
	{
		if(hcdc->TxState == 0)
		{
			/* Tx Transfer in progress */
			hcdc->TxState = 1;

			/* Transmit next packet */
			USBD_LL_Transmit(pdev,
			                 SP_IN_EP,
			                 hcdc->TxBuffer,
			                 hcdc->TxLength);

			return USBD_OK;
		}
		else
		{
			return USBD_BUSY;
		}
	}
	else
	{
		return USBD_FAIL;
	}
}


/**
 * @brief  USBD_SP_ReceivePacket
 *         prepare OUT Endpoint for reception
 * @param  pdev: device instance
 * @retval status
 */
uint8_t  USBD_SP_ReceivePacket(USBD_HandleTypeDef *pdev)
{
	USBD_SP_HandleTypeDef   *hcdc = (USBD_SP_HandleTypeDef*) pdev->pClassData;

	/* Suspend or Resume USB Out process */
	if(pdev->pClassData != NULL)
	{
		if(pdev->dev_speed == USBD_SPEED_HIGH  )
		{
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
			                       SP_OUT_EP,
			                       hcdc->RxBuffer,
			                       SP_DATA_HS_OUT_PACKET_SIZE);
		}
		else
		{
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev,
			                       SP_OUT_EP,
			                       hcdc->RxBuffer,
			                       SP_DATA_FS_OUT_PACKET_SIZE);
		}
		return USBD_OK;
	}
	else
	{
		return USBD_FAIL;
	}
}
