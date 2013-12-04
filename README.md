Hydronized arm_navigation stack 
=============


Introduction
------------------
Due the introduction of the moveIt framework in ROS Groovy, the arm_navigation stack was completely removed from ROS Hydro.
But some of the code my team and i am using in my RoboCup team depends on the arm_navigation stack.
So it wasn't possible to make moveIt run in a short amount of time with the youBot platform of KUKA, so the nearest idea was to make the arm_navigation stack
compilable and working under the new ROS API indroduced with ROS Groovy and Hydro.
With my changes the arm_navigation stack is compilable and working again under ROS Groovy at least for me and some additional test
which also included simulations.
So if you have the problem as well that you depend on the arm_navigation stack but you want to use the latest ROS version as well 
this might getting you started.

As i just modified the code of the original arm_navigation stack which is available at [source](https://kforge.ros.org/armnavigation/armnavigation)
and clonable via "hg https://kforge.ros.org/armnavigation/armnavigation", the modified code is of course still under the BSD license.

I also attached the "arm_navigation.patch" so you can easily review my changes.



Disclaimer
------------------
This code was modified from the lates branch of the arm_navigation stack on Dec. 1 2013 and edited in a 12hr coding sprint by me.
So i dont't give any kind of guarantee of completeness or accuracy.
