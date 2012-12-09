/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2012 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"

#include <gtk/gtk.h>
#include <colord.h>

#include "cd-sample-widget.h"

G_DEFINE_TYPE (CdSampleWidget, cd_sample_widget, GTK_TYPE_DRAWING_AREA);
#define CD_SAMPLE_WIDGET_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), CD_TYPE_SAMPLE_WIDGET, CdSampleWidgetPrivate))

struct CdSampleWidgetPrivate
{
	cairo_t			*cr;
	CdColorRGB		 color;
};

enum
{
	PROP_0,
	PROP_COLOR,
};

/**
 * up_sample_get_property:
 **/
static void
up_sample_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	CdSampleWidget *sample = CD_SAMPLE_WIDGET (object);
	switch (prop_id) {
	case PROP_COLOR:
		g_value_set_boxed (value, &sample->priv->color);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

/**
 * up_sample_set_property:
 **/
static void
up_sample_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	CdColorRGB *tmp;
	CdSampleWidget *sample = CD_SAMPLE_WIDGET (object);

	switch (prop_id) {
	case PROP_COLOR:
		tmp = g_value_get_boxed (value);
		cd_color_copy_rgb (tmp, &sample->priv->color);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}

	/* refresh widget */
	gtk_widget_hide (GTK_WIDGET (sample));
	gtk_widget_show (GTK_WIDGET (sample));
}

/**
 * cd_sample_widget_draw:
 **/
static gboolean
cd_sample_widget_draw (GtkWidget *widget, cairo_t *cr)
{
	CdColorRGB *color;
	CdSampleWidget *sample = CD_SAMPLE_WIDGET (widget);
	GtkAllocation allocation;

	g_return_val_if_fail (CD_IS_SAMPLE_WIDGET (sample), FALSE);

	color = &sample->priv->color;
	gtk_widget_get_allocation (widget, &allocation);
	cairo_save (cr);
	cairo_set_source_rgb (cr, color->R, color->G, color->B);
	cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);
	cairo_fill (cr);
	cairo_restore (cr);
	return FALSE;
}

/**
 * cd_sample_widget_class_init:
 **/
static void
cd_sample_widget_class_init (CdSampleWidgetClass *class)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	widget_class->draw = cd_sample_widget_draw;
	object_class->get_property = up_sample_get_property;
	object_class->set_property = up_sample_set_property;

	g_type_class_add_private (class, sizeof (CdSampleWidgetPrivate));

	/* properties */
	g_object_class_install_property (object_class,
					 PROP_COLOR,
					 g_param_spec_boxed ("color", NULL, NULL,
							     CD_TYPE_COLOR_RGB,
							     G_PARAM_READWRITE));
}

/**
 * cd_sample_widget_init:
 **/
static void
cd_sample_widget_init (CdSampleWidget *sample)
{
	sample->priv = CD_SAMPLE_WIDGET_GET_PRIVATE (sample);
	cd_color_set_rgb (&sample->priv->color, 1.0, 1.0, 1.0);
}

/**
 * cd_sample_widget_set_color:
 * @sample: This class instance
 * @color: A color
 *
 * Sets the color for the sample widget
 **/
void
cd_sample_widget_set_color (CdSampleWidget *sample, const CdColorRGB *color)
{
	g_return_if_fail (color != NULL);
	g_return_if_fail (CD_IS_SAMPLE_WIDGET (sample));

	/* set new color and refresh */
	cd_color_copy_rgb (color, &sample->priv->color);
	gtk_widget_queue_draw (GTK_WIDGET (sample));
}

/**
 * cd_sample_widget_new:
 * Return value: A new #CdSampleWidget object.
 **/
GtkWidget *
cd_sample_widget_new (void)
{
	return g_object_new (CD_TYPE_SAMPLE_WIDGET, NULL);
}