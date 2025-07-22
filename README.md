# Custom Font Renderer

## Description
Custom Font Renderer is a project designed to explore the low-level intricacies of font rendering. By developing this renderer from scratch, I aimed to gain a deeper understanding of how fonts are parsed and displayed, beyond the abstraction provided by higher-level libraries. This project provides insights into the core processes involved in font rendering, from reading .ttf files to rendering text on the screen.

## Why I Made It
I created this custom font renderer to dive into the detailed workings of font rendering. As a programmer passionate about low-level programming, I wanted to challenge myself by learning how to parse font files and render them manually. This project serves as an educational tool and a foundation for building more complex rendering systems.

## Showcase

<p align="center">
  <img src="https://github.com/user-attachments/assets/61d09e19-c0e2-421d-bf19-30d97d55888f" alt="Final" width="80%" />
  <br><br>
  <img src="https://github.com/user-attachments/assets/20930c27-6ed6-4623-87f2-87e002357bd8" alt="OutlineBezierProgress" width="80%"/>
  <br><br>
  <img src="https://github.com/user-attachments/assets/013aec82-bf9d-44b9-ac9f-58c3466cf4eb" alt="OnOffCurve" width="25%"/>
</p>

## How to Use

### Prerequisites
- Premake
- Visual Studio (for Windows)
- A terminal (for other operating systems)

### Setup Instructions

#### For Windows Users:
1. Navigate to the `scripts` folder.
2. Run the `.bat` file to generate a Visual Studio project.
3. Open the generated Visual Studio project and build the solution.

#### Windows `.bat` File:
```bat
@echo off
pushd ..\
call vendor\premake\premake5.exe vs2022
popd
pause
