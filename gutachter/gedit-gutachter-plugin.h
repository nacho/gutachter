/*
 * gedit-gutachter-plugin.h
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


#ifndef __GEDIT_GUTACHTER_PLUGIN_H__
#define __GEDIT_GUTACHTER_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GEDIT_TYPE_GUTACHTER_PLUGIN		(gedit_gutachter_plugin_get_type ())
#define GEDIT_GUTACHTER_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), GEDIT_TYPE_GUTACHTER_PLUGIN, GeditGutachterPlugin))
#define GEDIT_GUTACHTER_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GEDIT_TYPE_GUTACHTER_PLUGIN, GeditGutachterPluginClass))
#define GEDIT_IS_GUTACHTER_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GEDIT_TYPE_GUTACHTER_PLUGIN))
#define GEDIT_IS_GUTACHTER_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GEDIT_TYPE_GUTACHTER_PLUGIN))
#define GEDIT_GUTACHTER_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GEDIT_TYPE_GUTACHTER_PLUGIN, GeditGutachterPluginClass))

/* Private structure type */
typedef struct _GeditGutachterPluginPrivate	GeditGutachterPluginPrivate;

/*
 * Main object structure
 */
typedef struct _GeditGutachterPlugin		GeditGutachterPlugin;

struct _GeditGutachterPlugin
{
	GeditPlugin parent_instance;

	GeditGutachterPluginPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _GeditGutachterPluginClass	GeditGutachterPluginClass;

struct _GeditGutachterPluginClass
{
	GeditPluginClass parent_class;
};

/*
 * Public methods
 */
GType	gedit_gutachter_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_gedit_plugin 	(GTypeModule *module);

G_END_DECLS

#endif /* __GEDIT_Gutachter_PLUGIN_H__ */
