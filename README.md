<p align="center">
  <a href="https://www.softeralab.com/">
    <img src="docs/assets/images/en/logo.png" alt="Softera Lab" width="96">
  </a>
</p>

<h1 align="center">Hourglass</h1>

<p align="center"><strong>Softera Lab soldering kit — dual LED matrix · MPU-6050 · USB-C · LiPo</strong></p>

<p align="center">
  <a href="README.uk.md"><img alt="UA" src="https://img.shields.io/badge/UA-README.uk.md-F97316?style=flat-square"></a>
  <a href="https://www.softeralab.com/"><img alt="Website" src="https://img.shields.io/badge/softeralab.com-09090B?style=flat-square&labelColor=18181B"></a>
  <a href="https://www.instagram.com/softeralab/"><img alt="Instagram" src="https://img.shields.io/badge/Instagram-09090B?style=flat-square&labelColor=18181B"></a>
  <a href="https://www.youtube.com/@SofteraLab"><img alt="YouTube" src="https://img.shields.io/badge/YouTube-09090B?style=flat-square&labelColor=18181B"></a>
  <img alt="USB-C" src="https://img.shields.io/badge/USB--C-charge-09090B?style=flat-square&labelColor=F97316">
</p>

<p align="center"><strong>Languages:</strong> English (this page) · <a href="README.uk.md">Українська</a></p>

<p align="center">
  <img src="docs/assets/images/en/banner.png" alt="Hourglass solder kit" width="720">
</p>

**Hourglass** is a Softera Lab soldering kit: an electronic sand timer on a custom hourglass-shaped PCB with **two 8×8 LED matrices**, an **MPU-6050** motion sensor, **ATmega328P**, **USB-C** charging, and a **LiPo** cell. Flip the board — gravity reverses, and the “sand” falls the other way.

This repository is a **product page, assembly notes, and demo firmware**. It is **not open-source hardware** — Gerbers and manufacturing files are not published.

> © Softera Lab. All rights reserved.

## How it works

<p align="center">
  <img src="docs/assets/images/en/how-it-works.png" alt="How Hourglass works" width="720"><br>
  <em>Flip → MPU-6050 → ATmega328P → dual LED sand animation</em>
</p>

1. **Flip** the board to change orientation  
2. **MPU-6050** senses gravity  
3. **ATmega328P** runs the sand simulation  
4. **Dual 8×8 LED matrix** shows the falling sand  
5. **USB-C** charges the **LiPo**  
6. **Buzzer** + **SET** buttons — feedback and control  

## Inside the kit

<p align="center">
  <img src="docs/assets/images/en/kit-features.png" alt="Inside the kit" width="720"><br>
  <em>Dual matrix · ATmega328P · MPU-6050 · USB-C + LiPo · buzzer · Hourglass PCB</em>
</p>

| Zone | What you get |
| --- | --- |
| **Display** | 2 × **8×8** LED matrix (diamond layout) |
| **MCU** | **ATmega328P** + 16 MHz crystal |
| **Sensor** | **MPU-6050** (accel / gyro) |
| **Power** | **USB-C** charge · **LiPo** · On/Off |
| **I/O** | Piezo **buzzer** · **SET** buttons · **Reset** · **ICSP** |

## Flip & sand

<p align="center">
  <img src="docs/assets/images/en/flip.png" alt="Flip the board" width="720"><br>
  <em>Flip — sand follows gravity</em>
</p>

<p align="center">
  <img src="docs/assets/images/en/exp-filmstrip.png" alt="Sand timeline" width="720"><br>
  <em>Sand timeline — start → mid → done → flip</em>
</p>

<p align="center">
  <img src="docs/assets/images/en/mpu.png" alt="MPU-6050" width="720"><br>
  <em>MPU-6050 — orientation sensor</em>
</p>

## Assembly

<p align="center">
  <img src="docs/assets/images/en/assembly.png" alt="Assembly order" width="720"><br>
  <em>Assembly order — seven steps to first flip</em>
</p>

1. SMD passives + **ATmega328P**  
2. **USB-C** charge path  
3. Buzzer & switches  
4. LED matrices  
5. **MPU-6050**  
6. **LiPo** (**BAT+ / BAT−**)  
7. Flash firmware → flip  

Full notes: [docs/en/02-getting-started.md](docs/en/02-getting-started.md).

## Gallery

<p align="center">
  <img src="docs/assets/images/en/poster-desk.png" alt="Desk sculpture that codes" width="720"><br>
  <em>Desk sculpture that codes</em>
</p>

<p align="center">
  <img src="docs/assets/images/en/ed-desk.png" alt="DESK ICON MODE" width="720"><br>
  <em>DESK / ICON / MODE</em>
</p>

<p align="center">
  <img src="docs/assets/images/en/ed-bead.png" alt="BEAD BY BEAD" width="720"><br>
  <em>BEAD / BY / BEAD — one grain, one pixel</em>
</p>

## Firmware

Demo: [`firmware/HG_Hourglass/`](firmware/HG_Hourglass/) — gravity sand on two matrices + MPU-6050.

Pin defaults are in the sketch header — adjust **DIN / CS / CLK** if your PCB routing differs.

## Guides

| Guide | Link |
| --- | --- |
| Hardware overview | [docs/en/01-hardware-overview.md](docs/en/01-hardware-overview.md) |
| Getting started | [docs/en/02-getting-started.md](docs/en/02-getting-started.md) |
| Soldering | [docs/en/03-soldering.md](docs/en/03-soldering.md) |
| Troubleshooting | [docs/en/04-troubleshooting.md](docs/en/04-troubleshooting.md) |

## Links

| | |
| --- | --- |
| Website | [softeralab.com](https://www.softeralab.com/) |
| Contact | [Contacts](https://www.softeralab.com/our-contacts/) · support@softeralab.com |
| Instagram | [instagram.com/softeralab](https://www.instagram.com/softeralab/) |
| YouTube | [YouTube @SofteraLab](https://www.youtube.com/@SofteraLab) |

---

© Softera Lab. All rights reserved. See [COPYRIGHT.md](COPYRIGHT.md).
