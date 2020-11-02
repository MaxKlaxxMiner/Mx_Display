#define TWI_BUFFER_LENGTH MAX_BUFFER_LENGTH

#include "twi.h"
#include "TwoMaxWire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoMaxWire::rxBuffer[MAX_BUFFER_LENGTH];
uint8_t TwoMaxWire::rxBufferIndex = 0;
uint8_t TwoMaxWire::rxBufferLength = 0;

uint8_t TwoMaxWire::txAddress = 0;
uint8_t TwoMaxWire::txBuffer[MAX_BUFFER_LENGTH];
uint8_t TwoMaxWire::txBufferIndex = 0;
uint8_t TwoMaxWire::txBufferLength = 0;

uint8_t TwoMaxWire::transmitting = 0;
void(*TwoMaxWire::user_onRequest)(void);
void(*TwoMaxWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoMaxWire::TwoMaxWire()
{}

// Public Methods //////////////////////////////////////////////////////////////

void TwoMaxWire::begin(void)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  twi_init();
  twi_attachSlaveTxEvent(onRequestService); // default callback must exist
  twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
}

void TwoMaxWire::begin(uint8_t address)
{
  begin();
  twi_setAddress(address);
}

void TwoMaxWire::begin(int address)
{
  begin((uint8_t)address);
}

void TwoMaxWire::end(void)
{
  twi_disable();
}

void TwoMaxWire::setClock(uint32_t clock)
{
  twi_setFrequency(clock);
}

uint8_t TwoMaxWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  if (isize > 0)
  {
    // send internal address; this mode allows sending a repeated start to access
    // some devices' internal registers. This function is executed by the hardware
    // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

    beginTransmission(address);

    // the maximum size of internal address is 3 bytes
    if (isize > 3)
    {
      isize = 3;
    }

    // write internal register address - most significant byte first
    while (isize-- > 0)
      write((uint8_t)(iaddress >> (isize * 8)));
    endTransmission(false);
  }

  // clamp to buffer length
  if (quantity > MAX_BUFFER_LENGTH)
  {
    quantity = MAX_BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

uint8_t TwoMaxWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoMaxWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoMaxWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoMaxWire::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoMaxWire::directSend(byte address, byte firstByte, const byte* buffer, int byteCount)
{
  byte* ptr = buffer;
  ptr--;
  byte lastByte = ptr[0]; ptr[0] = firstByte;
  twi_directWrite(address, ptr, byteCount + 1);
  ptr[0] = lastByte;

  txBufferIndex = 0; txBufferLength = 0; transmitting = 0;
}

void TwoMaxWire::beginTransmission(uint8_t address)
{
  // indicate that we are transmitting
  transmitting = 1;
  // set address of targeted slave
  txAddress = address;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoMaxWire::beginTransmission(int address)
{
  beginTransmission((uint8_t)address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoMaxWire::endTransmission(uint8_t sendStop)
{
  // transmit buffer (blocking)
  uint8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  transmitting = 0;
  return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoMaxWire::endTransmission(void)
{
  return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoMaxWire::write(uint8_t data)
{
  if (transmitting)
  {
    // in master transmitter mode
    // don't bother if buffer is full
    if (txBufferLength >= MAX_BUFFER_LENGTH)
    {
      setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }
  else
  {
    // in slave send mode
    // reply to master
    twi_transmit(&data, 1);
  }
  return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoMaxWire::write(const uint8_t* data, size_t quantity)
{
  if (transmitting)
  {
    // in master transmitter mode
    for (size_t i = 0; i < quantity; ++i)
    {
      write(data[i]);
    }
  }
  else
  {
    // in slave send mode
    // reply to master
    twi_transmit(data, quantity);
  }
  return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoMaxWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoMaxWire::read(void)
{
  int value = -1;

  // get each successive byte on each call
  if (rxBufferIndex < rxBufferLength)
  {
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoMaxWire::peek(void)
{
  int value = -1;

  if (rxBufferIndex < rxBufferLength)
  {
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

void TwoMaxWire::flush(void)
{
  // XXX: to be implemented.
}

// behind the scenes function that is called when data is received
void TwoMaxWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
  // don't bother if user hasn't registered a callback
  if (!user_onReceive)
  {
    return;
  }
  // don't bother if rx buffer is in use by a master requestFrom() op
  // i know this drops data, but it allows for slight stupidity
  // meaning, they may not have read all the master requestFrom() data yet
  if (rxBufferIndex < rxBufferLength)
  {
    return;
  }
  // copy twi rx buffer into local read buffer
  // this enables new reads to happen in parallel
  for (uint8_t i = 0; i < numBytes; ++i)
  {
    rxBuffer[i] = inBytes[i];
  }
  // set rx iterator vars
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  // alert user program
  user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void TwoMaxWire::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if (!user_onRequest)
  {
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  txBufferIndex = 0;
  txBufferLength = 0;
  // alert user program
  user_onRequest();
}

// sets function called on slave write
void TwoMaxWire::onReceive(void(*function)(int))
{
  user_onReceive = function;
}

// sets function called on slave read
void TwoMaxWire::onRequest(void(*function)(void))
{
  user_onRequest = function;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoMaxWire MaxWire = TwoMaxWire();
