/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-11     Wayne        First version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>

#define DBG_TAG "st1663i"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "st1663i.h"

static struct rt_i2c_client st1663i_client;
static void st1663i_touch_up(void *buf, rt_int8_t id);
static rt_err_t st1663i_write_reg(struct rt_i2c_client *dev, rt_uint8_t reg, rt_uint8_t value)
{
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = value;

    msgs.addr  = dev->client_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buf;
    msgs.len   = sizeof(buf);

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static rt_err_t st1663i_read_reg(struct rt_i2c_client *dev, rt_uint8_t reg, rt_uint8_t *data, rt_uint8_t len)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = ST_REGITER_LEN;

    msgs[1].addr  = dev->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = data;
    msgs[1].len   = len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static rt_int16_t pre_x[ST_MAX_TOUCH];
static rt_int16_t pre_y[ST_MAX_TOUCH];
static rt_int16_t pre_w[ST_MAX_TOUCH];
static rt_uint8_t s_tp_dowm[ST_MAX_TOUCH];

static void st1663i_touch_up(void *buf, rt_int8_t id)
{
    struct rt_touch_data *read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        s_tp_dowm[id] = 0;
        read_data[id].event = RT_TOUCH_EVENT_UP;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_NONE;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = pre_w[id];
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1;  /* last point is none */
    pre_y[id] = -1;
    pre_w[id] = -1;

    //LOG_I("%s (%d)\n", __func__, id);
}

static void st1663i_touch_down(void *buf, rt_int8_t id, rt_int16_t x, rt_int16_t y, rt_int16_t w)
{
    struct rt_touch_data *read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        read_data[id].event = RT_TOUCH_EVENT_MOVE;

    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_DOWN;
        s_tp_dowm[id] = 1;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
    pre_w[id] = w;

    //LOG_I("%s (%d %d %d %d)\n", __func__, id, x, y, w );
}

static int8_t pre_id[ST_MAX_TOUCH];
static S_ST_REGMAP sStRegMap;
static rt_uint8_t pre_touch = 0;

static rt_ssize_t st1663i_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    int i;

    rt_err_t error = 0;
    rt_int32_t   touch_event, touchid;

    RT_ASSERT(touch);
    RT_ASSERT(buf);
    RT_ASSERT(read_num != 0);
    RT_ASSERT(read_num <= ST_MAX_TOUCH);

    error = st1663i_read_reg(&st1663i_client, 0x10, (rt_uint8_t *)&sStRegMap, sizeof(sStRegMap));
    if (error)
    {
        LOG_E("get touch data failed, err:%d\n", error);
        goto exit_read_point;
    }

    if (sStRegMap.u8Fingers > ST_MAX_TOUCH)
    {
        LOG_E("FW report max point:%d > panel info. max:%d\n", sStRegMap.u8Fingers, ST_MAX_TOUCH);
        goto exit_read_point;
    }

    if (pre_touch > sStRegMap.u8Fingers)               /* point up */
    {
        for (i = 0; i < ST_MAX_TOUCH; i++)
        {
            rt_uint8_t j;
            for (j = 0; j < sStRegMap.u8Fingers; j++)  /* this time touch num */
            {
                touchid = i;

                if (pre_id[i] == touchid)                /* this id is not free */
                    break;
            }

            if ((j == sStRegMap.u8Fingers) && (pre_id[i] != -1))         /* free this node */
            {
                // LOG_I("free %d tid=%d\n", i, pre_id[i]);
                st1663i_touch_up(buf, pre_id[i]);
                pre_id[i] = -1;
            }
        }
    }

    for (i = 0; i < sStRegMap.u8Fingers; i++)
    {
        touch_event = sStRegMap.m_sTP[i].u8Valid;
        touchid = i;

        //LOG_I("(%d/%d) %d %d\n", i, sStRegMap.u8Fingers, touchid, touch_event);

        pre_id[i] = touchid;

        if (touch_event)
        {
            rt_uint16_t  x, y, w;

            x = ((uint16_t)sStRegMap.m_sTP[i].u8X0_H << 8) |  sStRegMap.m_sTP[i].m_u8X0_L;
            y = ((uint16_t)sStRegMap.m_sTP[i].u8Y0_H << 8) |  sStRegMap.m_sTP[i].m_u8Y0_L;
            w = sStRegMap.m_sTP[i].m_u8Z;

            //LOG_I("[%d] (%d %d %d %d)\n", touch_event, touchid, x, y, w);

            if (x >= touch->info.range_x || y >= touch->info.range_y)
            {
                LOG_E("invalid position, X[%d,%u,%d], Y[%d,%u,%d]\n",
                      0, x, touch->info.range_x,
                      0, y, touch->info.range_y);
                continue;
            }

            st1663i_touch_down(buf, touchid, x, y, w);
        }
        else
        {
            // Up
            st1663i_touch_up(buf, touchid);
        }

    } // for (i = 0; i < sStRegMap.u8TDStatus; i++)

    pre_touch = sStRegMap.u8Fingers;

    return read_num;

exit_read_point:

    pre_touch = 0;

    return 0;
}

