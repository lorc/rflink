
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_SP_H
#define __USB_SP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

#define SP_IN_EP                                   0x81  /* EP1 for data IN */
#define SP_OUT_EP                                  0x01  /* EP1 for data OUT */

/* SP Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define SP_DATA_HS_MAX_PACKET_SIZE                 512  /* Endpoint IN & OUT Packet size */
#define SP_DATA_FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */
#define SP_CMD_PACKET_SIZE                         8  /* Control Endpoint Packet size */

#define USB_SP_CONFIG_DESC_SIZ                     67

/*---------------------------------------------------------------------*/
/*  SP definitions                                                    */
/*---------------------------------------------------------------------*/
#define SP_SEND_ENCAPSULATED_COMMAND               0x00
#define SP_GET_ENCAPSULATED_RESPONSE               0x01
#define SP_SET_COMM_FEATURE                        0x02
#define SP_GET_COMM_FEATURE                        0x03
#define SP_CLEAR_COMM_FEATURE                      0x04
#define SP_SET_LINE_CODING                         0x20
#define SP_GET_LINE_CODING                         0x21
#define SP_SET_CONTROL_LINE_STATE                  0x22
#define SP_SEND_BREAK                              0x23

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}USBD_SP_LineCodingTypeDef;

typedef struct _USBD_SP_Itf
{
  int8_t (* Init)          (void);
  int8_t (* DeInit)        (void);
  int8_t (* Control)       (uint8_t, uint8_t * , uint16_t);
  int8_t (* Receive)       (uint8_t *, uint32_t *);

}USBD_SP_ItfTypeDef;


typedef struct
{
  uint32_t data[SP_DATA_HS_MAX_PACKET_SIZE/4];      /* Force 32bits alignment */
  uint8_t  CmdOpCode;
  uint8_t  CmdLength;
  uint8_t  *RxBuffer;
  uint8_t  *TxBuffer;
  uint32_t RxLength;
  uint32_t TxLength;

  __IO uint32_t TxState;
  __IO uint32_t RxState;
}
USBD_SP_HandleTypeDef;



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_SP;
#define USBD_SP_CLASS    &USBD_SP
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t  USBD_SP_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                      USBD_SP_ItfTypeDef *fops);

uint8_t  USBD_SP_SetTxBuffer        (USBD_HandleTypeDef   *pdev,
                                      uint8_t  *pbuff,
                                      uint16_t length);

uint8_t  USBD_SP_SetRxBuffer        (USBD_HandleTypeDef   *pdev,
                                      uint8_t  *pbuff);

uint8_t  USBD_SP_ReceivePacket      (USBD_HandleTypeDef *pdev);

uint8_t  USBD_SP_TransmitPacket     (USBD_HandleTypeDef *pdev);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_SP_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
