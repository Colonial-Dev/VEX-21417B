# Overview
This repository contains the complete PROS competition code of VRC team 21417B (Robokauz - Intercept) during the 2021-2022 *Tipping Point* game season. Nothing is censored, and I've documented essentially everything!

This year, we went to worlds and ended up placing 3rd in the top division (Science) during qualifications, but were wiped out in the quarterfinals. In terms of skills, I was getting 240 at practice but only managed a single 109 attempt because the skills fields closed earlier than we realized.

# Features
- IMU and vector-based odometry - this position tracking method trades off a few decimal places of accuracy in exchange for ease of implementation and hardware flexibility.
- Full Pure Pursuit controller implementation, including:
  - Rich path generation via quintic spline interpolation (invoked via a flexible builder pattern, see below)
  - Path management wrapper to store generated paths and generate simple transient ones (e.g. to move a fixed distance regardless of position)
  - Generally well-behaved exit conditions and emergency stop mechanisms
- Several hand-rolled controllers, such as a simple conveyor state machine and a bang-bang arm controller.
- Rich Brain UI with status readouts and autonomous routine selector

# Reuse

This code is licensed under the GNU General Public License, v3.0. You should read the LICENSE.md file for more information, but in a nutshell, GNU GPL is a *copyleft* license. This means:
- You are free to download, modify, distribute and otherwise use the code as you see fit.
- ***However,*** the above privileges are contingent on the preservation of the GNU GPL license. If you create a modified version of the code, or include portions of the code in your own project (even modified) then these derivative works must also be licensed under GNU GPL v3.0 and made publicly available for others to take advantage of. 
- The decision to use GNU GPL v3.0 is an intentional decision on my part, in order to both stay compliant with VRC rules (code reuse by someone other than the author is only permissible if *everyone* can do so - this is why PROS and its associated libraries are allowed) and to foster a more collaborative environment in the VEX coding space. ~~Do you have any idea how much I had to dig to find pure pursuit reference code? I'd like others to not have to go through that, thanks very much.~~