#include <dbus/dbus.h>


int main(int argc, char** argv) {
	DBusError err;
	dbus_error_init(&err);
	dbus_threads_init_default();
	if (dbus_error_is_set(&err)) {
		//LOGE("Could not get onto the system bus: %s", err.message);
		dbus_error_free(&err);
		return false;
	}
	//dbus_connection_set_exit_on_disconnect(nat->conn, FALSE);

}
