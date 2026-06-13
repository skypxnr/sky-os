# sky-os
A CLI based operating system. This is a project so I learn how OSes interact with hardware. We are going to skip the beginner level topics like 'what is an OS', 'different types of OS', etc.

# How is Operating System loaded?
Every time we press on the power button on the computer, it triggers a set of instructions called <b>BIOS</b>. 

## Basic Input-Output System (BIOS)
The BIOS is responsible for performing checks and loading your OS into the RAM. The BIOS initially performs a series of tests called 'POST'. POST checks for connected hardware, I/O devices, bootable devices. <br>If any of the critical components fail, the computer will not boot. Depending on the BIOS, it might display an error code (using a 7 segment display if available in the motherboard); or a series of meaningful beeps which is reflected on its documentation provided by the manufacturer.<br><br>If everything is working fine, the BIOS then looks for a suitable "bootable" drive. Once it finds one, it loads the OS from that bootable drive to the RAM.
