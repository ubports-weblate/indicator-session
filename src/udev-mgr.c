/*
Copyright 2011 Canonical Ltd.

Authors:
    Conor Curran <conor.curran@canonical.com>

This program is free software: you can redistribute it and/or modify it 
under the terms of the GNU General Public License version 3, as published 
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranties of 
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR 
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "udev-mgr.h"
#include <gudev/gudev.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

static void udevice_mgr_device_list_iterator (gpointer data,
                                              gpointer userdata);
static void udev_mgr_uevent_cb  (GUdevClient *client,
                                 gchar       *action,
                                 GUdevDevice *device,
                                 gpointer     user_data);   
                                 
struct _UdevMgr
{
	GObject parent_instance;
  DbusmenuMenuitem* scanner_item;
  DbusmenuMenuitem* webcam_item;  
  GUdevClient* client;  
};

const char *subsystems[1] = {"usb"};
const gchar* usb_subsystem = "usb";

G_DEFINE_TYPE (UdevMgr, udev_mgr, G_TYPE_OBJECT);

static void
udev_mgr_init (UdevMgr* self)
{
  self->client = NULL;
  self->client = g_udev_client_new (subsystems);
  
  GList* devices_available  = g_udev_client_query_by_subsystem (self->client,
                                                                usb_subsystem);
  
  g_list_foreach (devices_available, udevice_mgr_device_list_iterator, self);
  g_list_free (devices_available);
  g_signal_connect (G_OBJECT (self->client),
                   "uevent",
                    G_CALLBACK (udev_mgr_uevent_cb),
                    self);
}
 
static void
udev_mgr_finalize (GObject *object)
{
	G_OBJECT_CLASS (udev_mgr_parent_class)->finalize (object);
}

static void
udev_mgr_class_init (UdevMgrClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = udev_mgr_finalize;
}

static void
udevice_mgr_device_list_iterator (gpointer data, gpointer userdata)
{
  g_return_if_fail (G_UDEV_IS_DEVICE (data));
  GUdevDevice* device = G_UDEV_DEVICE (data);
  const gchar* name = g_udev_device_get_name (device);
  
  g_debug ("UDEV MGR - the name of the device = %s", name);
  // for now tidy up here.
  g_object_unref (device);
}

static void udev_mgr_uevent_cb (GUdevClient *client,
                                gchar       *action,
                                GUdevDevice *device,
                                gpointer     user_data)   
{
  g_return_if_fail (UDEV_IS_MGR (user_data));

  g_debug ("just received a UEVENT with an action :  %s", action);

  const gchar* vendor;
  const gchar* product;
  const gchar* number;
  
	vendor = g_udev_device_get_property (device, "ID_VENDOR_ID");
	product = g_udev_device_get_property (device, "ID_MODEL_ID");
  number = g_udev_device_get_number (device);
  g_debug ("device vendor id %s and product id of %s and number of %s",
           g_strdup(vendor),
           g_strdup(product),
           g_strdup(number));
           
  const gchar *const *list;
  const gchar *const *iter;  
  char propstr[500];
  guint32 namelen = 0, i;  
  
  list = g_udev_device_get_property_keys(device);
  
  for (iter = list; iter && *iter; iter++) {
    if (strlen(*iter) > namelen)
      namelen = strlen(*iter);
  }
  namelen++;

  for (iter = list; iter && *iter; iter++) {
    strcpy(propstr, *iter);
    strcat(propstr, ":");
    for (i = 0; i < namelen - strlen(*iter); i++)
           strcat(propstr, " ");
    strcat(propstr, g_udev_device_get_property(device, *iter));
    g_debug("%s", propstr);
  }  
}


UdevMgr* udev_mgr_new (DbusmenuMenuitem* scanner, 
                       DbusmenuMenuitem* webcam)
{
  UdevMgr* mgr = g_object_new (UDEV_TYPE_MGR, NULL);
  mgr->scanner_item = scanner;
  mgr->webcam_item = webcam;
  return mgr;
}
