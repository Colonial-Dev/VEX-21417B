### Worldsbound

This was unexpected, but I'm certainly not complaining and glad to be part of the team! For now, I'm just going to jot down some key TODOs for converting the code to the new robot, plus some preliminary improvements.

- Strip ALL code specific to the old robot, leaving the function/control flow structures behind to be refilled later.
- Scrap the controller menu and take full advantage of PROS+LVGL to create a proper Brain UI. I can create most of this at home due to LVGL offering a simulator program, which is nice.
- Reconfigure motors/ports in general for the new bot, and refill operator control tasks.
- Reconfigure autonomous initialization for the new bot. The BIG thing here is the tracking wheels; I think there's already two, and if Duncan/Colin could install a third, then I can very quickly get full-blown odometry up and running via OkapiLib. (The key advantage of OkapiLib odom is its ability to self-correct; if a previous movement is off or disrupted in some way, future commands will automatically compensate, which HUGELY simplifies routine writing.)
- Write round autons, then skills auton. I'm going to defer to Duncan and the others in terms of strategy on this one; if we can get proper odom, then almost nothing is impossible.