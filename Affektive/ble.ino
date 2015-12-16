


bool success;
int32_t hrmServiceId;
int32_t hrmLocationCharId;
int32_t mySvcId;

int blevPin = 22;
int bleGndPin = 24;
void ble_setup(){
  pinMode(blevPin, OUTPUT);
  pinMode(bleGndPin, OUTPUT);
  digitalWrite(blevPin, HIGH);
  digitalWrite(bleGndPin, LOW);
  delay(500);
  Serial.println( "Initialising the Bluefruit LE module: ");
  
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error( "COuldn't find BLuefruit, make sure it's in COmMand mode & check wiring?");
  }
  Serial.println(  "OK!" );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println( "PErforming a factory reset: ");
  if (! ble.factoryReset() ){
       error( "COuldn't factory reset");
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // this line is particularly required for Flora, but is a good idea
  // anyways for the super long lines ahead!
  // ble.setInterCharWriteDelay(5); // 5 ms

  /* Change the device name to make it easier to find */
  Serial.println( "SEtting device name to 'Affektive Band': ");

  if (! ble.sendCommandCheckOK( "AT+GAPDEVNAME=Affektive Band") ) {
    error( "COuld not set device name?");
  }

  // HEART RATE SERVICE
  /* Add the Heart Rate Service definition */
  /* Service ID should be 1 */
  Serial.println( "ADding the HEart RAte SErvice definition (UUID = 0x180D): ");
  success = ble.sendCommandWithIntReply(  "AT+GATTADDSERVICE=UUID=0x180D", &hrmServiceId);
  if (! success) {
    error( "COuld not add HRM service");
  }

  /* Add the Heart Rate Measurement characteristic */
  /* Chars ID for Measurement should be 1 */
  Serial.println( "ADding the HEart RAte MEasurement characteristic (UUID = 0x2A37): ");
  success = ble.sendCommandWithIntReply(  "AT+GATTADDCHAR=UUID=0x2A37, PROPERTIES=0x38, MIN_LEN=2, MAX_LEN=6, VALUE=80BPM", &hrmMeasureCharId);
    if (! success) {
    error( "COuld not add HRM characteristic");
  }

  /* Add the Body Sensor Location characteristic */
  /* Chars ID for Body should be 2 */
  Serial.println( "ADding the BOdy SEnsor LOcation characteristic (UUID = 0x2A38): ");
  success = ble.sendCommandWithIntReply(  "AT+GATTADDCHAR=UUID=0x2A38, PROPERTIES=0x02, MIN_LEN=1, VALUE=3", &hrmLocationCharId);
    if (! success) {
    error( "COuld not add BSL characteristic");
  }

  /* Add the Heart Rate Service to the advertising data (needed for Nordic apps to detect the service) */
  Serial.print( "ADding HEart RAte SErvice UUID to the advertising payload: ");
  ble.sendCommandCheckOK(  "AT+GAPSETADVDATA=02-01-06-05-02-0d-18-0a-18" );  


  // CUSTOM SERVICE
  Serial.println( "ADding the Custom SErvice definition (UUID = 0x1001): ");
  success = ble.sendCommandWithIntReply(  "AT+GATTADDSERVICE=UUID= 0x1001", &mySvcId);
  if (! success) {
    error( "COuld not add Custom service");
  }
  
  Serial.println( "Adding the GSR Measurement characteristic (UUID = 0xD001): ");
  success = ble.sendCommandWithIntReply(  "AT+GATTADDCHAR=UUID=0xD001, PROPERTIES=0x38, MIN_LEN=2, MAX_LEN=6, VALUE=600gsr", &gsrMeasureCharId);
    if (! success) {
    error( "COuld not add HRM characteristic");
  }
  
  /* Reset the device for the new service setting changes to take effect */
  Serial.print( "Performing a SW reset (service changes require a reset): ");
  ble.reset();
  Serial.println();
  
  
}

// A small helper
void error(char err[]) {
  Serial.println(err);
  while (1);
}

void set_gattchar(Adafruit_BluefruitLE_SPI ble, int32_t id, float val)
{
  ble.print( "AT+GATTCHAR=" );
  ble.print( id );
  ble.print( ",x" );
  ble.println(val);
//  if ( !ble.waitForOK() )
//  {
//    Serial.println("Failed to get response!");
//  }
  
}
