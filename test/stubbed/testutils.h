#ifndef LOADED_TESTUTILS
#define LOADED_TESTUTILS

#define TEST                                                                   \
  before_each();                                                               \
  for (int i = 0; i < 1; i++, after_each())

#endif
