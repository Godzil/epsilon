/* Keyboard initialization code
 *
 * The job of this code is to implement the "ion_key_state" function.
 *
 * The keyboard is a matrix that is laid out as follow:
 *
 *      |  PC0 |  PC1 | PC13 | PC14 | PC15 |
 * -----+------+------+------+------+------+
 *  PB1 | K_A1 | K_A2 | K_A3 | K_A4 | K_A5 |
 * -----+------+------+------+------+------+
 *  PB2 | K_B1 | K_B2 | K_B3 | K_B4 | K_B5 |
 * -----+------+------+------+------+------+
 *  PB3 | K_C1 | K_C2 | K_C3 | K_C4 | K_C5 |
 * -----+------+------+------+------+------+
 *  PB4 | K_D1 | K_D2 | K_D3 | K_D4 | K_D5 |
 * -----+------+------+------+------+------+
 *  PB5 | K_E1 | K_E2 | K_E3 | K_E4 | K_E5 |
 * -----+------+------+------+------+------+
 *  PB6 | K_F1 | K_F2 | K_F3 | K_F4 | K_F5 |
 * -----+------+------+------+------+------+
 *  PB7 | K_G1 | K_G2 | K_G3 | K_G4 | K_G5 |
 * -----+------+------+------+------+------+
 *  PB8 | K_H1 | K_H2 | K_H3 | K_H4 | K_H5 |
 * -----+------+------+------+------+------+
 *  PB9 | K_I1 | K_I2 | K_I3 | K_I4 | K_I5 |
 * -----+------+------+------+------+------+
 * PB10 | K_J1 | K_J2 | K_J3 | K_J4 | K_J5 |
 * -----+------+------+------+------+------+
 *
 *  We decide to drive the rows (PB1-10) and read the columns (PC0,1,13,14,15).
 *
 *  To avoid short-circuits, the pins B1-B10 will not be standard outputs but
 *  only open-drain. Open drain means the pin is either driven low or left
 *  floating.
 *  When a user presses multiple keys, a connection between two rows can happen.
 *  If we dont' use open drain outputs, this situation could trigger a short
 *  circuit between an output driving high and another driving low.
 *
 *  If the outputs are open-drain, this means that the input must be pulled up.
 *  So if the input reads "1", this means the key is in fact *not* pressed, and
 *  if it reads "0" it means that there's a short to an open-drain output. Which
 *  means the corresponding key is pressed.
 */

#include <ion.h>
#include "keyboard.h"

// Public Ion::Keyboard methods

namespace Ion {
namespace Keyboard {

bool keyDown(Key k) {
  // Drive the corresponding row low, and let all the others float.
  int row = Device::rowForKey(k);
  for (uint8_t i=0; i<Device::numberOfRows; i++) {
    /* In open-drain mode, a 0 in the register drives the pin low, and a 1 lets
     * the pin floating (Hi-Z). So we want to set the current row to zero and
     * all the others to 1. */
    bool state = (i == row ? 0 : 1);
    uint8_t pin = Device::RowPins[i];
    Device::RowGPIO.ODR()->set(pin, state);
  }

  msleep(1);

  // Read the input of the proper column
  uint8_t column = Device::columnForKey(k);
  uint8_t pin = Device::ColumnPins[column];
  bool input = Device::ColumnGPIO.IDR()->get(pin);

  /* The key is down if the input is brought low by the output. In other words,
   * we want to return true if the input is low (false). */
  return (input == false);
}

}
}

// Private Ion::Keyboard::Device methods

void Ion::Keyboard::Device::init() {
  for (uint8_t i=0; i<numberOfRows; i++) {
    uint8_t pin = RowPins[i];
    RowGPIO.MODER()->setMode(pin, GPIO::MODER::Mode::Output);
    RowGPIO.OTYPER()->setType(pin, GPIO::OTYPER::Type::OpenDrain);
  }

  for (uint8_t i=0; i<numberOfColumns; i++) {
    uint8_t pin = ColumnPins[i];
    ColumnGPIO.MODER()->setMode(pin, GPIO::MODER::Mode::Input);
    ColumnGPIO.PUPDR()->setPull(pin, GPIO::PUPDR::Pull::Up);
  }
}