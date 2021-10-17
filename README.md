# Third Person Shooter

Made by Unreal Engine 5
If you have any questions or suggestions about this project, talk to me on [LinkedIn](https://www.linkedin.com/in/danialkamali/).

This repository contains:
- Test map
- Shooting Arena map
- Environment Materials
- Pickup system for weapons and ammo
- AI enemy and friend (based on Gameplay Tags) with damage, hearing, prediction, and sight senses and abilities like a point to point patrolling, combat (aim, fire weapon, reload and switch to another weapon), navigate to important places, search, retreat (on low health), and healing.
- Chaos Destruction and Field Systems (plugin is in beta version and there are many bugs)
- Physics material (like head flesh, body flesh, wood, metal, and stone)
- Health system, including reducing health based on damage type like point damage (rife bullet), radial damage (explosion), or fall damage, and health recovery based on stamina level.
- Stamina system, including reducing stamina based on movement states like walking, running, sprinting, and stamina recovery.
- Weapon system, including fire rate (time between shots), cool down time, supporting ammo types (projectile), auto fire support, max ammo, default ammo, mag size, reloading, fire sound, particle, and animation
- Weapon swap, reload and drop with animations and sound
- Weapon recoil (camera shake for player and aim offset for AI) and bullet spread based on Float Curves
- Projectile system, including reducing damage, impact particle, sound, and decal based on surface type (like flesh or surfaces), travel speed, trail emitter, and field impact on destructible objects
- Mag actor (full or empty mag based on ammo in the mag on reload time and surface hit sound) 
- Empty shell actor (custom ejection speed and surface hit sound)
- Different animation setup for every weapon type and every movement state and moving direction (2D animation blend space)
- Body dismemberment based on which bone get hit (At the time of death)
- Idle action and fall damage with getup animation based on mesh orientation
- Control Rig for character foot place and Correct hands place on gun based on aiming state with IK
- AI respawn system
- Level Manager and Interactable actors (like an automatic door)
- Level Streaming blueprints that interact with other actors (load level opens door after the stream is complete or unload level start unloading after doors is close)
- Controllable Pawns, helicopter, and jet
- Chaos vehicle 

Minor details:
Assertion failed solved by unchecking Async Loading Thread Enabled.
Navigation failed on streamed level solved by set Runtime Generation to Dynamic.

You can also access the project launcher [here](https://mega.nz/folder/ePBzRAiR#9p42rAnO-mbLILq2aViTpg).

Here are some contents that I used in this project:
- [FPS Weapon Bundle](https://www.unrealengine.com/marketplace/en-US/product/fps-weapon-bundle)
- [Military Weapons Dark](https://www.unrealengine.com/marketplace/en-US/product/military-weapons-dark)
- [Military Weapons Silver](https://www.unrealengine.com/marketplace/en-US/product/military-weapons-silver)
- [AH-64D Apache Longbow (West)](https://www.unrealengine.com/marketplace/en-US/product/ah-64d-apache-longbow-us)
- [Fighter F/A-18C Hornet (West)](https://www.unrealengine.com/marketplace/en-US/product/fighter-f-a-18c-hornet)
- Some free 3D models from [cgtrader](https://www.cgtrader.com/)
- Most animations are from the [Mixamo](https://www.mixamo.com/)