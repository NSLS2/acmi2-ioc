# ACMI-2 — Accelerator Charge Monitor Interlock

## Overview

The **ACMI-2 (Accelerator Charge Monitor Interlock)** is a radiation safety device that enforces injected beam charge limits as defined in the **Accelerator Safety Envelope (ASE)**.

The system continuously monitors the accelerated beam charge from the **NSLS-II electron gun** and provides a fast, hardware-enforced interlock to prevent unsafe operating conditions.

---

## Functionality

The ACMI-2 system performs the following safety functions:

- Monitors **average beam current** derived from the injected charge
- Compares measured values against **preset safety limits**
- Detects additional fault conditions related to beam delivery or system health
- **Generates a fault** when a safety violation is detected

When a fault condition occurs, the ACMI-2 **disables the electron gun**, preventing further beam injection and ensuring compliance with radiation safety requirements.

---

## Safety Role

ACMI-2 is a **personnel and radiation safety system** component and operates independently of normal machine control software. Its behavior and limits are governed by approved accelerator safety documentation and configuration controls.

---

## Repository Contents

This repository contains the **IOC software, configuration, and support files** required to operate and maintain the ACMI-2 system.

---

## Notes

- Changes to ACMI-2 logic, limits, or behavior must follow applicable **NSLS-II safety review and approval processes**
- This system is not intended for routine beam tuning or performance optimization

---

## Contact

For questions related to ACMI-2 operation, configuration, or safety role, contact Tony Caracappa.

