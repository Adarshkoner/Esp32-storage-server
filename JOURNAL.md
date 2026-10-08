Project Journal: ESP32 Mini Storage Server
​Project Overview
​Project Name: ESP32 Local File Storage Server
​Target Program: Hack Life by Hack Club
​Goal: Build an ultra-low-power, portable local storage device powered by an ESP32 micro-controller that hosts a web interface allowing devices (mobile, tablet, PC) to upload, download, and manage files over Wi-Fi using an SD card module.
​Log Entries
​Entry 1: Conceptualization & Hardware Selection
​Date: Day 1
​Focus: Architecture Planning
​Notes:
​Selected ESP32 (ESP-WROOM-32) due to integrated Wi-Fi capabilities, sufficient RAM for HTTP streaming, and low cost.
​Selected MicroSD SPI Module for storage media. SPI mode is preferred for simple PCB routing and compatibility across standard ESP32 boards.
​Formatted SD card to FAT32 filesystem to ensure universal read/write compatibility via FFat/SD.h libraries.
​Entry 2: Circuit Schematic & Pin Assignment
​Date: Day 2
​Focus: Pin Mapping & Power Budget
​Notes:
​SPI Pin Configuration chosen:
​MISO: GPIO 19
​MOSI: GPIO 23
​SCK: GPIO 18
​CS: GPIO 5
​Power consideration: SD cards require peak currents up to 100–200mA during write bursts. Included a 10uF bypass capacitor on the 3.3V power line to smooth out voltage dips.
​Entry 3: Firmware Development & Web UI
​Date: Day 3
​Focus: WebServer & File Operations
​Notes:
​Built an asynchronous/lightweight HTTP web server running on Port 80.
​Created REST-like endpoints:
​GET / -> Serves interactive HTML dashboard showing file list and upload form.
​GET /download?file=... -> Streams file from SD card to client.
​POST /upload -> Handles multipart form data for uploading files to SD card.
​GET /delete?file=... -> Deletes a specified file.
​Added Access Point (AP) mode capability so the server works standalone anywhere without an external router.
​Entry 4: Hardware Assembly & Testing
​Date: Day 4
​Focus: Breadboard Prototype & Verification
​Notes:
​Breadboarded components and wired the SPI bus.
​Tested upload speeds: achieved ~150-250 KB/s transfer rates over Wi-Fi AP mode.
​Verified file persistence across power cycles.
​Reflection & Next Steps
​Challenges: ESP32 memory constraints required buffer-based chunked streaming during downloads and uploads to prevent out-of-memory (OOM) crashes.
​Future Upgrades:
​Add captive portal functionality to automatically pop up the file manager upon connecting to Wi-Fi.
​Transition from standard SPI to SDMMC 4-bit mode for higher transfer speeds on custom PCB.
