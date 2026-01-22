/*
===========================================================================================
    This file is part of BTFEdit BTF Texture format framework.

    Copyright (c) 2026 Cristiano B. Santos <cristianobeato_dm@hotmail.com>
    Contributor(s): none yet.

-------------------------------------------------------------------------------------------

 This file is part of the crglLib library and is licensed under the
 MIT License with Attribution Requirement.

 You are free to use, modify, and distribute this file (even commercially),
 as long as you give credit to the original author:

     “Based on BTFEdit by Cristiano Beato – https://github.com/CristianoBeato”

 For full license terms, see the LICENSE file in the root of this repository.
===============================================================================================
*/

#ifndef __PRECOMPILED_HPP__
#define __PRECOMPILED_HPP__

#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <cstddef>
#include <cstdio>

#include <iostream>
#include <exception>
#include <string>
#include <vector>

#include <SDL3/SDL_stdinc.h>

#include <SDL3/SDL_window.hpp>
#include <SDL3/SDL_iostream.hpp>
#include <SDL3/SDL_thread.hpp>
#include <SDL3/SDL_mutex.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gli/type.hpp>
#include <gli/gli.hpp>
#include <gli/image.hpp>
#include <gli/format.hpp>
#include <gli/convert.hpp>

#include <crglCore.hpp>

#include "BTFtexture/BTFStructures.hpp"
#include "BTFtexture/BTFLoader.hpp"

#endif //!__PRECOMPILED_HPP__