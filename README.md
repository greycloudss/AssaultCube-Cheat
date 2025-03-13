# AssaultCube Aimbot & ESP Cheat

## Overview
This project is an educational exploration into memory manipulation and computer vision techniques applied to game environments. Specifically, it is designed for **AssaultCube 1.3.0.2**, demonstrating how one can interact with game memory to develop an **aimbot** and **ESP (Extra Sensory Perception)** system. The project highlights methods used in red teaming exercises for security research and ethical hacking.

## Features
- **Aimbot**: Uses trigonometric calculations to adjust the player's view to target enemy heads.
- **Memory Manipulation**: Reads and writes to game memory using `ReadProcessMemory` and `WriteProcessMemory`.
- **Entity Prioritization**: Identifies and tracks the closest enemy.
- **Efficient Offset Utilization**: Uses predefined memory offsets for precise interaction with the game.

## File Structure
```
📦 AssaultCube-Cheat
├── main.cpp         # Entry point for execution
├── aimbot.cpp       # Aimbot logic and calculations
├── aimbot.h         # Aimbot class declaration
├── entity.cpp       # Entity class implementation
├── entity.h         # Entity structure definitions
├── memchecker.cpp   # Memory reading and writing functions
├── memchecker.h     # Memory handler declarations
├── offsets.h        # Memory offsets for AssaultCube
├── payload.cpp      # Additional helper functions
├── payload.h        # Payload function declarations
└── README.md        # Project documentation
```

## Setup & Compilation
### Requirements
- Windows 10/11 (x64)
- C++ x32 Compiler (MinGW32 or other)
- Visual Studio 2019/2022 (Recommended)
- AssaultCube 1.3.0.2 (Tested Version)

### Compilation Steps
1. Clone the repository:
   ```sh
   git clone https://github.com/your-repo/AssaultCube-Cheat.git
   cd AssaultCube-Cheat
   ```
2. Open **Visual Studio** and create a new C++ project.
3. Add all `.cpp` and `.h` files to the project.
4. Compile the project using:
   ```sh
   g++ -shared -o client.dll main.cpp payload.cpp checkmem/memchecker.cpp /checkmem/entity.cpp /features/aimbot.cpp -m32 "-Wl,--subsystem,windows" 
   ```
5. Inject the compiled .dll file while **AssaultCube** is active.

## Technical Details
### Aimbot Functionality
- The **aimbot** calculates angles between the player's view and enemy positions.
- Uses `atan2()` for yaw and pitch calculation.
- Applies **normalization techniques** to keep angles within expected ranges.
- Writes computed angles to camera memory offsets.

### ESP Implementation
- Reads the **entity list** from game memory.
- Extracts **player positions, health, and team data**.
- Converts **3D world coordinates to 2D screen positions**.

## Troubleshooting
### Common Issues
- **Aimbot misalignment**
  - Check the calculation in `atan2(-deltaZ, -deltaX) * (180.0f / M_PI);`
  - Normalize yaw angles between `-180° to 180°`.
- **Failure to read/write memory**
  - Run the program as injector as **Administrator**.
  - Check if antivirus software is blocking memory access.

## Ethical Considerations
This project is strictly for **educational purposes** and **ethical hacking research**. Understanding how software interacts with system memory is a valuable skill in **red teaming**, **reverse engineering**, and **cybersecurity analysis**. Unauthorized use of such techniques in online environments may violate **terms of service** agreements and **legal regulations**.


