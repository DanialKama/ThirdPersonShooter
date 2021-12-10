# Third Person Shooter
## Introduction
This project is a TPS sample project developed in Unreal Engine 5.
This is not production-ready code but a starting point for learning TPS concepts.
If you have any questions or suggestions about this project, talk to me on [LinkedIn](https://www.linkedin.com/in/danialkamali/).
You can also access the project launcher and pictures of the project [here](https://mega.nz/folder/ePBzRAiR#9p42rAnO-mbLILq2aViTpg).

## Concepts covered
- Pickup system for weapons and ammo
- AI enemy and friend (based on Gameplay Tags) with damage, hearing, prediction, and sight senses and abilities like a point to point patrolling, combat (aim, fire weapon, reload and switch to another weapon), navigate to important places, search, retreat (on low health), and healing.
- Chaos Destruction and Field Systems (plugin is in beta and not stable)
- Health system, including reducing health based on damage type like point damage (rife bullet), radial damage (explosion), or fall damage, and health recovery based on stamina level.
- Stamina system, including reducing stamina based on movement states like walking, running, sprinting, and stamina recovery.
- Weapon system, including fire rate (time between shots), cool down time, supporting ammo types (projectile), auto fire support, max ammo, default ammo, mag size, reloading, fire sound, particle, and animation
- Weapon related activities such ass switch, reload, drop with animations and sound, mag actor, and empty shell ejection
- Weapon recoil (player camera shake, AI aim offset, and character animation) and bullet spread based on Float Curves
- Projectile system, including reducing damage, impact particle, sound, and decal based on surface type (like flesh or surfaces), travel speed, trail emitter, and field impact on destructible objects
- Advanced animation setups for every weapon type and every movement state and moving direction (2D blend space and aim offsets)
- Body dismemberment based on hit bone
- fall damage with getup animation based on mesh orientation
- Control Rig for character foot place and Correct hands place on gun based on aiming state with IK
- AI respawn system
- Level Manager and Interactable actors (like the automatic door)
- Improved level Streaming blueprints (load or unload levels based on doors state)

## Minor details
Assertion failed solved by unchecking Async Loading Thread Enabled.
Navigation failed on streamed level solved by set Runtime Generation to Dynamic.

The assets included are listed below, and some of them were made by myself.
- [FPS Weapon Bundle](https://www.unrealengine.com/marketplace/en-US/product/fps-weapon-bundle)
- [Military Weapons Dark](https://www.unrealengine.com/marketplace/en-US/product/military-weapons-dark)
- [Military Weapons Silver](https://www.unrealengine.com/marketplace/en-US/product/military-weapons-silver)
- Some free 3D models from [cgtrader](https://www.cgtrader.com/)
- Most animations are from [Mixamo](https://www.mixamo.com/) and sounds from [Freesound](https://freesound.org/)
