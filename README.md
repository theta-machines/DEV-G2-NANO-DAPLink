# DAPLink Port for the G2 Nano

This repository is a fork of the open-source [DAPLink project](https://github.com/ARMmbed/DAPLink), building on the [v0257 release](https://github.com/ARMmbed/DAPLink/releases/tag/v0257). It contains board-specific configuration files and subtle firmware changes specific to the G2 Nano development board.

This repository is maintained independently and has no affiliation with the original project.



## How to Build
1. Install version `10.3-2021.10` of the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm).
    * Note that later versions are known to require changes. See [this issue](https://github.com/ARMmbed/DAPLink/pull/1043) for details.

    * Verify that your PATH variable include's the tool's location with
        ```
        arm-none-eabi-gcc --version
        ```

2. If on Windows, install Make with
    ```
    winget install --exact --id GnuWin32.Make
    ```
    * Verify that your PATH variable include's the tool's location with
        ```
        make --version
        ```

3. If on Windows, install the `env` command with
    ```
    winget install --exact --id Microsoft.Coreutils
    ```
    * Verify that your PATH variable include's the tool's location with
        ```
        env --version
        ```

4. Create a Python virtual environment with
    ```
    python -m venv .venv
    ```

5. Activate the Python virtual environment. On Windows, run
    ```
    .\.venv\Scripts\Activate.ps1
    ```

6. Install Python dependencies with
    ```
    pip install -r requirements.txt
    ```

7. Build with
    ```
    python tools/progen_compile.py -t make_gcc_arm --clean --parallel kl26z_bl kl26z_g2_nano_if
    ```



## How to Flash the HIC
These instructions assume that you're using the [MCULink Pro Debug Probe](https://www.nxp.com/design/design-center/software/software-library/mcu-link-pro-debug-probe:MCU-LINK-PRO).

The diagram below illustrates the HIC's SWD pins. You must also connect the probe's ground pin to one of the G2 Nano's ground pins.

```
BOTTOM VIEW

+--------------------------------------------------+
|                                                  |
|                                    [1] [2]       |
|                                      [3]         |
|                                                  |
+--------------------------------------------------+

1. RESET_B (optional)
2. SWD_CLK
3. SWD_DIO
```

1. Install [LinkServer](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/linkserver-for-microcontrollers:LINKERSERVER).

2. Connect the `SWD_CLK`, `SWD_DIO`, and `GND` pins. The `RESET_B` pin is optional.

3. Use the LinkFlash GUI tool and program the HIC with the two `*.hex` files
    * `projectfiles/make_gcc_arm/kl26z_bl/build/kl26z_bl_crc.hex`
    * `projectfiles/make_gcc_arm/kl26z_if/build/kl26z_if_crc.hex`
