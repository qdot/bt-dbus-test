#include <dbus-c++/dbus.h>
#include "manager.h"
#include "all.h"
#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <cstring>

class BluezManager
  : public org::bluez::ManagerProxy,
		public DBus::IntrospectableProxy,
		public DBus::ObjectProxy
{
public:

  BluezManager(DBus::Connection &connection, const char *path, const char *name)
		: DBus::ObjectProxy(connection, path, name)
	{}

	void AdapterAdded(const ::DBus::Path& argin0) {}
	void AdapterRemoved(const ::DBus::Path& argin0) {}
	void DefaultAdapterChanged(const ::DBus::Path& argin0) {}
};

class BluezAdapter
  : public org::bluez::Adapter_proxy,
		public DBus::IntrospectableProxy,
		public DBus::ObjectProxy
{
public:

  BluezAdapter(DBus::Connection &connection, const char *path, const char *name)
		: DBus::ObjectProxy(connection, path, name)
	{}

	void DeviceCreated(const ::DBus::Path& argin0) {}
	virtual void DeviceRemoved(const ::DBus::Path& argin0) {}
	virtual void DeviceFound(const std::string& argin0, const std::map< std::string, ::DBus::Variant >& argin1) {}
	virtual void PropertyChanged(const std::string& argin0, const ::DBus::Variant& argin1) {}
	virtual void DeviceDisappeared(const std::string& argin0) {}
};

using namespace std;

static const char *BLUEZ_SERVER_NAME = "org.bluez";
static const char *BLUEZ_SERVER_PATH = "/";

static const size_t THREADS = 1;

static bool spin = true;

BluezManager *g_client = NULL;
//BluezAdapter *g_adapter = NULL;

DBus::Pipe *thread_pipe_list[THREADS];

DBus::BusDispatcher dispatcher;
DBus::DefaultTimeout *timeout;

void niam(int sig)
{
  spin = false;

  dispatcher.leave();
}

int main()
{

	typedef std::map< std::string, ::DBus::Variant > DBusDict;
  size_t i;

  signal(SIGTERM, niam);
  signal(SIGINT, niam);

  DBus::_init_threading();

  DBus::default_dispatcher = &dispatcher;

  // increase DBus-C++ frequency
  new DBus::DefaultTimeout(100, false, &dispatcher);

  DBus::Connection conn = DBus::Connection::SystemBus();

  BluezManager manager(conn, BLUEZ_SERVER_PATH, BLUEZ_SERVER_NAME);
  //g_client = &client;

	::DBus::Path default_adapter_path = manager.DefaultAdapter();
	printf("%s\n", default_adapter_path.c_str());

	BluezAdapter adapter(conn, default_adapter_path.c_str(), BLUEZ_SERVER_NAME);

	DBusDict d = adapter.GetProperties();
	for(DBusDict::iterator itr = d.begin(); itr != d.end(); ++itr)
	{
		printf("%s\n", (*itr).first.c_str());
	}
	
  // pthread_t threads[THREADS];

  // thread_pipe_list[0] = dispatcher.add_pipe(handler1, NULL);
	// pthread_create(threads + i, NULL, greeter_thread, (void *) i);

  // dispatcher.enter();

  // cout << "terminating" << endl;

  // for (i = 0; i < THREADS; ++i)
  // {
  //   pthread_join(threads[i], NULL);
  // }

  // dispatcher.del_pipe(thread_pipe_list[0]);
  // dispatcher.del_pipe(thread_pipe_list[1]);
  // dispatcher.del_pipe(thread_pipe_list[2]);

  return 0;
}
