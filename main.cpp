#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <cstdio>
#include <unistd.h>

#include	<glib-object.h>


#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_boolean(v)  g_value_get_boolean (v)
#define g_marshal_value_peek_char(v)     g_value_get_char (v)
#define g_marshal_value_peek_uchar(v)    g_value_get_uchar (v)
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_uint(v)     g_value_get_uint (v)
#define g_marshal_value_peek_long(v)     g_value_get_long (v)
#define g_marshal_value_peek_ulong(v)    g_value_get_ulong (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_uint64(v)   g_value_get_uint64 (v)
#define g_marshal_value_peek_enum(v)     g_value_get_enum (v)
#define g_marshal_value_peek_flags(v)    g_value_get_flags (v)
#define g_marshal_value_peek_float(v)    g_value_get_float (v)
#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#define g_marshal_value_peek_string(v)   (char*) g_value_get_string (v)
#define g_marshal_value_peek_param(v)    g_value_get_param (v)
#define g_marshal_value_peek_boxed(v)    g_value_get_boxed (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
#define g_marshal_value_peek_variant(v)  g_value_get_variant (v)
#else /* !G_ENABLE_DEBUG */
/* WARNING: This code accesses GValues directly, which is UNSUPPORTED API.
 *          Do not access GValues directly in your code. Instead, use the
 *          g_value_get_*() functions
 */
#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_variant(v)  (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */

/* VOID:STRING,BOXED (../marshaller.txt:1) */
void
g_cclosure_user_marshal_VOID__STRING_BOXED (GClosure     *closure,
                                            GValue       *return_value G_GNUC_UNUSED,
                                            guint         n_param_values,
                                            const GValue *param_values,
                                            gpointer      invocation_hint G_GNUC_UNUSED,
                                            gpointer      marshal_data) {
	typedef void (*GMarshalFunc_VOID__STRING_BOXED) (gpointer     data1,
													 gpointer     arg_1,
													 gpointer     arg_2,
													 gpointer     data2);
	register GMarshalFunc_VOID__STRING_BOXED callback;
	register GCClosure *cc = (GCClosure*) closure;
	register gpointer data1, data2;
	g_return_if_fail (n_param_values == 3);

	if (G_CCLOSURE_SWAP_DATA (closure))
    {
		data1 = closure->data;
		data2 = g_value_peek_pointer (param_values + 0);
    }
	else
    {
		data1 = g_value_peek_pointer (param_values + 0);
		data2 = closure->data;
    }
	callback = (GMarshalFunc_VOID__STRING_BOXED) (marshal_data ? marshal_data : cc->callback);

	callback (data1,
			  g_marshal_value_peek_string (param_values + 1),
			  g_marshal_value_peek_boxed (param_values + 2),
			  data2);
}

/* VOID:STRING,VARIANT (../marshaller.txt:1) */
void
g_cclosure_user_marshal_VOID__STRING_VARIANT (GClosure     *closure,
                                              GValue       *return_value G_GNUC_UNUSED,
                                              guint         n_param_values,
                                              const GValue *param_values,
                                              gpointer      invocation_hint G_GNUC_UNUSED,
                                              gpointer      marshal_data) {
	typedef void (*GMarshalFunc_VOID__STRING_VARIANT) (gpointer     data1,
													   gpointer     arg_1,
													   gpointer     arg_2,
													   gpointer     data2);
	register GMarshalFunc_VOID__STRING_VARIANT callback;
	register GCClosure *cc = (GCClosure*) closure;
	register gpointer data1, data2;
	printf("Calling marshaller!");
	g_return_if_fail (n_param_values == 3);

	if (G_CCLOSURE_SWAP_DATA (closure))
    {
		data1 = closure->data;
		data2 = g_value_peek_pointer (param_values + 0);
    }
	else
    {
		data1 = g_value_peek_pointer (param_values + 0);
		data2 = closure->data;
    }
	callback = (GMarshalFunc_VOID__STRING_VARIANT) (marshal_data ? marshal_data : cc->callback);

	callback (data1,
			  g_marshal_value_peek_string (param_values + 1),
			  g_marshal_value_peek_variant (param_values + 2),
			  data2);
}


static void DeviceChanged(DBusGProxy *device, const char *property,
						  GValue *value, gpointer user_data) {
	printf("Calling device changed on %s\n", property);
}

static void DeviceFound(DBusGProxy *device, const char *address,
						GHashTable*  values, gpointer user_data) {
	printf("Calling device found on %s\n", address);//g_value_get_string(static_cast<const GValue*>(g_hash_table_lookup(values, "Name"))));
}

