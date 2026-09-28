# Custom RC Tank Controller

A high-level overview and controller codebase for a custom-built, Bluetooth-controlled tracked RC tank. 

This repository contains the core firmware for the **ESP32 microcontroller**, handling real-time Bluetooth communications with a PlayStation 4 controller to drive the tank via I2C motor commands.

---

## Project Demo

Watch a short overview of the build in action:

[![RC Tank Demo](https://img.youtube.com/vi/3niGtesECac/0.jpg)](https://youtube.com/shorts/3niGtesECac)

* **Video:** [Watch on YouTube Shorts](https://youtube.com/shorts/3niGtesECac)

---

## System Overview

The project combines heavy-duty hardware with onboard processing for wireless RC driving and real-time computer vision streaming:

* **Chassis & Motors:** Heavy-duty metal tank chassis with continuous tracks powered by high-torque DC encoder motors.
* **Low-Level Control:** **ESP32** microcontroller paired with a Hiwonder I2C motor driver for low-latency gamepad handling.
* **Gamepad Input:** Wireless PS4 DualShock controller connected via Bluetooth.
* **Onboard Intelligence:** **Raspberry Pi** managing video streaming, computer vision capabilities, and telemetry.
* **Gimbal & Optics:** 2-axis (pan/tilt) camera mount on top for real-time video feed.
