#pragma once

#define EXPECT_ERROR(command, TException, message)                                                                     \
  EXPECT_THROW(                                                                                                        \
      {                                                                                                                \
        try {                                                                                                          \
          (command).Execute();                                                                                         \
        } catch (const TException& e) {                                                                                \
          EXPECT_EQ(e.what(), message);                                                                                \
          throw;                                                                                                       \
        }                                                                                                              \
      },                                                                                                               \
      TException);