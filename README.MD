# Magic Poi Lite Source Code
##### *Firmware for ESP8266 - fetches saved timelines from magicpoi.circusscientist.com (Magic Poi Lite server API) and displays specified colours according to precise timing.*

### Requirements: 
- Software: PlatformIO
- Hardware: D1 mini
- Additional: common cathode RGB LED connected to D5 D6 and D7 *(optional - otherwise just look at the Serial Monitor to see colours)*

### Setup: 
- sign up for a free account at http://magicpoi.circusscientist.com and go to the "Magic Poi Lite" page, use "Record Timeline" and "Save Timeline" to create a timeline.
- copy the secrets_example.txt file to src folder and re-name to secrets.h
- edit secrets.h, adding your WiFi details and MadicPoi login
- upload to D1 mini and watch your LED's change colour

### Notes: 
- on the Magic Poi Lite page you can click on Sequence Mode to create a timeline with fixed interval (1 second between changes) 
- the strobing colours are not implemented yet
- you may need to log in again periodically
- on startup the code fetches the currently selected timeline (buttons show up on the right of interface after save, click on button and press "Play Timeline" to use). Re-start the D1 mini to re-load after selecting a new timeline on the site. 
- timeline will loop back to start on finish *(this will be optional in a future version)*

- *this is all experimental code subject to change without notice* 

### TODO: 
- download all timelines at once and store, select which one to use with a button press
- update timelines (fetch from server) with another button press
- play once only option (for shows where you don't want to repeat)
- add strobing colours to changeColours() function
- WiFi remote control hardware addon to sync multiple poi. 
