# artefact
This is the repo of the code from the box of my bachelor thesis. 

Libraries used are:\
FastLED\
Wire\
SimpleSDAudio: https://www.hackerspace-ffm.de/wiki/index.php?title=Datei:SimpleSDAudio_V1.03.zip


Here you can see the connection points between sensor and arduino used in the code.

| Sensor            | Pin On Sensor | Pin Connected to Arduino | 
|-------------------|---------------|--------------------------|
| MH-SD Card Module |               |                          |
|                   | CS            |  4                       | 
|                   | Mosi          |  11                      | 
|                   | SCK           |  13                      |   
|                   | MISO          |  12                      | 
| GY-521            |               |                          | 
|                   | SCL           |  5                       | 
|                   | SDA           |  4                       | 
| LM386             |               |                          | 
|                   | IN            |  9                       | 
| REED              |               |                          | 
|                   | IN            | 2                        | 
| REED              |               |                          | 
|                   | IN            | 3                        | 
| NeoPixel          |               |                          |
|                   |  3Volt        | 3Volt                    |
|                   |  IN           | 5                        |
