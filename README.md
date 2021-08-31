# Soft Renderer
순수한 CPU 기반으로 렌더링을 수행하는 것을 목표로 하는 프로젝트입니다.
현재는 Windows 플랫폼 만을 지원합니다.

## Fork
해당 프로젝트는 다음의 리포지토리의 Main 브랜치를 포크하여 구성된 프로젝트입니다.
https://github.com/ideugu/CKSoftRenderer

## Preparations
Soft Renderer를 빌드하기 위해서는 다음과 같은 소프트웨어가 필요합니다.  
CMake 3.1 or later: https://cmake.org/download  
Visual Studio 2017 or 2019: https://visualstudio.microsoft.com

## Installation
위의 소프트웨어를 준비했다면 다음의 절차를 따라서 빌드하면 됩니다.  
1. CMake-VS-201X.bat를 실행하여 Makefile을 구성합니다.  
2. Project 폴더 안에 존재하는 .sln 파일을 실행하여 VS를 이용해 빌드합니다.  

## Feature
![image](https://user-images.githubusercontent.com/49399405/131482366-d3e8d1f2-3004-4eb0-bee5-6db9050f9dd6.png)
- OBJ, TGA 자체 파싱 및 렌더링
- Lambert Diffuse / Blin-Phong Specular 적용
- Entity-Component 구조 적용
