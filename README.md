# Smart Barrier System

A smart IoT-based access control system that automates barrier entry
using license plate validation and temporary QR codes. The system
integrates a Telegram bot, a Python QR scanning application, an
ESP32-CAM module, and an Arduino UNO to provide secure and automated
access management.

This project was developed as part of a microcontroller systems course
to demonstrate the integration of IoT hardware, computer vision, and
messaging platforms into a real-world automation solution.

------------------------------------------------------------------------

## Features

### QR-Based Access Control

-   Authorized users can generate temporary QR codes through a Telegram
    bot.
-   Each QR code contains:
    -   License plate
    -   Secret user code
    -   Timestamp
-   QR codes expire automatically after 30 seconds.

### Telegram Bot Authentication

-   Users authenticate using a personal secret code.
-   The bot validates the license plate and code combination.
-   Only authorized users can generate QR codes.

### Real-Time QR Scanning

-   A Python application captures video frames from the ESP32-CAM.
-   QR codes are detected and decoded using OpenCV and pyzbar.
-   If the QR code is valid, the system opens the barrier.

### Automated Barrier Control

-   The ESP32-CAM receives HTTP commands from the scanner application.
-   A servomotor opens the barrier for 10 seconds and then closes
    automatically.

### Automatic Exit Barrier

-   An Arduino UNO controls a second barrier using an HC-SR04 ultrasonic
    sensor.
-   If an object is detected within 10 cm, the exit barrier opens
    automatically.

### License Plate Database

Authorized vehicles are stored in a simple file:

plates.txt

Example:

B123XYZ,abc123 B456DEF,xyz789

------------------------------------------------------------------------

## Technology Stack

### Hardware

-   ESP32-CAM
-   Arduino UNO
-   OV2640 Camera
-   SG90 Servomotor
-   HC-SR04 Ultrasonic Sensor

### Software

-   Python
-   C / C++
-   Arduino IDE

### Libraries

-   OpenCV
-   Pyzbar
-   Tkinter
-   Python Telegram Bot
-   qrcode

------------------------------------------------------------------------

## System Architecture

The system is composed of four main components:

1.  Telegram Bot
2.  Python QR Scanner Application
3.  ESP32-CAM Camera Module
4.  Arduino Exit Barrier Controller

## System workflow:

1.  User authenticates with the Telegram bot.
2.  Bot generates a temporary QR code.
3.  User shows the QR code to the camera.
4.  Python application detects and validates the QR code.
5.  If valid, a command is sent to the ESP32-CAM.
6.  The ESP32-CAM opens the barrier.
7.  The barrier closes automatically after a delay.

------------------------------------------------------------------------

## Getting Started

### Prerequisites

-   Python 3.9+
-   Arduino IDE
-   Telegram Bot Token
-   ESP32-CAM module
-   Arduino UNO
-   Internet connection for the Telegram bot

------------------------------------------------------------------------

## Installation

1. Install Python Dependencies

pip install opencv-python numpy pyzbar pillow qrcode python-telegram-bot

------------------------------------------------------------------------

2. Configure the License Plate Database

Create a file named:

plates.txt

Example content:

B123XYZ,abc123 B456DEF,xyz789

------------------------------------------------------------------------

3. Setup the Telegram Bot

1.  Open Telegram and search for @BotFather
2.  Create a new bot
3.  Obtain the API Token
4.  Insert the token into:

start_bot.py

Run the bot:

python start_bot.py

------------------------------------------------------------------------

4. Start the QR Scanner Application

Run the camera scanning application:

python start_cam.py

The application will: - Read frames from the ESP32-CAM - Detect QR
codes - Validate access - Send barrier commands

------------------------------------------------------------------------

5. Upload Firmware to ESP32-CAM

Upload the ESP32 firmware using Arduino IDE.

After booting, the board will output the camera stream URL in the serial
monitor.

Example:

http://192.168.4.1/cam-hi.jpg

------------------------------------------------------------------------

6. Upload Arduino Exit Barrier Code

Upload the Arduino sketch to the Arduino UNO.

The ultrasonic sensor will automatically trigger the exit barrier when a
vehicle approaches.

------------------------------------------------------------------------

## Usage

1.  Start the Telegram bot.
2.  Start the QR scanning application.
3.  Power the ESP32-CAM and Arduino boards.
4.  Authenticate with the Telegram bot using your secret code.
5.  Generate a QR code using:

/getqr

6.  Show the QR code to the camera to open the barrier.

------------------------------------------------------------------------

## Limitations

-   Uses a text file instead of a database
-   No encryption for QR payload
-   Designed for local network usage
-   Limited scalability for large deployments

------------------------------------------------------------------------

## Future Improvements

Possible extensions include:

-   Web dashboard for administrators
-   Database integration (MySQL / PostgreSQL)
-   Access logging and analytics
-   License plate recognition
-   Mobile application
-   Encrypted QR tokens

License

This project was created for educational purposes as part of a
university course on microcontroller-based systems.
