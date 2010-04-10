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

#include <gutachter-widget.h>
#include <gutachter-suite.h>
#include <gutachter-runner.h>

#include <gedit/gedit-debug.h>
#include <gedit/gedit-window.h>

#define WINDOW_DATA_KEY "GeditGutachterPluginDataKey"

typedef struct _WindowData
{
	GtkWidget *panel;
	GtkWidget *widget;
} WindowData;

GEDIT_PLUGIN_REGISTER_TYPE (GeditGutachterPlugin, gedit_gutachter_plugin)

static void
gedit_gutachter_plugin_init (GeditGutachterPlugin *plugin)
{
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
	gedit_debug_message (DEBUG_PLUGINS, "GeditGutachterPlugin disposing");

	G_OBJECT_CLASS (gedit_gutachter_plugin_parent_class)->dispose (object);
}

static void
free_window_data (gpointer data)
{
	WindowData *wdata = (WindowData *)data;

	g_slice_free (WindowData, data);
}

static void
add_panel (GeditWindow *window,
	   WindowData  *data)
{
	GeditPanel *panel;

	data->widget = gtk_test_widget_new ();

	panel = gedit_window_get_side_panel (window);

	gedit_panel_add_item (panel,
			      data->widget,
			      "Gutachter",
			      NULL);
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	WindowData *data;

	data = g_slice_new (WindowData);

	add_panel (window, data);

	g_object_set_data_full (G_OBJECT (window),
				WINDOW_DATA_KEY,
				data,
				free_window_data);
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{
	WindowData *data;
	GeditPanel *panel;

	data = g_object_get_data (G_OBJECT (window), WINDOW_DATA_KEY);

	panel = gedit_window_get_side_panel (window);

	gedit_panel_remove_item (panel, data->widget);

	g_object_set_data (G_OBJECT (window), WINDOW_DATA_KEY, NULL);
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
}
