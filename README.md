# BaseCode

this is ANN BaseCode for myself to learn about **ANN**

## Project tree
~~~
-Root
|-ReadME.md
|-Makefile
|-include
	|-General.h(General.cpp's head file)	
	|-Matrix.h(include Array model class&Array operation)
	|-sysTool.h(include some system tool class declaration)
|-src
	|-Ann.cpp（about Ann code）	
	|-General.cpp	
	|-sysTool.cpp
~~~

### Brief

in **include** director is project head file

		General.h:
			General.cpp's head file
		Matrix.h:
			include Array model class&Array operation
		sysTool.h:
			include some system tool class declaration

in **src** director is project srouce file

        Ann.cpp:
            include network source code
        Gerneral.cpp:
            include some Global obj
        sysTool.cpp:
            include sysTool source code like timestamp

## Makefile

use **make** in project root dir to build aim

    make

and u can set **PREFIX** to set the build dir like follow:

    make PREFIX=~/privat

the default value of PREFIX is `.`
there will be `bin`、`lib` created in **PREFIX** director
