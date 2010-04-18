/*
 * gedit-gutachter-plugin.c
 * This file is part of gedit
 *
 * Copyright (C) 2010 - Ignacio Casal Quinteiro
 *
 * Based on gutachter code by Sven Herzberg.
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
#include <gutachter-xvfb.h>

#include <gedit/gedit-debug.h>
#include <gedit/gedit-window.h>

#include <glib/gi18n.h>

#define WINDOW_DATA_KEY "GeditGutachterPluginDataKey"

#define GEDIT_GUTACHTER_PLUGIN_GET_PRIVATE(object) \
				(G_TYPE_INSTANCE_GET_PRIVATE ((object),	\
				GEDIT_TYPE_GUTACHTER_PLUGIN,		\
				GeditGutachterPluginPrivate))

typedef struct _WindowData
{
	GtkWidget *panel;
	GtkWidget *widget;
	GtkToolItem *execute_button;
	glong status_handler;
} WindowData;

struct _GeditGutachterPluginPrivate
{
	GutachterXvfb* xvfb;
};

GEDIT_PLUGIN_REGISTER_TYPE (GeditGutachterPlugin, gedit_gutachter_plugin)

static void
gedit_gutachter_plugin_init (GeditGutachterPlugin *plugin)
{
	plugin->priv = GEDIT_GUTACHTER_PLUGIN_GET_PRIVATE (plugin);

	gedit_debug_message (DEBUG_PLUGINS, "GeditGutachterPlugin initializing");

	plugin->priv->xvfb = gutachter_xvfb_get_instance ();
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

	if (plugin->priv->xvfb != NULL)
	{
		g_object_unref (plugin->priv->xvfb);
		plugin->priv->xvfb = NULL;
	}

	G_OBJECT_CLASS (gedit_gutachter_plugin_parent_class)->dispose (object);
}

static void
free_window_data (gpointer data)
{
	WindowData *wdata = (WindowData *)data;

	g_slice_free (WindowData, data);
}

static void
status_changed_cb (GObject    *suite,
                   GParamSpec *pspec G_GNUC_UNUSED,
                   WindowData *data)
{
	switch (gutachter_suite_get_status (GUTACHTER_SUITE (suite)))
	{
		case GUTACHTER_SUITE_LOADED:
		case GUTACHTER_SUITE_FINISHED:
			gtk_widget_set_sensitive (GTK_WIDGET (data->execute_button),
						  TRUE);
		break;
		default:
			gtk_widget_set_sensitive (GTK_WIDGET (data->execute_button),
						  FALSE);
		break;
	}
}

static void
set_file (WindowData *data,
          GFile      *file)
{
	GutachterSuite* suite;

	if (data->status_handler)
	{
		g_signal_handler_disconnect (gutachter_runner_get_suite (GUTACHTER_RUNNER (data->widget)),
					     data->status_handler);
		data->status_handler = 0;
	}

	gutachter_runner_set_file (GUTACHTER_RUNNER (data->widget), file);
	suite = gutachter_runner_get_suite (GUTACHTER_RUNNER (data->widget));

	if (suite)
	{
		data->status_handler = g_signal_connect (suite, "notify::status",
		                                         G_CALLBACK (status_changed_cb),
		                                         data);
		status_changed_cb (G_OBJECT (suite), NULL, data);
	}
}

static void
on_open_clicked (GtkToolItem *button G_GNUC_UNUSED,
                 GeditWindow *window)
{
	GtkFileFilter *filter;
	GtkWidget *dialog;
	WindowData *data;

	data = g_object_get_data (G_OBJECT (window), WINDOW_DATA_KEY);

	dialog = gtk_file_chooser_dialog_new (_("Choose Unit Test"),
	                                      GTK_WINDOW (window),
	                                      GTK_FILE_CHOOSER_ACTION_OPEN,
	                                      GTK_STOCK_CLOSE, GTK_RESPONSE_REJECT,
	                                      GTK_STOCK_OPEN,  GTK_RESPONSE_ACCEPT,
	                                      NULL);

	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_ACCEPT);

	filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, _("Executable Programs"));
	gtk_file_filter_add_mime_type (filter, "application/x-executable");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
	                             filter);

	if (GTK_RESPONSE_ACCEPT == gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		GFile* file;

		file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
		set_file (data, file);
		g_object_unref (file);
	}

	gtk_widget_destroy (dialog);
}

static void
on_execute_clicked (GtkToolItem *button G_GNUC_UNUSED,
		    WindowData  *data)
{
	gutachter_suite_execute (gutachter_runner_get_suite (GUTACHTER_RUNNER (data->widget)));
}

static void
test_suite_changed (GtkWidget  *widget,
                    GParamSpec *pspec G_GNUC_UNUSED,
                    WindowData *data)
{
	GutachterSuite *suite;
	GFile *file;

	file = gutachter_runner_get_file (GUTACHTER_RUNNER (widget));
	gtk_widget_set_sensitive (GTK_WIDGET (data->execute_button), file != NULL);

	suite = gutachter_runner_get_suite (GUTACHTER_RUNNER (widget));
	if (suite)
	{
		gutachter_suite_load (suite);
	}
}

static void
add_panel (GeditWindow *window,
	   WindowData  *data)
{
	GtkWidget *box;
	GtkWidget *toolbar;
	GtkToolItem *open_button;
	GtkToolItem *execute;
	GeditPanel *panel;

	box = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (box);

	toolbar = gtk_toolbar_new ();
	gtk_widget_show (toolbar);
	gtk_box_pack_start (GTK_BOX (box), toolbar, FALSE, FALSE, 0);

	open_button = gtk_tool_button_new_from_stock (GTK_STOCK_OPEN);
	gtk_widget_show (GTK_WIDGET (open_button));
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), open_button, -1);

	g_signal_connect (open_button, "clicked",
			  G_CALLBACK (on_open_clicked), window);

	data->execute_button = gtk_tool_button_new_from_stock (GTK_STOCK_EXECUTE);
	gtk_widget_show (GTK_WIDGET (data->execute_button));
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), data->execute_button, -1);
	gtk_widget_set_sensitive (GTK_WIDGET (data->execute_button),
				  FALSE);

	g_signal_connect (data->execute_button, "clicked",
			  G_CALLBACK (on_execute_clicked), data);

	data->widget = gutachter_widget_new ();
	gtk_widget_show (data->widget);
	gtk_box_pack_start (GTK_BOX (box), data->widget, TRUE, TRUE, 0);

	g_signal_connect (data->widget, "notify::test-suite",
	                  G_CALLBACK (test_suite_changed), data);

	panel = gedit_window_get_side_panel (window);

	gedit_panel_add_item (panel,
			      box,
			      "Gutachter",
			      NULL);
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	WindowData *data;

	data = g_slice_new (WindowData);
	data->status_handler = 0;

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

	g_signal_handlers_disconnect_by_func (data->widget, test_suite_changed,
					      data);

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

	g_type_class_add_private (object_class, sizeof (GeditGutachterPluginPrivate));
}
