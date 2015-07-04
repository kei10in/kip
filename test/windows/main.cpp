#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <Objbase.h>


struct co_initialize {
  co_initialize() {
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
  }

  ~co_initialize() {
    CoUninitialize();
  }
};

co_initialize com_init;