//#include "BluetoothCommon.h"
//#include "nsString.h"
#define BLUEZ_DBUS_BASE_PATH      "/org/bluez"
#define BLUEZ_DBUS_BASE_IFC       "org.bluez"
#define DBUS_ADAPTER_IFACE BLUEZ_DBUS_BASE_IFC ".Adapter"
#define DBUS_DEVICE_IFACE BLUEZ_DBUS_BASE_IFC ".Device"

#include <string.h>
#include <stdint.h>
#include <dbus/dbus.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

class RawDBusConnection
{
public:
  RawDBusConnection();
  ~RawDBusConnection();
  bool createDBusConnection();

  DBusMessage * dbus_func_args_timeout_valist(
    int timeout_ms,
    DBusError *err,
    const char *path,
    const char *ifc,
    const char *func,
    int first_arg_type,
    va_list args);

  DBusMessage * dbus_func_args_timeout(
    int timeout_ms,
    const char *path,
    const char *ifc,
    const char *func,
    int first_arg_type,
    ...);

  DBusMessage * dbus_func_args(                           
    const char *path,
    const char *ifc,
    const char *func,
    int first_arg_type,
    ...);

  DBusMessage * dbus_func_args_error(
    DBusError *err,
    const char *path,
    const char *ifc,
    const char *func,
    int first_arg_type,
    ...);

  int32_t dbus_returns_int32(DBusMessage *reply);
  uint32_t dbus_returns_uint32(DBusMessage *reply);
  // std::string dbus_returns_string(DBusMessage *reply);
  bool dbus_returns_boolean(DBusMessage *reply);

  template<typename T>
  bool GetDBusDictValue(DBusMessageIter& iter, const char* name, int expected_type, T& val) {
    DBusMessageIter dict_entry, dict;
    int size = 0,array_index = 0;
    int len = 0, prop_type = DBUS_TYPE_INVALID, prop_index = -1, type;

    if(dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_ARRAY) {
      printf("This isn't a dictionary!\n");
      return false;
    }
    dbus_message_iter_recurse(&iter, &dict);
    do {
      len = 0;
      if (dbus_message_iter_get_arg_type(&dict) != DBUS_TYPE_DICT_ENTRY)
      {
        printf("Not a dict entry!\n");
        return false;
      }            
      dbus_message_iter_recurse(&dict, &dict_entry);
      DBusMessageIter prop_val, array_val_iter;
      char *property = NULL;
      uint32_t array_type;
      char *str_val;
      int i, j, type, int_val;

      if (dbus_message_iter_get_arg_type(&dict_entry) != DBUS_TYPE_STRING) {
        printf("Iter not a string!\n");
        return false;
      }
      dbus_message_iter_get_basic(&dict_entry, &property);
      printf("Property: %s\n", property);
      if(strcmp(property, name) != 0) continue;
      if (!dbus_message_iter_next(&dict_entry))
      {
        printf("No next!\n");
        return false;
      }
      if (dbus_message_iter_get_arg_type(&dict_entry) != DBUS_TYPE_VARIANT)
      {
        printf("Iter not a variant!\n");
        return false;
      }
      dbus_message_iter_recurse(&dict_entry, &prop_val);
      // type = properties[*prop_index].type;
      if (dbus_message_iter_get_arg_type(&prop_val) != expected_type) {
        //   LOGE("Property type mismatch in get_property: %d, expected:%d, index:%d",
        //        dbus_message_dict_entry_get_arg_type(&prop_val), type, *prop_index);
        printf("Not the type we expect!\n");
        return false;
      }
      dbus_message_iter_get_basic(&prop_val, &val);
      return true;
    } while(dbus_message_iter_next(&dict));
    return false;
  }

  
protected:
  DBusConnection* mConnection;  
};

RawDBusConnection::RawDBusConnection() {
}

RawDBusConnection::~RawDBusConnection() {
}

bool RawDBusConnection::createDBusConnection() {
  DBusError err;
  dbus_error_init(&err);
  dbus_threads_init_default();
  mConnection = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err)) {
    dbus_error_free(&err);
    return false;
  }
  dbus_connection_set_exit_on_disconnect(mConnection, FALSE);
  return true;
}


