

/// \addtogroup u2w
/// @{
/// \file

#ifndef __BASEPLAYER_H
#define __BASEPLAYER_H

#include "Platform\Define.h"

/// Player session in the World
class MANGOS_DLL_SPEC BasePlayer
{
    public:
		BasePlayer(){};
		~BasePlayer(){};

		virtual const bool& IsInWorld() const = 0;
};
#endif
/// @}
