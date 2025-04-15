 This documentation covers project setup, hardware connections, code functionality, and how to build and flash the firmware.

---

```markdown
# STM32 NAND Flash Interface via FSMC

This project demonstrates how to interface an external NAND flash memory with an STM32F4 series microcontroller using the FSMC peripheral and HAL drivers.

## 📌 Project Overview

- **MCU Used:** STM32F429 / STM32F4xx series  
- **Peripheral Used:** FSMC (Flexible Static Memory Controller)  
- **Memory Type:** NAND Flash (8-bit bus width)  
- **Flash Page Size:** 2048 bytes  
- **IDE:** STM32CubeIDE  
- **Purpose:** Erase, write, and verify data in external NAND flash memory

---

## 🔌 Hardware Requirements

| Component            | Details                          |
|---------------------|----------------------------------|
| STM32 Board         | STM32F429ZI or equivalent        |
| NAND Flash IC       | Compatible with FSMC, 8-bit bus  |
| ST-Link/V2 Debugger | For flashing and debugging       |
| Jumper Wires        | For external connections         |

### 📐 Pin Configuration (FSMC NAND)

> ⚠️ Double-check your specific NAND IC datasheet to confirm pin compatibility.

| STM32 Pin (Port) | Function              | NAND Flash Pin |
|------------------|-----------------------|----------------|
| PD14             | D0                    | IO0            |
| PD15             | D1                    | IO1            |
| PD0              | D2                    | IO2            |
| PD1              | D3                    | IO3            |
| PE7              | D4                    | IO4            |
| PE8              | D5                    | IO5            |
| PE9              | D6                    | IO6            |
| PE10             | D7                    | IO7            |
| PD4              | NOE (Read Enable)     | RE             |
| PD5              | NWE (Write Enable)    | WE             |
| PD7              | NE1 (Chip Enable)     | CE             |
| PD11             | ALE (Addr Latch)      | ALE            |
| PD12             | CLE (Command Latch)   | CLE            |
| PE6              | NWAIT (Optional)      | R/B#           |
| GND              | Ground                | GND            |
| 3.3V             | Power Supply          | VCC            |

---

## ⚙️ STM32CubeMX Configuration

### Peripherals:
- **FSMC**
  - NAND Bank 2
  - Bus width: 8-bit
  - ECC: Disabled
  - TCLR / TAR Setup: 1 cycle
- **GPIO**
  - Enable clocks for GPIOD and GPIOE
  - Configure all required FSMC NAND alternate functions

---

## 🧠 Code Highlights (`main.c`)

- `NAND_Test()` function handles:
  - Page erase using `HAL_NAND_Erase_Block()`
  - Page write using `HAL_NAND_Write_Page_8b()`
  - Page read using `HAL_NAND_Read_Page_8b()`
  - Verifying read vs written buffer

### Buffer Sizes
```c
uint8_t txBuffer[2048];  // Transmit buffer
uint8_t rxBuffer[2048];  // Receive buffer
```

### NAND Addressing
```c
nandAddress.Block = 0;
nandAddress.Page  = 0;
nandAddress.Plane = 0;
```

### FSMC Init Structure
```c
hnand1.Init.NandBank = FSMC_NAND_BANK2;
hnand1.Init.MemoryDataWidth = FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
...
hnand1.Config.PageSize = 2048;
hnand1.Config.BlockSize = 64;
hnand1.Config.BlockNbr = 2048;
```

---

## 🛠️ Build & Flash

1. Open project in **STM32CubeIDE**
2. Click **Project > Build**
3. Connect ST-Link and click **Run > Debug**
4. Observe NAND test status (toggle LED/UART can be added for feedback)

---

## 🔍 Debugging Tips

- Confirm NAND IC is powered with proper Vcc (usually 3.3V)
- Check if **FSMC pins** are not shared with other peripherals
- Use **logic analyzer** to verify signals on RE/WE/CE/ALE/CLE lines
- In case of failure, watch for entry into `Error_Handler()` loop

---

## 📎 References

- [STM32F4 Reference Manual (FSMC Section)](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405415-stm32f407417-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [Your NAND IC Datasheet] - Timing, Addressing, and Bus Config

---

## ✅ TODO / Improvements

- Add **LED/UART** indication for success/failure
- Expand to **multi-page write/read**
- Integrate **ECC feature** and error reporting
- Port to **FreeRTOS task** for multitasking NAND ops

---

## 🧑‍💻 Author

> Created by Akash Gaikwad  
> If you face issues or want enhancements, feel free to contribute or raise a pull request!

---

```