// If err is NULL, then any errors will be LOGE'd, and free'd and the reply
// will be NULL.
// If err is not NULL, then it is assumed that dbus_error_init was already
// called, and error's will be returned to the caller without logging. The
// return value is NULL iff an error was set. The client must free the error if
// set.
DBusMessage * RawDBusConnection::dbus_func_args_timeout_valist(int timeout_ms,
                                                               DBusError *err,
                                                               const char *path,
                                                               const char *ifc,
                                                               const char *func,
                                                               int first_arg_type,
                                                               va_list args) {

  DBusMessage *msg = NULL, *reply = NULL;
  const char *name;
  bool return_error = (err != NULL);

  if (!return_error) {
    err = (DBusError*)malloc(sizeof(DBusError));
    dbus_error_init(err);
  }

  /* Compose the command */
  msg = dbus_message_new_method_call(BLUEZ_DBUS_BASE_IFC, path, ifc, func);

  if (msg == NULL) {
    //LOGE("Could not allocate D-Bus message object!");
    goto done;
  }

  /* append arguments */
  if (!dbus_message_append_args_valist(msg, first_arg_type, args)) {
    //LOGE("Could not append argument to method call!");
    goto done;
  }

  /* Make the call. */
  reply = dbus_connection_send_with_reply_and_block(mConnection, msg, timeout_ms, err);
  if (!return_error && dbus_error_is_set(err)) {
    //LOG_AND_FREE_DBUS_ERROR_WITH_MSG(err, msg);
  }

done:
  if (!return_error) {
    free(err);
  }
  if (msg) dbus_message_unref(msg);
  return reply;
}

DBusMessage * RawDBusConnection::dbus_func_args_timeout(int timeout_ms,
                                                        const char *path,
                                                        const char *ifc,
                                                        const char *func,
                                                        int first_arg_type,
                                                        ...) {
  DBusMessage *ret;
  va_list lst;
  va_start(lst, first_arg_type);
  ret = dbus_func_args_timeout_valist(timeout_ms, NULL,
                                      path, ifc, func,
                                      first_arg_type, lst);
  va_end(lst);
  return ret;
}

DBusMessage * RawDBusConnection::dbus_func_args(                           
  const char *path,
  const char *ifc,
  const char *func,
  int first_arg_type,
  ...) {
  DBusMessage *ret;
  va_list lst;
  va_start(lst, first_arg_type);
  ret = dbus_func_args_timeout_valist(-1, NULL,
                                      path, ifc, func,
                                      first_arg_type, lst);
  va_end(lst);
  return ret;
}

DBusMessage * RawDBusConnection::dbus_func_args_error(DBusError *err,
                                                      const char *path,
                                                      const char *ifc,
                                                      const char *func,
                                                      int first_arg_type,
                                                      ...) {
  DBusMessage *ret;
  va_list lst;
  va_start(lst, first_arg_type);
  ret = dbus_func_args_timeout_valist(-1, err,
                                      path, ifc, func,
                                      first_arg_type, lst);
  va_end(lst);
  return ret;
}

int32_t RawDBusConnection::dbus_returns_int32(DBusMessage *reply) {

  DBusError err;
  int ret = -1;

  dbus_error_init(&err);
  if (!dbus_message_get_args(reply, &err,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INVALID)) {
    //LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, reply);
  }
  dbus_message_unref(reply);
  return ret;
}

uint32_t RawDBusConnection::dbus_returns_uint32(DBusMessage *reply) {

  DBusError err;
  int ret = -1;

  dbus_error_init(&err);
  if (!dbus_message_get_args(reply, &err,
                             DBUS_TYPE_UINT32, &ret,
                             DBUS_TYPE_INVALID)) {
    //LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, reply);
  }
  dbus_message_unref(reply);
  return ret;
}

// nsString RawDBusConnection::dbus_returns_string(DBusMessage *reply) {

//   DBusError err;
//   nsString ret = NS_LITERAL_STRING("test");
//   const char *name;

//   dbus_error_init(&err);
//   if (dbus_message_get_args(reply, &err,
//                             DBUS_TYPE_STRING, &name,
//                             DBUS_TYPE_INVALID)) {
//     //ret = env->NewStringUTF(name);
//   } else {
//     //LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, reply);
//   }
//   dbus_message_unref(reply);