static rt_err_t st1663i_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    switch (cmd)
    {
    case RT_TOUCH_CTRL_GET_INFO:
    {
        struct rt_touch_info *info = (struct rt_touch_info *)arg;
        RT_ASSERT(arg);

        rt_memcpy(info, &touch->info, sizeof(struct rt_touch_info));
        break;
    }
    case RT_TOUCH_CTRL_GET_ID:
        break;
    case RT_TOUCH_CTRL_SET_X_RANGE:
        break;
    case RT_TOUCH_CTRL_SET_Y_RANGE:
        break;
    case RT_TOUCH_CTRL_SET_X_TO_Y:
        break;
    case RT_TOUCH_CTRL_SET_MODE:
        break;
    default:
        break;
    }
    return RT_EOK;
}

static struct rt_touch_ops st1663i_touch_ops =
{
    .touch_readpoint = st1663i_read_point,
    .touch_control = st1663i_control,
};

static void st1663i_init(struct rt_i2c_client *dev)
{
    st1663i_write_reg(dev, 0x0, 0);
}

int rt_hw_st1663i_init(const char *name, struct rt_touch_config *cfg)
{
    struct rt_touch_device *touch_device = RT_NULL;
    rt_uint32_t bus_speed = 400000;

    touch_device = (struct rt_touch_device *)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_device == RT_NULL)
    {
        LOG_E("touch device malloc fail");
        return -RT_ERROR;
    }
    rt_memset((void *)touch_device, 0, sizeof(struct rt_touch_device));

    /* hw init*/
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);

    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_LOW);
    rt_thread_delay(5);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_HIGH);
    rt_thread_delay(200);

    rt_pin_mode(cfg->irq_pin.pin, cfg->irq_pin.mode);

    st1663i_client.bus = (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (st1663i_client.bus == RT_NULL)
    {
        LOG_E("Can't find %s device", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)st1663i_client.bus, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_control((rt_device_t)st1663i_client.bus, RT_I2C_DEV_CTRL_CLK, &bus_speed) != RT_EOK)
    {
        LOG_E("control %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    st1663i_client.client_addr = ST1663I_ADDRESS;

    st1663i_init(&st1663i_client);

    rt_memset(&pre_x[0], 0xff,  ST_MAX_TOUCH * sizeof(rt_int16_t));
    rt_memset(&pre_y[0], 0xff,  ST_MAX_TOUCH * sizeof(rt_int16_t));
    rt_memset(&pre_w[0], 0xff,  ST_MAX_TOUCH * sizeof(rt_int16_t));
    rt_memset(&s_tp_dowm[0], 0, ST_MAX_TOUCH * sizeof(rt_int8_t));
    rt_memset(&pre_id[0], 0xff,  ST_MAX_TOUCH * sizeof(rt_int8_t));

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_UNKNOWN;
    touch_device->info.range_x = BSP_LCD_WIDTH;
    touch_device->info.range_y = BSP_LCD_HEIGHT;
    touch_device->info.point_num = ST_MAX_TOUCH;

    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &st1663i_touch_ops;

    rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    LOG_I("touch device st1663i init success");

    return RT_EOK;
}
