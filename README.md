# Epidemic simulation

**Context** : Second year project \
**Class** : Operating Systems \
**Year** : 2020-2021

**Students** :

* Sarah Brood *sarah.brood@ecole.ensicaen.fr*
* Thomas Savignac *thomas.savignac@ecole.ensicaen.fr*

## What is this ?
A simple project simulating an epidemic.. a topical one ! 
The project is developped in **C**. 

## Installation & Usage
Be sure to have [ncurses](https://ftp.gnu.org/pub/gnu/ncurses/) and [gnuplot](http://www.gnuplot.info/download.html) installed. 
### All in one 
Build and run the project 
```shell script
./play_simulation
```
### Makefile 
Make to build all executables
```shell script
make 
```
Clean everything 
```shell script
make clean # to clean everything
make mrproper # to clean exes 
```
### Run the project 
pay attention to the order
```shell script
./bin/timer &
./bin/epidemic_sim &
./bin/press_agency &
./bin/citizen_manager
```
Indication about the simulation 
----------------------------------
* You like the action and the city is too calm ? Sorry but firefighters and doctors are too good for you or the seed of intial contamination was too small, try again ! 
* You are notified about who dies in the death notice.
* To see more, put in full screen. 
* Wait for the end to see a graphic representating the entire simulation

### About the city map 
| symbole | building |
| ------ | ------ |
| H | hospital |
| F | firestation |
| ^ | house |

| color | tile contamination rate |
| ------ | ------ |
| ![#008000](https://via.placeholder.com/15/008000/000000?text=+) | <0.1 |
|![#FFFF00](https://via.placeholder.com/15/FFFF00/000000?text=+) |  <0.3 |
|![#f03c15](https://via.placeholder.com/15/f03c15/000000?text=+)  | \>0.3 |


Sources
----
* Some of the content is from [Alain Lebret's github](https://github.com/alainlebret/os) and his class
* We used [stackoverflow](https://stackoverflow.com/) 
* Learned a little ncurses thanks to [NCURSES Programming HOWTO](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)

License
----
**Free Software, Hell Yeah!**
