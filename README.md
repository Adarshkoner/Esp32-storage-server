ESP32 Mini Storage Server

​A compact wireless storage server built around an ESP32 microcontroller and a MicroSD card interface. Designed for localized, off-grid file management, this project provides a lightweight personal cloud accessible from any standard web browser without requiring external internet connectivity.
<img width="1024" height="559" alt="image_1f814417-adaa-4ba0-8e3e-a8756d33d2fd" src="https://github.com/user-attachments/assets/4ff1363c-938d-4e77-9829-e994514bbf3b" />

<img width="1312" height="1199" alt="1000069764" src="https://github.com/user-attachments/assets/6a4b51dd-e9fb-4100-a5b7-8f5d1fc15098" />

Overview

​This project transforms an ESP32 microcontroller into a portable, low-power network file server. The firmware hosts an web interface that allows connected smartphones, tablets, or computers to upload, browse, stream, and download files directly to an onboard MicroSD card via a dedicated local Wi-Fi network.

​Hardware Architecture

​Microcontroller: ESP32-WROOM module managing local HTTP traffic, DNS redirection, and Wi-Fi Access Point/Station modes.

​Storage Interface: Dedicated MicroSD socket routed over high-speed SPI with impedance-aware routing to eliminate bus crosstalk.

​Power Delivery: Onboard 3.3V low-dropout regulator supported by widened 25-mil power traces and decoupling capacitors to eliminate write-cycle brownout resets.

​Enclosure Design

​The system is housed in a custom 3D-printable enclosure tailored for thermal reliability and field usability:

​Integrated top-mount fan cutout for active cooling during sustained, multi-client read/write workloads.

​External push-push MicroSD slot for convenient hot-swapping without opening the case.

​Recessed USB Type-C port for universal 5V power input.

Quick Start

​Hardware Setup: Insert a FAT32-formatted MicroSD card into the slot and power the device via USB Type-C.

​Firmware: Open firmware/main.ino in Arduino IDE or PlatformIO, install the necessary ESPAsyncWebServer and FS libraries, and flash the board.

​Connect: Connect to the ESP32-Storage Wi-Fi network and navigate to [http://192.168.4.1](http://192.168.4.1) in your browser.

This project is still under development. If you think something need improvement feel free to say.
Hope this helps.
