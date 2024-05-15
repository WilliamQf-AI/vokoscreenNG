/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkWASAPIGstreamer.h"

#include <QDebug>

QvkWASAPIGstreamer::QvkWASAPIGstreamer()
{
}


QvkWASAPIGstreamer::~QvkWASAPIGstreamer()
{
}

QStringList QvkWASAPIGstreamer::get_all_Audio_Source_devices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "audio/x-raw" );
    gst_device_monitor_add_filter( monitor, "Audio/Source", caps );
    bool isMonitorStart = gst_device_monitor_start( monitor );

    listStructure.clear();
    listDevices.clear();
    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {

        QString stringStructure;
        gint device_width = 0;
        gint device_height = 0;

        device = (GstDevice*)iterator->data;

        // From here caps
        GstCaps *Caps = gst_device_get_caps( device );
        GstStructure *structureCaps = gst_caps_get_structure( Caps, 0 );
        stringStructure = "CAPS: " + QString( gst_structure_to_string( structureCaps ) );
/*
        if ( gst_structure_has_field( structureCaps, "format" ) ) {
            const gchar *format = NULL;
            format = gst_structure_get_string( structureCaps, "format" );
            Q_UNUSED(format)
        }

        if ( gst_structure_has_field( structureCaps, "width" ) ) {
            gint width = 0;
            if ( gst_structure_get_int( structureCaps, "width", &width ) ) {
                device_width = width;
            }
        }

        if ( gst_structure_has_field( structureCaps, "height" ) ) {
            gint height = 0;
            if ( gst_structure_get_int( structureCaps, "height", &height ) ) {
                device_height = height;
            }
        }
*/

        // From here properties
        GstStructure *structure = gst_device_get_properties( device );
        stringStructure.append( "PROPERTIES: " + QString( gst_structure_to_string( structure ) ) );
        listStructure.append( stringStructure );

        QString device_api = QString( gst_structure_get_string( structure, "device.api" ) );
        QString device_id = QString( gst_structure_get_string( structure, "device.id" ) );
        if ( ( device_api == "wasapi2" ) and ( device_id.contains( "MMDEVAPI" ) ) ) {
            QString device_id = QString( gst_structure_get_string( structure, "device.id" ) );
            QString device_description = QString( gst_structure_get_string( structure, "wasapi2.device.description" ) );
            listDevices << device_id + ":::" + device_description + ":::" + "Source" + ":::" + device_api;
        }


/*
        guint64 value;
        gst_structure_get_uint64( structure, "device.hmonitor", &value );
        QString device_handle = QString::number( value );

        gint valueRight;
        gst_structure_get_int( structure, "display.coordinates.right", &valueRight );
        int right = valueRight;
        Q_UNUSED(right)

        gint valueLeft;
        gst_structure_get_int( structure, "display.coordinates.left", &valueLeft );
        int left = valueLeft;
        Q_UNUSED(left)

        gint valueTop;
        gst_structure_get_int( structure, "display.coordinates.top", &valueTop );
        int top = valueTop;
        Q_UNUSED(top)

        gint valueBottom;
        gst_structure_get_int( structure, "display.coordinates.bottom", &valueBottom );
        int bottom = valueBottom;
        Q_UNUSED(bottom)
*/
        gst_structure_free( structure );
    }

    if ( isMonitorStart == true ) {
        gst_device_monitor_stop( monitor );
    }

    return listDevices;
}


