# CPU_Analyzer

This project is a consol application that monitors CPU usage and prints it to console.

Main application contains three threads:

Reader - reads number of CPU cores,file /proc/stat and sends it to next thread,
Analyzer - reads data from Reader and calculate recived data to % usage and sends it to next thread,
Printer -  reads data from Analyzer and print data to console.


## Console view 
![This is an image](https://github.com/YocoZuna/CPU_Analyzer/blob/main/console_view.png)

Like on the picture application prints usgae of each core, number of cores and PID of main application. To do gracefull exit just type in another console kill {PID}, 
this is only proper way to clean up program. 


Application has been tested with valgrind - no leaks detected  
## Valgrind result 
![This is an image](https://github.com/YocoZuna/CPU_Analyzer/blob/main/valgrind.png)


