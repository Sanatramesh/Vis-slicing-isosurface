Compilation Instructions
The program consists of two files:

1. VolumeVisDraw.cpp ----> It contains code for plotting isosurfaces and slicing planes.

   To compile VolumeVisDraw.cc do:
   make VolumeVis

   and run using command:
  ./VolumeVis <datafile with path> 

  To remove the binary file do:
  make clear

      ##################### Useful Keys #####################
      z ---> Zoom In
      x ---> Zoom Out
      c ---> Change between isosurfaces and slicing planes
      up arrow ---> translate up
      down arrow ---> translate down
      left arrow ---> translate left
      right arrow ---> translate down
      ax + by + cz + d = 0
      w ---> increase value of 'a'
      s ---> decrease value of 'a'
      e ---> increase value of 'b'
      d ---> decrease value of 'b'
      r ---> increase value of 'c'
      f ---> decrease value of 'c'
      t ---> increase value of 'd'
      g ---> decrease value of 'd'
      q ---> to quit 
      mouse drag is used to rotate along x and y axis.

2. VolumeVis.cpp  ----> It contains all the utility functions used in VectorFieldPlot.cpp
