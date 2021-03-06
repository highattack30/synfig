/* === S Y N F I G ========================================================= */
/*!	\file guidset.h
**	\brief GUIDSet typedef
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2008 Chris Moore
**	Copyright (c) 2016 caryoscelus
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_GUIDSET_H
#define __SYNFIG_GUIDSET_H

/* === H E A D E R S ======================================================= */

#include "guid.h"

#include <set>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

// TODO: It would be better to replace it with typedef/using
// But that'll break some forward declarations, so leaving it as is for now
class GUIDSet : public std::set<synfig::GUID> {};

};

/* === E N D =============================================================== */

#endif