//   return ret;
// }

bool RawDBusConnection::dbus_returns_boolean(DBusMessage *reply) {
  DBusError err;
  bool ret = false;
  dbus_bool_t val = false;

  dbus_error_init(&err);

  /* Check the return value. */
  if (dbus_message_get_args(reply, &err,
                            DBUS_TYPE_BOOLEAN, &val,
                            DBUS_TYPE_INVALID)) {
    ret = val == true ? true : false;
  } else {
    //LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, reply);
  }

  dbus_message_unref(reply);
  return ret;
}

class BluetoothAdapter : public RawDBusConnection {
public:
  BluetoothAdapter();
  ~BluetoothAdapter() {}

  void get_adapter_path ();
  void get_adapter_properties();
  const char* mAdapterPath;
  bool mPowered;
  bool mDiscoverable;
  uint32_t mClass;
  const char* mAddress;
  const char* mName;
  bool mPairable;
  uint32_t mPairableTimeout;
  uint32_t mDiscoverableTimeout;
  bool mDiscovering;
};

BluetoothAdapter::BluetoothAdapter() {
  
}

void BluetoothAdapter::get_adapter_path() {
  DBusMessage *msg = NULL, *reply = NULL;
  DBusError err;
  const char *device_path = NULL;
  int attempt = 0;

  for (attempt = 0; attempt < 1000 && reply == NULL; attempt ++) {
    msg = dbus_message_new_method_call("org.bluez", "/",
                                       "org.bluez.Manager", "DefaultAdapter");
    if (!msg) {
      printf("%s: Can't allocate new method call for get_adapter_path!",
             __FUNCTION__);
      return;
    }
    dbus_message_append_args(msg, DBUS_TYPE_INVALID);
    dbus_error_init(&err);
    reply = dbus_connection_send_with_reply_and_block(mConnection, msg, -1, &err);

    if (!reply) {
      if (dbus_error_is_set(&err)) {
        if (dbus_error_has_name(&err,
                                "org.freedesktop.DBus.Error.ServiceUnknown")) {
          // bluetoothd is still down, retry
          //LOG_AND_FREE_DBUS_ERROR(&err);
          printf("Service unknown\n");
          usleep(10000);  // 10 ms
          continue;
        } else {
          // Some other error we weren't expecting
          printf("other error\n");
          //LOG_AND_FREE_DBUS_ERROR(&err);
        }
      }
      goto failed;
    }
  }
  if (attempt == 1000) {
    printf("timeout\n");
    //LOGE("Time out while trying to get Adapter path, is bluetoothd up ?");
    goto failed;
  }

  if (!dbus_message_get_args(reply, &err, DBUS_TYPE_OBJECT_PATH,
                             &device_path, DBUS_TYPE_INVALID)
      || !device_path){
    if (dbus_error_is_set(&err)) {
      //LOG_AND_FREE_DBUS_ERROR(&err);
    }
    goto failed;
  }
  dbus_message_unref(msg);
  printf("Adapter path: %s\n", device_path);
  mAdapterPath = device_path;
  return;
failed:
  dbus_message_unref(msg);
}

void BluetoothAdapter::get_adapter_properties() {
  DBusMessage *msg, *reply;
  DBusError err;
  dbus_error_init(&err);

  reply = dbus_func_args(mAdapterPath,
                         DBUS_ADAPTER_IFACE, "GetProperties",
                         DBUS_TYPE_INVALID);
  if (!reply) {
    if (dbus_error_is_set(&err)) {
      //LOG_AND_FREE_DBUS_ERROR(&err);
      printf("Error!\n");
    } else
      printf("DBus reply is NULL in function %s\n", __FUNCTION__);
    return ;
  }
  
  DBusMessageIter iter;
  //jobjectArray str_array = NULL;
  if (!dbus_message_iter_init(reply, &iter)) {
    printf("Return value is wrong!\n");
    dbus_message_unref(reply);
    return;
  }
  //str_array = parse_adapter_properties(env, &iter);
  GetDBusDictValue<const char*>(iter, "Name", DBUS_TYPE_STRING, mName);
  printf("name! %s\n", mName);
  dbus_message_unref(reply);
  printf("Adapter properties got\n");
}

