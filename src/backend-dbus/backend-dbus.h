/*
 * Copyright 2013 Canonical Ltd.
 *
 * Authors:
 *   Charles Kerr <charles.kerr@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __INDICATOR_SESSION_BACKEND_DESKTOP_H__
#define __INDICATOR_SESSION_BACKEND_DESKTOP_H__

#include <gio/gio.h> /* GCancellable */

#include "../actions.h"
#include "../guest.h"
#include "../users.h"

G_BEGIN_DECLS

void backend_get (GCancellable             * cancellable,
                  IndicatorSessionActions ** setme_actions,
                  IndicatorSessionUsers   ** setme_users,
                  IndicatorSessionGuest   ** setme_guest);

G_END_DECLS

#endif