int main(int argc, char** argv) {

	g_type_init();

/* GMainLoop for our program. */
	GMainContext* mainloop = NULL;
/* Will store the result of the RequestName RPC here. */
	guint result;
	GError* error = NULL;
/* Initialize the GType/GObject system. */
//g_type_init ();
/* Create a main loop that will dispatch callbacks. */
	mainloop = g_main_context_default ();
/* Print error and terminate. */
	if (mainloop == NULL) {
/* Print error and terminate. */
		return 1;
	}

	DBusGConnection* mDBusConnection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);

	if (!mDBusConnection) {
		g_printerr("Failed to open connection to bus: %s\n", error->message);
		g_error_free(error);
		return 1;
	}

	DBusConnection* dbusConnection =
		dbus_g_connection_get_connection(mDBusConnection);

	// Make sure we do not exit the entire program if DBus connection get lost.
	dbus_connection_set_exit_on_disconnect(dbusConnection, false);

	DBusGProxy* mBluezManagerProxy = dbus_g_proxy_new_for_name(mDBusConnection,
															   "org.bluez",
															   "/",
															   "org.bluez.Manager");
	if(!mBluezManagerProxy) {
		printf("Can't create proxy!\n");
		return 1;
	}

	const char* adapter;
	if(!dbus_g_proxy_call(mBluezManagerProxy, "DefaultAdapter", &error,
						  G_TYPE_INVALID,
						  DBUS_TYPE_G_OBJECT_PATH, &adapter,
						  G_TYPE_INVALID)) {
		g_printerr("Error: %s\n", error->message);
		g_error_free(error);
		return 1;	
	}
	printf("%s\n", adapter);
  
	DBusGProxy* mAdapterProxy = dbus_g_proxy_new_for_name(mDBusConnection,
														  "org.bluez",
														  adapter,
														  "org.bluez.Adapter");

	if(!mAdapterProxy) {
		printf("Can't create adapter!\n");
		return 1;
	}

  
	dbus_g_object_register_marshaller(g_cclosure_user_marshal_VOID__STRING_BOXED,
									  G_TYPE_NONE, G_TYPE_STRING,
									  G_TYPE_VALUE, G_TYPE_INVALID);
	dbus_g_proxy_add_signal(mAdapterProxy, "PropertyChanged",
							G_TYPE_STRING, G_TYPE_VALUE, G_TYPE_INVALID);
	dbus_g_proxy_connect_signal(mAdapterProxy, "PropertyChanged",
								G_CALLBACK(DeviceChanged), NULL, NULL);

	GType typeGHashTable = dbus_g_type_get_map("GHashTable", G_TYPE_STRING,
																						 G_TYPE_VALUE);

	dbus_g_proxy_add_signal(mAdapterProxy, "DeviceFound",
													G_TYPE_STRING, typeGHashTable, G_TYPE_INVALID);
	dbus_g_proxy_connect_signal(mAdapterProxy, "DeviceFound",
															G_CALLBACK(DeviceFound), NULL, NULL);

	GHashTable* hashTable = NULL;
	if (!dbus_g_proxy_call(mAdapterProxy, "GetProperties", &error,
						   G_TYPE_INVALID,
						   typeGHashTable, &hashTable, G_TYPE_INVALID)) {
		g_printerr("Error: %s\n", error->message);
		g_error_free(error);
		return 1;
	}

	const gchar* name;
	name = g_value_get_string(static_cast<const GValue*>(g_hash_table_lookup(hashTable, "Name")));
	printf("%s\n", name);

	GValue val = {0, };

	g_value_init (&val, G_TYPE_STRING);
	g_value_set_string (&val, "testing2w34e");
  
	if (!dbus_g_proxy_call(mAdapterProxy, "SetProperty", &error, G_TYPE_STRING, "Name",
						   G_TYPE_VALUE, &val,
						   G_TYPE_INVALID,
						   G_TYPE_INVALID)) {
		g_printerr("Error: %s\n", error->message);
		g_error_free(error);
		return 1;
	}

	if (!dbus_g_proxy_call(mAdapterProxy, "StartDiscovery", &error,
						   G_TYPE_INVALID,
						   G_TYPE_INVALID)) {
		g_printerr("Error: %s\n", error->message);
		g_error_free(error);
		return 1;
	}

	while(1) {
		g_main_context_iteration (mainloop, true);
	}
	g_object_unref(mBluezManagerProxy);
	g_object_unref(mAdapterProxy);
	dbus_g_connection_unref(mDBusConnection);
  
	return 0;
}
