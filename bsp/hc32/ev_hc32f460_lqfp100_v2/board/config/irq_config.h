/*
 * Copyright (C) 2022-2024, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#ifndef __IRQ_CONFIG_H__
#define __IRQ_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_EXTINT0_IRQ_NUM             INT022_IRQn
#define BSP_EXTINT0_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT1_IRQ_NUM             INT023_IRQn
#define BSP_EXTINT1_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT2_IRQ_NUM             INT024_IRQn
#define BSP_EXTINT2_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT3_IRQ_NUM             INT025_IRQn
#define BSP_EXTINT3_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT4_IRQ_NUM             INT026_IRQn
#define BSP_EXTINT4_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT5_IRQ_NUM             INT027_IRQn
#define BSP_EXTINT5_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT6_IRQ_NUM             INT028_IRQn
#define BSP_EXTINT6_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT7_IRQ_NUM             INT029_IRQn
#define BSP_EXTINT7_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT8_IRQ_NUM             INT030_IRQn
#define BSP_EXTINT8_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT9_IRQ_NUM             INT031_IRQn
#define BSP_EXTINT9_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT10_IRQ_NUM            INT032_IRQn
#define BSP_EXTINT10_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT11_IRQ_NUM            INT033_IRQn
#define BSP_EXTINT11_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT12_IRQ_NUM            INT034_IRQn
#define BSP_EXTINT12_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT13_IRQ_NUM            INT035_IRQn
#define BSP_EXTINT13_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT14_IRQ_NUM            INT036_IRQn
#define BSP_EXTINT14_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT15_IRQ_NUM            INT037_IRQn
#define BSP_EXTINT15_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

/* DMA1 ch0 */
#define BSP_DMA1_CH0_IRQ_NUM            INT038_IRQn
#define BSP_DMA1_CH0_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch1 */
#define BSP_DMA1_CH1_IRQ_NUM            INT039_IRQn
#define BSP_DMA1_CH1_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch2 */
#define BSP_DMA1_CH2_IRQ_NUM            INT040_IRQn
#define BSP_DMA1_CH2_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch3 */
#define BSP_DMA1_CH3_IRQ_NUM            INT041_IRQn
#define BSP_DMA1_CH3_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

/* DMA2 ch0 */
#define BSP_DMA2_CH0_IRQ_NUM            INT042_IRQn
#define BSP_DMA2_CH0_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch1 */
#define BSP_DMA2_CH1_IRQ_NUM            INT043_IRQn
#define BSP_DMA2_CH1_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch2 */
#define BSP_DMA2_CH2_IRQ_NUM            INT020_IRQn
#define BSP_DMA2_CH2_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch3 */
#define BSP_DMA2_CH3_IRQ_NUM            INT021_IRQn
#define BSP_DMA2_CH3_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_USING_UART1)
#define BSP_UART1_RXERR_IRQ_NUM         INT012_IRQn
#define BSP_UART1_RXERR_IRQ_PRIO        DDL_IRQ_PRIO_DEFAULT
#define BSP_UART1_RX_IRQ_NUM            INT082_IRQn
#define BSP_UART1_RX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_UART1_TX_IRQ_NUM            INT081_IRQn
#define BSP_UART1_TX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART1_RX_USING_DMA)
#define BSP_UART1_RXTO_IRQ_NUM          INT013_IRQn
#define BSP_UART1_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(RT_USING_SERIAL_V1) && defined(BSP_UART1_TX_USING_DMA)
#define BSP_UART1_TX_CPLT_IRQ_NUM       INT080_IRQn
#define BSP_UART1_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#elif defined(RT_USING_SERIAL_V2)
#define BSP_UART1_TX_CPLT_IRQ_NUM       INT080_IRQn
#define BSP_UART1_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
#define BSP_UART2_RXERR_IRQ_NUM         INT014_IRQn
#define BSP_UART2_RXERR_IRQ_PRIO        DDL_IRQ_PRIO_DEFAULT
#define BSP_UART2_RX_IRQ_NUM            INT085_IRQn
#define BSP_UART2_RX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_UART2_TX_IRQ_NUM            INT084_IRQn
#define BSP_UART2_TX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART2_RX_USING_DMA)
#define BSP_UART2_RXTO_IRQ_NUM          INT015_IRQn
#define BSP_UART2_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(RT_USING_SERIAL_V1) && defined(BSP_UART2_TX_USING_DMA)
#define BSP_UART2_TX_CPLT_IRQ_NUM       INT083_IRQn
#define BSP_UART2_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#elif defined(RT_USING_SERIAL_V2)
#define BSP_UART2_TX_CPLT_IRQ_NUM       INT083_IRQn
#define BSP_UART2_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
#define BSP_UART3_RXERR_IRQ_NUM         INT016_IRQn
#define BSP_UART3_RXERR_IRQ_PRIO        DDL_IRQ_PRIO_DEFAULT
#define BSP_UART3_RX_IRQ_NUM            INT088_IRQn
#define BSP_UART3_RX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_UART3_TX_IRQ_NUM            INT087_IRQn
#define BSP_UART3_TX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART3_RX_USING_DMA)
#define BSP_UART3_RXTO_IRQ_NUM          INT017_IRQn
#define BSP_UART3_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(RT_USING_SERIAL_V1) && defined(BSP_UART3_TX_USING_DMA)
#define BSP_UART3_TX_CPLT_IRQ_NUM       INT086_IRQn
#define BSP_UART3_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#elif defined(RT_USING_SERIAL_V2)
#define BSP_UART3_TX_CPLT_IRQ_NUM       INT086_IRQn
#define BSP_UART3_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
#define BSP_UART4_RXERR_IRQ_NUM         INT018_IRQn
#define BSP_UART4_RXERR_IRQ_PRIO        DDL_IRQ_PRIO_DEFAULT
#define BSP_UART4_RX_IRQ_NUM            INT091_IRQn
#define BSP_UART4_RX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_UART4_TX_IRQ_NUM            INT090_IRQn
#define BSP_UART4_TX_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART4_RX_USING_DMA)
#define BSP_UART4_RXTO_IRQ_NUM          INT019_IRQn
#define BSP_UART4_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(RT_USING_SERIAL_V1) && defined(BSP_UART4_TX_USING_DMA)
#define BSP_UART4_TX_CPLT_IRQ_NUM       INT089_IRQn
#define BSP_UART4_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#elif defined(RT_USING_SERIAL_V2)
#define BSP_UART4_TX_CPLT_IRQ_NUM       INT089_IRQn
#define BSP_UART4_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART4 */

