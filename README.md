
# The Legend of Zelda
A fan-made version of the NES classic The Legend of Zelda. The game was made as a final project for Introduction to Embedded Systems at the University of Texas at Austin. The game runs on the [EK-TM4C123GXL](http://www.ti.com/tool/EK-TM4C123GXL) (TM4C123) microcontroller using the TM4C123 microprocessor. 

* [Circuit Diagram](https://github.com/cpuguy96/the-legend-of-zelda/tree/master/res/images/circuit/circuit_diagram.png)
* [Bill of Materials](https://github.com/cpuguy96/the-legend-of-zelda/tree/master/bill_of_materials.xlsx)

## Installation

### Install Kiel uVision4 and LaunchPad drivers

Kiel uVision4 is IDE used to for development on TI microprocessors. You can find the instructions to install Kiel uVision [here](http://users.ece.utexas.edu/~valvano/EE445L/downloads.htm#Keil). The free version only supports up to 56kb size projects, however. Kiel uVision5 can be used, but it is not tested.

You can find the launchpad drivers [here](http://users.ece.utexas.edu/~valvano/EE445L/downloads.htm#drivers).

Other IDEs can be used but you will have to find a  toolchain that can compile C code to ARM assembly. 

### Clone the repository

```bash
get clone https://github.com/cpuguy96/the-legend-of-zelda.git
```

### Create a uVision Project

You can find starter code for TM4C123 [here](http://users.ece.utexas.edu/~valvano/arm/). Move an existing .uvproj file into the `the-legend-of-zeld/src` folder. Include all C and assmbley files in the project items. 

It is better to build off an existing uVision Project instead of creating your own. This ensures the code will build and flash correctly. If you want to try to setup a project yourself, make sure the target device is the TM4C1231C3PM. 

Build and flash the project onto the TM4C123 microcontroller.

Message me if you have difficulties setting up the project.

## Authors

* **Chimeize Iwuanyanwu** - *Circuit design, game model and display*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Nintendo for creating the Legend of Zelda
* Xinyuan (Allen) Pan (my original lab partner) 