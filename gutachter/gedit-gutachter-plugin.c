/*
 * gedit-gutachter-plugin.c
 * This file is part of gedit
 *
 * Copyright (C) 2010 - Ignacio Casal Quinteiro
 *
 * gedit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gedit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gedit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gedit-gutachter-plugin.h"

#include <gedit/gedit-debug.h>
#include <gedit/gedit-window.h>

#define WINDOW_DATA_KEY "GeditGutachterPluginDataKey"

#define GEDIT_GUTACHTER_PLUGIN_GET_PRIVATE(object) \
				(G_TYPE_INSTANCE_GET_PRIVATE ((object),	\
				GEDIT_TYPE_GUTACHTER_PLUGIN,		\
				GeditGutachterPluginPrivate))

struct _GeditGutachterPluginPrivate
{

};

GEDIT_PLUGIN_REGISTER_TYPE_WITH_CODE (GeditGutachterPlugin, gedit_gutachter_plugin,
	gsc_provider_gutachter_register_type (type_module);
)

static void
gedit_gutachter_plugin_init (GeditGutachterPlugin *plugin)
{
	plugin->priv = GEDIT_GUTACHTER_PLUGIN_GET_PRIVATE (plugin);

	gedit_debug_message (DEBUG_PLUGINS, "GeditGutachterPlugin initializing");
}

static void
gedit_gutachter_plugin_finalize (GObject *object)
{
	gedit_debug_message (DEBUG_PLUGINS, "GeditGutachterPlugin finalizing");

	G_OBJECT_CLASS (gedit_gutachter_plugin_parent_class)->finalize (object);
}

static void
gedit_gutachter_plugin_dispose (GObject *object)
{
	GeditGutachterPlugin *plugin = GEDIT_GUTACHTER_PLUGIN (object);

	gedit_debug_message (DEBUG_PLUGINS, "GeditGutachterPlugin disposing");

	G_OBJECT_CLASS (gedit_gutachter_plugin_parent_class)->dispose (object);
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{

}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{

}

static void
gedit_gutachter_plugin_class_init (GeditGutachterPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = gedit_gutachter_plugin_finalize;
	object_class->dispose = gedit_gutachter_plugin_dispose;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;

	//g_type_class_add_private (object_class, sizeof (GeditGutachterPluginPrivate));
}