#if defined(BSP_USING_SPI1)
#define BSP_SPI1_ERR_IRQ_NUM            INT008_IRQn
#define BSP_SPI1_ERR_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(BSP_USING_SPI2)
#define BSP_SPI2_ERR_IRQ_NUM            INT009_IRQn
#define BSP_SPI2_ERR_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(BSP_USING_SPI3)
#define BSP_SPI3_ERR_IRQ_NUM            INT010_IRQn
#define BSP_SPI3_ERR_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(BSP_USING_SPI4)
#define BSP_SPI4_ERR_IRQ_NUM            INT011_IRQn
#define BSP_SPI4_ERR_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#endif

#if defined(BSP_USING_CAN1)
#define BSP_CAN1_IRQ_NUM                INT126_IRQn
#define BSP_CAN1_IRQ_PRIO               DDL_IRQ_PRIO_DEFAULT
#endif /* BSP_USING_CAN1 */

#if defined(BSP_USING_SDIO1)
#define BSP_SDIO1_IRQ_NUM               INT122_IRQn
#define BSP_SDIO1_IRQ_PRIO              DDL_IRQ_PRIO_DEFAULT
#endif /* BSP_USING_SDIO1 */

#if defined(BSP_USING_SDIO2)
#define BSP_SDIO2_IRQ_NUM               INT124_IRQn
#define BSP_SDIO2_IRQ_PRIO              DDL_IRQ_PRIO_DEFAULT
#endif /* BSP_USING_SDIO2 */

#if defined(RT_USING_ALARM)
#define BSP_RTC_ALARM_IRQ_NUM           INT044_IRQn
#define BSP_RTC_ALARM_IRQ_PRIO          DDL_IRQ_PRIO_DEFAULT
#endif/* RT_USING_ALARM */

#if defined(BSP_USING_USBD) || defined(BSP_USING_USBH)
#define BSP_USB_GLB_IRQ_NUM             INT003_IRQn
#define BSP_USB_GLB_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_USBD */

#if defined (BSP_USING_QSPI)
#define BSP_QSPI_ERR_IRQ_NUM            INT004_IRQn
#define BSP_QSPI_ERR_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#endif /* BSP_USING_QSPI */

#if defined(BSP_USING_PULSE_ENCODER_TMRA_1)
#define BSP_PULSE_ENCODER_TMRA_1_OVF_IRQ_NUM   INT080_IRQn
#define BSP_PULSE_ENCODER_TMRA_1_OVF_IRQ_PRIO  DDL_IRQ_PRIO_DEFAULT
#define BSP_PULSE_ENCODER_TMRA_1_UDF_IRQ_NUM   INT081_IRQn
#define BSP_PULSE_ENCODER_TMRA_1_UDF_IRQ_PRIO  DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_PULSE_ENCODER_TMRA_1 */

#if defined(BSP_USING_PULSE_ENCODER_TMR6_1)
#define BSP_PULSE_ENCODER_TMR6_1_OVF_IRQ_NUM   INT050_IRQn
#define BSP_PULSE_ENCODER_TMR6_1_OVF_IRQ_PRIO  DDL_IRQ_PRIO_DEFAULT
#define BSP_PULSE_ENCODER_TMR6_1_UDF_IRQ_NUM   INT051_IRQn
#define BSP_PULSE_ENCODER_TMR6_1_UDF_IRQ_PRIO  DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_PULSE_ENCODER_TMR6_1 */

#if defined(BSP_USING_TMR0_1A)
#define BSP_USING_TMR0_1A_IRQ_NUM       INT046_IRQn
#define BSP_USING_TMR0_1A_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_TMR0_1A */
#if defined(BSP_USING_TMR0_1B)
#define BSP_USING_TMR0_1B_IRQ_NUM       INT047_IRQn
#define BSP_USING_TMR0_1B_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_TMR0_1B */
#if defined(BSP_USING_TMR0_2A)
#define BSP_USING_TMR0_2A_IRQ_NUM       INT048_IRQn
#define BSP_USING_TMR0_2A_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_TMR0_2A */
#if defined(BSP_USING_TMR0_2B)
#define BSP_USING_TMR0_2B_IRQ_NUM       INT049_IRQn
#define BSP_USING_TMR0_2B_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_TMR0_2B */

#ifdef __cplusplus
}
#endif

#endif /* __IRQ_CONFIG_H__ */
