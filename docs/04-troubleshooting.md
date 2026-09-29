---
id: troubleshooting
title: Пошук несправностей
sidebar_position: 4
description: Типові проблеми Hourglass.
---

# Пошук несправностей

| Симптом | Що перевірити |
| --- | --- |
| Немає LED | On/Off · LiPo / USB · піни DIN/CS/CLK у прошивці |
| Пісок не реагує на переворот | Посадка MPU-6050 · I2C · адреса 0x68 |
| Лише одна матриця | Пайка daisy-chain · `NUM_DEVICES = 2` |
| Плата гріється | Зніміть живлення · містки на МК / USB-C |
| Не заряджає | Кабель USB-C · CHARGE · полярність батареї |

![MPU-6050](./assets/images/uk/mpu.png)