class DBusThread : protected RawDBusConnection {
public:
	DBusThread();
	~DBusThread();
	bool setUpEventLoop();
	static DBusHandlerResult event_filter(DBusConnection *conn, DBusMessage *msg,
																				void *data);

};

DBusThread::DBusThread()
{
	createDBusConnection();
}

DBusThread::~DBusThread()
{
}

bool
DBusThread::setUpEventLoop()
{
	if(!mConnection) {
		return false;
	}
		
	dbus_threads_init_default();
	DBusError err;
	dbus_error_init(&err);

	// Add a filter for all incoming messages_base
	if (!dbus_connection_add_filter(mConnection, DBusThread::event_filter, this, NULL)){
		return false;
	}

	// Set which messages will be processed by this dbus connection
	dbus_bus_add_match(mConnection,
										 "type='signal',interface='org.freedesktop.DBus'",
										 &err);
	if (dbus_error_is_set(&err)) {
		//LOG_AND_FREE_DBUS_ERROR(&err);
		return false;
	}
	dbus_bus_add_match(mConnection,
										 "type='signal',interface='"BLUEZ_DBUS_BASE_IFC".Adapter'",
										 &err);
	if (dbus_error_is_set(&err)) {
		return false;
	}
	dbus_bus_add_match(mConnection,
										 "type='signal',interface='"BLUEZ_DBUS_BASE_IFC".Device'",
										 &err);
	if (dbus_error_is_set(&err)) {
		return false;
	}
	dbus_bus_add_match(mConnection,
										 "type='signal',interface='org.bluez.AudioSink'",
										 &err);
	if (dbus_error_is_set(&err)) {
		return false;
	}
	
	// const char *agent_path = "/android/bluetooth/agent";
	// const char *capabilities = "DisplayYesNo";
	// if (register_agent(nat, agent_path, capabilities) < 0) {
	// 	dbus_connection_unregister_object_path (nat->conn, agent_path);
	// 	return JNI_FALSE;
	// }
	return true;
}

