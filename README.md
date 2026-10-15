

# TimerApp

An iOS app with a single START/STOP button. Time is measured in a C++ library.

Architecture: Swift (UI) → Objective-C++ wrapper → C++ library

## Requirements

- macOS 27.0.1 (Build 26A434)
- Xcode 27.0 (Build 27A266a)
- CMake 3.16 or higher (tested with 4.4.4)
- AppleClang 21.0.0 compiler with C++17 support
- iOS Simulator: iPhone 18 Pro, iOS 27.0
- Internet access on the first test build (GoogleTest is downloaded)

## Project structure

```
.
├── README.md
├── cpp-lib/                 C++ library
│   ├── CMakeLists.txt       build (static and dynamic)
│   ├── include/             public header
│   ├── src/                 implementation (timer.cpp)
│   └── tests/               unit tests (timer_tests.cpp)
└── ios-app/
    └── TimerApp/            Xcode project
        ├── ObjCWrapper/     Objective-C++ wrapper (TimerWrapper.h, TimerWrapper.mm)
        └── ...              Swift code (ContentView, TimerAppApp), bridging header
```

The project has three separate components: the C++ library (`cpp-lib`), the Objective-C++ wrapper (`ObjCWrapper`), and the Swift app (the remaining `TimerApp` files).

## Building the C++ library

Run from the `cpp-lib` folder.

Static library:

```
cmake -S . -B build-static -DBUILD_SHARED_LIBS=OFF
cmake --build build-static
```

Output: `libtimer.a`

Dynamic library:

```
cmake -S . -B build-shared -DBUILD_SHARED_LIBS=ON
cmake --build build-shared
```

Output: [file name, expected to be `libtimer.dylib`; verify with `ls build-shared`]

## Tests

Tests are written with GoogleTest (downloaded via CMake FetchContent). Test file: `cpp-lib/tests/timer_tests.cpp`.

From the `cpp-lib` folder:

```
cmake -S . -B build-test
cmake --build build-test
ctest --test-dir build-test --output-on-failure
```

Result: 100% tests passed out of 6.

| # | Test | What it checks |
|---|------|----------------|
| 1 | `TimerTest.StartReturnsZero` | `start()` returns 0 |
| 2 | `TimerTest.StopWithoutStartReturnsError` | `stop()` without `start()` returns an error code [value, e.g. -1] |
| 3 | `TimerTest.StopBeforeOneSecondReturnsZero` | `stop()` called in under a second returns 0 (partial seconds are discarded) |
| 4 | `TimerTest.StopReturnsFullSeconds` | after waiting [about 1 second], `stop()` returns 1 |
| 5 | `TimerTest.SecondStopReturnsError` | a repeated `stop()` after `stop()` returns an error code, since the object has already been destroyed |
| 6 | `TimerTest.RepeatedStartKeepsOriginalStartTime` | a repeated `start()` does not reset the countdown: time is measured from the first call |

## Running the iOS app

1. Open `ios-app/TimerApp/[name].xcodeproj` in Xcode.
2. Select the iPhone 18 Pro simulator (iOS 27.0).
3. Press Cmd + R.

The C++ library is linked to the wrapper statically. [Describe how it is built for iOS: with a separate CMake command (build-ios-iphoneos and build-ios-iphonesimulator folders) or automatically during the Xcode build. Include the command if one is needed.]

App behavior:

- Initial state: green START button.
- Tapping START: `start()` is called through the wrapper, and the button turns red with the text STOP.
- Tapping STOP: `stop()` is called, and an alert "XX seconds elapsed" appears with an OK button.
- After OK: the button turns green START again.
- The app works in Portrait and Landscape.

## Technical decisions

- **Time is measured only in C++.** Swift and Objective-C code do not measure time: the wrapper only calls the library functions and returns the result.
- **`std::chrono::steady_clock`, not `system_clock`.** A monotonic clock does not jump when the system time changes (synchronization, time zone change, manual adjustment), so the measured interval stays correct.
- **A separate timer class and `std::unique_ptr`.** The object is created in `start()` and held in a `unique_ptr`. `stop()` calls `reset()`, and the object is destroyed. There is no manual `new`/`delete`, so leaks are ruled out.
- **Whole seconds.** The result is computed with `duration_cast<seconds>`, and the fractional part is discarded.
- **Repeated `start()`.** It does not restart the countdown: the object already exists, and time is measured from the first call. [In your own words: why this behavior was chosen.]
- **`stop()` without `start()`.** Returns an error code [value]. [How the app shows it: a separate message instead of "Прошло XX секунд", and the button stays/returns to START.]
- **Repeated `stop()`.** The object has already been destroyed, so the same error code is returned.
- **Wrapper header without C++.** `TimerWrapper.h` contains only Objective-C, and C++ is included in `TimerWrapper.mm` (Objective-C++). Swift can only import Objective-C, so C++ types and headers must not appear in the public `.h`.
- **Bridging header.** The file `[name]-Bridging-Header.h` contains `#import "TimerWrapper.h"`, and its path is set in Build Settings → Objective-C Bridging Header.
- **Static linking.** [How exactly the C++ library is linked to the wrapper: the built `.a` is added to the target, or the C++ sources are compiled directly. Describe it for your project.]
- **UI.** [SwiftUI or UIKit.] [How rotation is handled: e.g. the button is centered using layout, with no fixed coordinates.]

## Memory checks

Memory was checked with two tools: Address Sanitizer (memory access errors) and Instruments → Leaks (leaks).

### 1. Address Sanitizer: C++ tests

From the `cpp-lib` folder:

```
cmake -S . -B build-asan -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g"
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
```

If there are no errors, all tests pass and the output contains no `AddressSanitizer` lines.

### 2. Address Sanitizer: the app

1. In Xcode, open Product → Scheme → Edit Scheme (Cmd + <).
2. Select Run, then the Diagnostics tab.
3. Check Address Sanitizer.
4. Run the app with Cmd + R (not Cmd + I).
5. Perform 20 cycles: START → STOP → OK.
6. Watch the Xcode console. If there are no errors, the console stays free of `AddressSanitizer` reports.
7. After the check, uncheck the option (optional).

Do not enable Address Sanitizer at the same time as profiling in Instruments.

### 3. Instruments → Leaks

1. Select the iPhone 18 Pro simulator; Address Sanitizer must be off.
2. Choose Product → Profile (Cmd + I) and select the Leaks template.
3. Click the red Record button. The app launches in the simulator.
4. Wait 5–10 seconds for the first leak check to complete.
5. Select the Allocations track and click Mark Generation (button at the bottom of the window).
6. Perform 20 cycles of START → STOP → OK.
7. Wait 15–20 seconds so Leaks can run checks after the last cycle.
8. Click Mark Generation again and stop the recording.
9. Inspect the Leaks track. A red ✕ icon indicates a detected leak. If no new ✕ appear after the series of cycles, leaks are not growing.
10. In the Detail Filter field, enter `Timer` and make sure there are no live objects of the timer class.

### Results

- **Instruments → Leaks:** 20 START/STOP cycles (6 minutes 21 seconds). Leaks do not grow with the number of cycles. The only recorded leak (32 bytes) occurs at app launch and is unrelated to the project code. 

- **Address Sanitizer, C++ tests:** 6 of 6 passed, no errors.
- **Address Sanitizer, app:** 20 START/STOP cycles, no errors.

Address Sanitizer catches memory access errors (use-after-free, double free, out-of-bounds access). Leaks are covered by Instruments → Leaks.

## Known limitations

- Building the tests requires internet access (GoogleTest is downloaded via FetchContent).