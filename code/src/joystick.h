/**
 * @file joystick.h
 * @brief Header file for the joystick.
 * 
 * Contains the variables and functions for the joysticks.
 */


#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include <USBHost_t36.h>

struct JoystickData {
  int joystickNumber;
  bool buttonData[10]; // X A B Y L R 0 0 Select Start
  int axisDataX;
  int axisDataY;
};

extern JoystickData joystickData[4];

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHIDParser hid1(myusb);
USBHIDParser hid2(myusb);
USBHIDParser hid3(myusb);
USBHIDParser hid4(myusb);

void handleUsb();


#define COUNT_JOYSTICKS 4
uint8_t joystickButtons[COUNT_JOYSTICKS];

void printJoystickData(const JoystickData& data);

JoystickController joysticks[COUNT_JOYSTICKS] = {
  JoystickController(myusb), JoystickController(myusb),
  JoystickController(myusb), JoystickController(myusb)
};
int user_axis[64];
uint32_t buttons_prev = 0;

USBDriver *drivers[] = {&hub1, &joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3], &hid1};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "joystick[0D]", "joystick[1D]", "joystick[2D]", "joystick[3D]",  "HID1"};
bool driver_active[CNT_DEVICES] = {false, false, false, false};


USBHIDInput *hiddrivers[] = {&joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3]};
#define CNT_HIDDEVICES (sizeof(hiddrivers)/sizeof(hiddrivers[0]))
const char * hid_driver_names[CNT_DEVICES] = {"joystick[0H]", "joystick[1H]", "joystick[2H]", "joystick[3H]"};
bool hid_driver_active[CNT_DEVICES] = {false};
bool show_changed_only = false;

uint8_t joystick_left_trigger_value[COUNT_JOYSTICKS] = {0};
uint8_t joystick_right_trigger_value[COUNT_JOYSTICKS] = {0};
uint64_t joystick_full_notify_mask = (uint64_t) - 1;

int psAxis[64];

void printJoystickData(const JoystickData& data);
void PrintDeviceListChanges();
void handleUsb();





// print the joystick data to the serial monitor for debugging formated into a table
void printJoystickData(const JoystickData& data) {
  Serial.print("Joystick ");
  Serial.print(data.joystickNumber);
  Serial.print(": ");
  for (int i = 0; i < 10; i++) {
    Serial.print(data.buttonData[i]);
    Serial.print(" ");
  }
  Serial.print("  ");
  Serial.print(data.axisDataX);
  Serial.print(" ");
  Serial.print(data.axisDataY);
  Serial.println();
}











void handleUsb()
{
  myusb.Task();
  PrintDeviceListChanges();

  if (Serial.available()) {
    int ch = Serial.read(); // get the first char.
    while (Serial.read() != -1) ;
    if ((ch == 'b') || (ch == 'B')) {
      Serial.println("Only notify on Basic Axis changes");
      for (int joystick_index = 0; joystick_index < COUNT_JOYSTICKS; joystick_index++)
        joysticks[joystick_index].axisChangeNotifyMask(0x3ff);
    } else if ((ch == 'f') || (ch == 'F')) {
      Serial.println("Only notify on Full Axis changes");
      for (int joystick_index = 0; joystick_index < COUNT_JOYSTICKS; joystick_index++)
        joysticks[joystick_index].axisChangeNotifyMask(joystick_full_notify_mask);

    } else {
      if (show_changed_only) {
        show_changed_only = false;
        Serial.println("\n*** Show All fields mode ***");
      } else {
        show_changed_only = true;
        Serial.println("\n*** Show only changed fields mode ***");
      }
    }
  }

  for (int joystick_index = 0; joystick_index < COUNT_JOYSTICKS; joystick_index++) {
    if (joysticks[joystick_index].available()) {
        joystickData[joystick_index].joystickNumber = joystick_index;

        uint32_t buttons = joysticks[joystick_index].getButtons();
        for (int i = 0; i < 10; i++) {
            joystickData[joystick_index].buttonData[i] = (buttons >> i) & 1;
        }

        uint64_t axis_mask = joysticks[joystick_index].axisMask();
        for (uint8_t i = 0; axis_mask != 0; i++, axis_mask >>= 1) {
            if (axis_mask & 1) {
                if (i == 0) {
                    joystickData[joystick_index].axisDataX = joysticks[joystick_index].getAxis(i);
                } else if (i == 1) {
                    joystickData[joystick_index].axisDataY = joysticks[joystick_index].getAxis(i);
                }
            }
        }

        joysticks[joystick_index].joystickDataClear();
    }
}

}


void PrintDeviceListChanges() {
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {
    if (*drivers[i] != driver_active[i]) {
      if (driver_active[i]) {
        Serial.printf("*** Device %s - disconnected ***\n", driver_names[i]);
        driver_active[i] = false;
      } else {
        Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[i], drivers[i]->idVendor(), drivers[i]->idProduct());
        driver_active[i] = true;

        const uint8_t *psz = drivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = drivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = drivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
      }
    }
  }

  for (uint8_t i = 0; i < CNT_HIDDEVICES; i++) {
    if (*hiddrivers[i] != hid_driver_active[i]) {
      if (hid_driver_active[i]) {
        Serial.printf("*** HID Device %s - disconnected ***\n", hid_driver_names[i]);
        hid_driver_active[i] = false;
      } else {
        Serial.printf("*** HID Device %s %x:%x - connected ***\n", hid_driver_names[i], hiddrivers[i]->idVendor(), hiddrivers[i]->idProduct());
        hid_driver_active[i] = true;

        const uint8_t *psz = hiddrivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = hiddrivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = hiddrivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
      }
    }
  }
}


#endif // JOYSTICK_H