// Called by dbus during WaitForAndDispatchEventNative()
DBusHandlerResult DBusThread::event_filter(DBusConnection *conn, DBusMessage *msg,
                                      void *data) {
//     native_data_t *nat;
//     JNIEnv *env;
//     DBusError err;
//     DBusHandlerResult ret;

//     dbus_error_init(&err);

//     nat = (native_data_t *)data;
//     nat->vm->GetEnv((void**)&env, nat->envVer);
//     if (dbus_message_get_type(msg) != DBUS_MESSAGE_TYPE_SIGNAL) {
//         LOGV("%s: not interested (not a signal).", __FUNCTION__);
//         return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
//     }

//     LOGE("%s: Received signal %s:%s from %s", __FUNCTION__,
//         dbus_message_get_interface(msg), dbus_message_get_member(msg),
//         dbus_message_get_path(msg));

//     env->PushLocalFrame(EVENT_LOOP_REFS);
//     if (dbus_message_is_signal(msg,
//                                "org.bluez.Adapter",
//                                "DeviceFound")) {
//         char *c_address;
//         DBusMessageIter iter;
//         jobjectArray str_array = NULL;
//         if (dbus_message_iter_init(msg, &iter)) {
//             dbus_message_iter_get_basic(&iter, &c_address);
//             if (dbus_message_iter_next(&iter))
//                 str_array =
//                     parse_remote_device_properties(env, &iter);
//         }
//         if (str_array != NULL) {
//             env->CallVoidMethod(nat->me,
//                                 method_onDeviceFound,
//                                 env->NewStringUTF(c_address),
//                                 str_array);
//         } else
//             LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, msg);
//         goto success;
//     } else if (dbus_message_is_signal(msg,
//                                      "org.bluez.Adapter",
//                                      "DeviceDisappeared")) {
//         char *c_address;
//         if (dbus_message_get_args(msg, &err,
//                                   DBUS_TYPE_STRING, &c_address,
//                                   DBUS_TYPE_INVALID)) {
//             LOGV("... address = %s", c_address);
//             env->CallVoidMethod(nat->me, method_onDeviceDisappeared,
//                                 env->NewStringUTF(c_address));
//         } else LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, msg);
//         goto success;
//     } else if (dbus_message_is_signal(msg,
//                                      "org.bluez.Adapter",
//                                      "DeviceCreated")) {
//         char *c_object_path;
//         if (dbus_message_get_args(msg, &err,
//                                   DBUS_TYPE_OBJECT_PATH, &c_object_path,
//                                   DBUS_TYPE_INVALID)) {
//             LOGV("... address = %s", c_object_path);
//             env->CallVoidMethod(nat->me,
//                                 method_onDeviceCreated,
//                                 env->NewStringUTF(c_object_path));
//         } else LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, msg);
//         goto success;
//     } else if (dbus_message_is_signal(msg,
//                                      "org.bluez.Adapter",
//                                      "DeviceRemoved")) {
//         char *c_object_path;
//         if (dbus_message_get_args(msg, &err,
//                                  DBUS_TYPE_OBJECT_PATH, &c_object_path,
//                                  DBUS_TYPE_INVALID)) {
//            LOGV("... Object Path = %s", c_object_path);
//            env->CallVoidMethod(nat->me,
//                                method_onDeviceRemoved,
//                                env->NewStringUTF(c_object_path));
//         } else LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, msg);
//         goto success;
//     } else if (dbus_message_is_signal(msg,
//                                       "org.bluez.Adapter",
//                                       "PropertyChanged")) {
//         jobjectArray str_array = parse_adapter_property_change(env, msg);
//         if (str_array != NULL) {
//             /* Check if bluetoothd has (re)started, if so update the path. */
//             jstring property =(jstring) env->GetObjectArrayElement(str_array, 0);
//             const char *c_property = env->GetStringUTFChars(property, NULL);
//             if (!strncmp(c_property, "Powered", strlen("Powered"))) {
//                 jstring value =
//                     (jstring) env->GetObjectArrayElement(str_array, 1);
//                 const char *c_value = env->GetStringUTFChars(value, NULL);
//                 if (!strncmp(c_value, "true", strlen("true")))
//                     nat->adapter = get_adapter_path(nat->conn);
//                 env->ReleaseStringUTFChars(value, c_value);
//             }
//             env->ReleaseStringUTFChars(property, c_property);

//             env->CallVoidMethod(nat->me,
//                               method_onPropertyChanged,
//                               str_array);
//         } else LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, msg);
//         goto success;
//     } else if (dbus_message_is_signal(msg,
//                                       "org.bluez.Device",
//                                       "PropertyChanged")) {
//         jobjectArray str_array = parse_remote_device_property_change(env, msg);
//         if (str_array != NULL) {
//             const char *remote_device_path = dbus_message_get_path(msg);
//             env->CallVoidMethod(nat->me,
//                             method_onDevicePropertyChanged,
//                             env->NewStringUTF(remote_device_path),
//                             str_array);
//         } else LOG_AND_FREE_DBUS_ERROR_WITH_MSG(&err, msg);
//         goto success;
//     } else if (dbus_message_is_signal(msg,
//                                       "org.bluez.Device",
//                                       "DisconnectRequested")) {
//         const char *remote_device_path = dbus_message_get_path(msg);
//         env->CallVoidMethod(nat->me,
//                             method_onDeviceDisconnectRequested,
//                             env->NewStringUTF(remote_device_path));
//         goto success;
//     }

//     ret = a2dp_event_filter(msg, env);
//     env->PopLocalFrame(NULL);
//     return ret;

// success:
//     env->PopLocalFrame(NULL);
    return DBUS_HANDLER_RESULT_HANDLED;
}


int main(int argc, char** argv) {
	DBusThread t;
  BluetoothAdapter b;
  b.createDBusConnection();
	if(!t.setUpEventLoop()) {
		printf("Cannot set up event loop!\n");
		return 1;
	}
  b.get_adapter_path();
  b.get_adapter_properties();
	// b.start_dbus_thread();
	// b.start_device_discovery();
	// sleep(10000);
	// b.stop_device_discovery();
	return 0;
}