/*
QString QvkWASAPIGstreamer::get_AudioDeviceString( GstDevice *device )
{
  static const char *const ignored_propnames[] = { "name", "parent", "direction", "template", "caps", Q_NULLPTR };
  GString *launch_line = Q_NULLPTR;
  GstElement *element;
  GstElement *pureelement;
  GParamSpec **properties, *property;
  GValue value = G_VALUE_INIT;
  GValue pvalue = G_VALUE_INIT;
  guint i, number_of_properties;
  GstElementFactory *factory;

  element = gst_device_create_element( device, Q_NULLPTR );

  if ( !element ) {
    return Q_NULLPTR;
  }

  factory = gst_element_get_factory( element );
  if ( !factory ) {
    gst_object_unref( element );
    return Q_NULLPTR;
  }

  if ( !gst_plugin_feature_get_name( factory ) ) {
    gst_object_unref( element );
    return Q_NULLPTR;
  }

  pureelement = gst_element_factory_create( factory, Q_NULLPTR );

  properties = g_object_class_list_properties( G_OBJECT_GET_CLASS( element ), &number_of_properties );
  if ( properties ) {
    for ( i = 0; i < number_of_properties; i++ ) {
      gint j;
      gboolean ignore = FALSE;
      property = properties[i];

      if ( ( property->flags & G_PARAM_READWRITE ) != G_PARAM_READWRITE ) {
        continue;
      }

      for ( j = 0; ignored_propnames[j]; j++ ) {
        if ( !g_strcmp0( ignored_propnames[j], property->name ) ) {
          ignore = TRUE;
        }
      }

      if ( ignore ) {
        continue;
      }

      g_value_init( &value, property->value_type );
      g_value_init( &pvalue, property->value_type );
      g_object_get_property( G_OBJECT( element ), property->name, &value );
      g_object_get_property( G_OBJECT( pureelement ), property->name, &pvalue );
      if (gst_value_compare( &value, &pvalue ) != GST_VALUE_EQUAL )
      {
        gchar *valuestr = gst_value_serialize( &value );
        if ( !valuestr ) {
          GST_WARNING( "Could not serialize property %s:%s", GST_OBJECT_NAME( element ), property->name );
          g_free( valuestr );
          goto next;
        }

        launch_line = g_string_new( valuestr );

        g_free( valuestr );
      }

    next:
      g_value_unset( &value );
      g_value_unset( &pvalue );
    }
    g_free( properties );
  }

  gst_object_unref( GST_OBJECT( element ) );
  gst_object_unref( GST_OBJECT( pureelement ) );

  QString string = "";
  if ( launch_line != Q_NULLPTR ) {
    string = g_string_free( launch_line, FALSE );
  }
  return string;
}


QStringList QvkWASAPIGstreamer::get_all_Audio_Source_devices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    gchar *name;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;
    QString stringDevice;
    QStringList stringList;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "audio/x-raw" );
    gst_device_monitor_add_filter( monitor, "Audio/Source", caps );
    bool isMonitorStart =  gst_device_monitor_start( monitor );

    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {
        device = (GstDevice*)iterator->data;
        name = gst_device_get_display_name( device );
        stringDevice = get_AudioDeviceString( device );
        stringDevice.append( ":::" ).append( name ).append( ":::" ).append( "Source" );
        if ( stringDevice.contains( ".") ) {
            if ( stringDevice.contains( "MMDEVAPI" ) == false ) {
                stringList.append( stringDevice );
            }
        }
    }

    if ( isMonitorStart == true ) {
       gst_device_monitor_stop( monitor );
    }

    return stringList;
}

QStringList QvkWASAPIGstreamer::get_all_Audio_Playback_devices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    gchar *name;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;
    QString stringDevice;
    QStringList stringList;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "audio/x-raw" );
    gst_device_monitor_add_filter( monitor, "Audio/Sink", caps );
    bool isMonitorStart =  gst_device_monitor_start( monitor );

    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {
        device = (GstDevice*)iterator->data;
        name = gst_device_get_display_name( device );
        stringDevice = get_AudioDeviceString( device );
        stringDevice.append( ":::" ).append( name ).append( ":::" ).append( "Playback" );
        if ( stringDevice.contains( ".") ) {
            if ( stringDevice.contains( "MMDEVAPI" ) == false ) {
                stringList.append( stringDevice );
            }
        }
    }

    if ( isMonitorStart == true ) {
       gst_device_monitor_stop( monitor );
    }

    return stringList;
}
*/
